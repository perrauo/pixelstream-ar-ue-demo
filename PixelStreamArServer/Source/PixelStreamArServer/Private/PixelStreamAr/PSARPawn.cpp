// Fill out your copyright notice in the Description page of Project Settings.


#include "PixelStreamAr/PSARPawn.h"

#include "PixelStreamingInputComponent.h"
#include "IPixelStreamingInputHandler.h"
#include "IPixelStreamingModule.h"
#include "IPixelStreamingInputModule.h"
#include "IPixelStreamingStreamer.h"
#include "Camera/PlayerCameraManager.h"


// Sets default values
APSARPawn::APSARPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APSARPawn::BeginPlay()
{
	Super::BeginPlay();

    if (auto PlayerController = Cast<APlayerController>(GetController()))
    {
        CameraManager = PlayerController->PlayerCameraManager;

        typedef EPixelStreamingMessageTypes EType;
        FPixelStreamingInputProtocol::ToStreamerProtocol.Add("CustomArTransform", FPixelStreamingInputMessage(110, {	// 4x4 Transform
        EType::Float, EType::Float, EType::Float, EType::Float,
        EType::Float, EType::Float, EType::Float, EType::Float,
        EType::Float, EType::Float, EType::Float, EType::Float,
        EType::Float, EType::Float, EType::Float, EType::Float,
            }));


        auto& PixelStreamingModule = IPixelStreamingModule::Get();
        auto Streamers = PixelStreamingModule.GetStreamerIds();
        if (Streamers.Num() != 0)
        {
            if (auto Streamer = PixelStreamingModule.FindStreamer(Streamers[0]))
            {
                if (auto InputHandler = Streamer->GetInputHandler().Pin())
                {
                    InputHandler->RegisterMessageHandler("CustomArTransform", [this](FString SourceId, FMemoryReader Ar) { HandleOnARTransform(Ar); });
                }
            }
        }
    }	
}

// Called every frame
void APSARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APSARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


FMatrix ExtractWebXRMatrix(FMemoryReader& Ar)
{
    FMatrix OutMat;
    for (int32 Row = 0; Row < 4; ++Row)

    {
        float Col0 = 0.0f, Col1 = 0.0f, Col2 = 0.0f, Col3 = 0.0f;
        Ar << Col0 << Col1 << Col2 << Col3;
        OutMat.M[Row][0] = Col0;
        OutMat.M[Row][1] = Col1;
        OutMat.M[Row][2] = Col2;
        OutMat.M[Row][3] = Col3;
    }
    OutMat.DiagnosticCheckNaN();
    return OutMat;
}

FTransform WebXRMatrixToUETransform(FMatrix Mat)
{
    // Rows and columns are swapped between raw mat and FMat
    FMatrix UEMatrix = FMatrix(
        FPlane(Mat.M[0][0], Mat.M[1][0], Mat.M[2][0], Mat.M[3][0]),
        FPlane(Mat.M[0][1], Mat.M[1][1], Mat.M[2][1], Mat.M[3][1]),
        FPlane(Mat.M[0][2], Mat.M[1][2], Mat.M[2][2], Mat.M[3][2]),
        FPlane(Mat.M[0][3], Mat.M[1][3], Mat.M[2][3], Mat.M[3][3]));
    // Extract & convert translation
    FVector Translation = FVector(-UEMatrix.M[3][2], UEMatrix.M[3][0], UEMatrix.M[3][1]) * 100.0f;
    // Extract & convert rotation
    FQuat RawRotation(UEMatrix);
    FQuat Rotation(-RawRotation.Z, RawRotation.X, RawRotation.Y, -RawRotation.W);
    return FTransform(Rotation, Translation, FVector(Mat.GetScaleVector(1.0f)));
}

void APSARPawn::HandleOnARTransform(FMemoryReader Ar)
{
    // The `Ar` buffer contains the transform matrix stored as 16 floats
    FTransform Transform = WebXRMatrixToUETransform(ExtractWebXRMatrix(Ar));       
    Transform.SetScale3D(FVector(1, 1, 1));
    //CameraManager->SetActorRotation(Transform.GetRotation());
    //Transform.SetRotation(FQuat::Identity);
    // Set the camera transform
    SetActorTransform(Transform);
}

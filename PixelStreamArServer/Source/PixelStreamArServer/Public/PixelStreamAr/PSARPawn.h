// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PSARPawn.generated.h"

class UPixelStreamingInput;
class IPixelStreamingInputHandler;
class APlayerCameraManager;
class FMemoryReader;

UCLASS()
class PIXELSTREAMARSERVER_API APSARPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APSARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY()
	TObjectPtr<APlayerCameraManager> CameraManager;

	void HandleOnARTransform(FMemoryReader Ar);

};

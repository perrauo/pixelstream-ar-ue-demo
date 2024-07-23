// Fill out your copyright notice in the Description page of Project Settings.


#include "PixelStreamAr/PSARPlaceable.h"

// Sets default values
AAPSARPlaceable::AAPSARPlaceable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAPSARPlaceable::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AAPSARPlaceable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


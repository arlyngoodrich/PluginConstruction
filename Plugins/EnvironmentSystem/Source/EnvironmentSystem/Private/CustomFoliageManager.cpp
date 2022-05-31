// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFoliageManager.h"

// Sets default values
ACustomFoliageManager::ACustomFoliageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomFoliageManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomFoliageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


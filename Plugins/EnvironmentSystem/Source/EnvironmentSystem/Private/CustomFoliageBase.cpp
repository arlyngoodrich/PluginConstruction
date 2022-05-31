// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFoliageBase.h"

#include "EnvironmentSystem.h"

// Sets default values
ACustomFoliageBase::ACustomFoliageBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void ACustomFoliageBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomFoliageBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UCustomFoliageISMC* ACustomFoliageBase::GetOriginatingFoliageISMC() const {return OriginatingFoliageISMC;}

void ACustomFoliageBase::SetReferences(UCustomFoliageISMC* SetOriginatingFoliageISMC)
{
	if(SetOriginatingFoliageISMC == nullptr)
	{
		UE_LOG(LogEnvironmentSystem,Error,TEXT("%s was given null Foliage ISMC"),
			*GetName())

		return;
	}
	
	OriginatingFoliageISMC = SetOriginatingFoliageISMC;
}


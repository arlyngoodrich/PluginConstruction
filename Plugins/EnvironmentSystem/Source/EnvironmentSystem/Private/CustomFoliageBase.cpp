// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFoliageBase.h"

#include "CustomFoliageISMC.h"
#include "EnvironmentSystem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACustomFoliageBase::ACustomFoliageBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

void ACustomFoliageBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACustomFoliageBase, OriginatingFoliageISMC);
	
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
	
	if(bInstanceRemoved == false)
	{
		if(OriginatingFoliageISMC)
		{
			RemoveInstance();
		}
	}
}

UCustomFoliageISMC* ACustomFoliageBase::GetOriginatingFoliageISMC() const {return OriginatingFoliageISMC;}

void ACustomFoliageBase::OnSpawned(UCustomFoliageISMC* SetOriginatingFoliageISMC, ACustomFoliageManager* SetFoliageManager)
{
	if(SetOriginatingFoliageISMC == nullptr)
	{
		UE_LOG(LogEnvironmentSystem,Error,TEXT("%s was given null Foliage ISMC"),
			*GetName())

		return;
	}
	
	OriginatingFoliageISMC = SetOriginatingFoliageISMC;
	FoliageManager = SetFoliageManager;
	RemoveInstance();
	
}

void ACustomFoliageBase::RemoveInstance()
{
	if(OriginatingFoliageISMC->RemoveInstanceAtLocation(GetActorLocation()) == false)
	{
		UE_LOG(LogEnvironmentSystem,Warning,TEXT("%s Failed To Remove Instance"),
			*GetName())
	}
	
	bInstanceRemoved = true;
}

void ACustomFoliageBase::RequestRemoval()
{
	if(HasAuthority() == false){return;}
	Multicast_AddInstance();
}


void ACustomFoliageBase::Multicast_AddInstance_Implementation()
{
	OriginatingFoliageISMC->AddInstance(GetActorTransform(),true);
	Destroy();
}


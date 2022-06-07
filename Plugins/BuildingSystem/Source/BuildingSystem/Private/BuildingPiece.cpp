// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPiece.h"

// Sets default values
ABuildingPiece::ABuildingPiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABuildingPiece::OnPlacementStart()
{
	if(HasAuthority())
	{
		SetReplicates(false);
		if(GetLocalRole()==ROLE_AutonomousProxy)
		{
			Destroy();
		}
	}

}

// Called when the game starts or when spawned
void ABuildingPiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class ABuildingPiece;

UCLASS()
class BUILDINGSYSTEM_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void CheckBuildingPieceIn(ABuildingPiece* BuildingPiece);

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void CheckBuildingPieceOut(ABuildingPiece* BuildingPiece);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	TArray<ABuildingPiece*> MyBuildingPieces;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingPiece.generated.h"

class UBuildingPieceSnapPoint;
class ABuilding;

UCLASS()
class BUILDINGSYSTEM_API ABuildingPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingPiece();

	UFUNCTION(BlueprintCallable,Category="Building System")
	bool GetShouldCheckForSnaps() const;

	/**
	 * @brief Called by Building Piece Spawner 
	 */
	void OnPlacementStart();

	/**
	 * @brief Blueprint version to check placement.  Defaults to native Internal_CheckPlacement method but can be overriden.
	 * @return True if OK to place, false if not.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Building System")
	bool CheckPlacement();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Called by Building Piece Spawning Component when spawn is set on  sever
	 * @param SetIsSnapped If the building piece was snapped during spawning
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void OnPlaced(bool SetIsSnapped);

	/**
	 * @brief Returns owning building.  Will be null if no owning building
	 * @return 
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	ABuilding* GetOwningBuilding();

	/**
	 * @brief Updates Owning Building for this piece. Can only be done on the server
	 * @param NewOwningBuilding New Owning Building 
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void SetOwningBuilding(ABuilding* NewOwningBuilding);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * @brief Overriden from Actor
	 * @param EndPlayReason reason play was ended.  
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/**
	 * @brief If the building should attempt to snap during spawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Building System")
	bool bCheckForSnaps = true;

	/**
	 * @brief If the building piece is snapped 
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	bool bIsSnapped;

	/**
	 * @brief Array of snap points that overlapping the building piece. 
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	TArray<UBuildingPieceSnapPoint*> SupportingSnapPoints;

	/**
	 * @brief Owning building piece
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	ABuilding* OwningBuilding = nullptr;

	/**
	* @brief Native version of check placement. 
	 * @return True if OK to place, false if not. 
	*/
	virtual bool Internal_CheckPlacement();

};

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

	/**
	 * @brief Gets if the piece should attempt to snap while spawning
	 * @return true if yes, false if no
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	bool GetShouldCheckForSnaps() const;

	/**
	 * @brief Returns the piece's instability.  0 being the most stable and higher numbers being more unstable.  
	 * @return Current instability of the piece
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	int32 GetCurrentInstability();

	/**
	 * @brief Called by Building Piece Spawner 
	 */
	void OnSpawnStart();

	/**
	 * @brief Blueprint version to check placement.  Defaults to native Internal_CheckPlacement method but can be overriden.
	 * @return True if OK to place, false if not.
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Building System")
	bool CheckPlacement(bool bIsSnappedDuringSpawn);


	/**
	 * @brief Checks for valid overlapping snap points and sets support points
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Buidling System")
	void UpdateSupportPoints();

	/**
	 * @brief Called by Building Piece Spawning Component when spawn is set on  sever
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void OnPlaced();

	/**
	 * @brief Returns owning building.  Will be null if no owning building
	 * @return Pointer to owning building piece. Can be nullptr.
	 */
	UFUNCTION(BlueprintCallable,Category="Building System")
	ABuilding* GetOwningBuilding() const;

	/**
	 * @brief Updates Owning Building for this piece. Can only be done on the server
	 * @param NewOwningBuilding New Owning Building 
	 */
	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly,Category="Building System")
	void SetOwningBuilding(ABuilding* NewOwningBuilding);

	/**
	 * @brief Called when this piece should be destroyed during gameplay.  WIll do an RPC if not authority. 
	 */
	UFUNCTION(BlueprintCallable,Category="Buildng System")
	void RemoveBuildingPiece();
		
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	 * @brief Max amount of Instability before piece breaks from building
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Building System")
	int32 MaxInstability = 3;

	/**
	 * @brief Current amount of Instability
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	int32 CurrentInstability;

	/**
	 * @brief If the building piece is snapped 
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	bool bIsSnapped;

	/**
	* @brief If the building piece is overlapping World Static (IE Landscape).   
	*/
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	bool bIsOverlappingWorldStatic;

	/**
	* @brief If the building piece is overlapping other building pieces(IE Landscape).   
	*/
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Building System")
	bool bIsOverlappingBuildingPiece;

	/**
 * @brief Array of snap points that overlapping the building piece. 
 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	TArray<ABuildingPiece*> SupportingBuildingPieces;

	/**
	 * @brief Owning building piece
	 */
	UPROPERTY(BlueprintReadOnly,Replicated,Category="Building System")
	ABuilding* OwningBuilding = nullptr;

	/**
	* @brief Native version of check placement. 
	 * @return True if OK to place, false if not. 
	*/
	virtual bool Internal_CheckPlacement(bool bIsSnappedDuringSpawn);

	/**
	 * @brief If in world static, instability will be 0. If connected to building piece, instability will be 1 more than
	 * the minimum instability 
	 */
	UFUNCTION()
	void CalculateInstability();

	/**
	 * @brief RCP for removing a building piece during gameplay
	 */
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_RemoveBuildingPiece();
	bool Server_RemoveBuildingPiece_Validate();
	void Server_RemoveBuildingPiece_Implementation();
	
};

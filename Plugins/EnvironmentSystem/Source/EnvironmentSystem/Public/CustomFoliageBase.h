// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomFoliageBase.generated.h"

class UCustomFoliageISMC;
class ACustomFoliageManager;

UCLASS()
class ENVIRONMENTSYSTEM_API ACustomFoliageBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomFoliageBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UCustomFoliageISMC* GetOriginatingFoliageISMC() const;
	
	/**
	 * @brief Called by Foliage Manager when actor is spawned
	 * @param SetOriginatingFoliageISMC FoliageISMC to return instance to 
	 * @param SetFoliageManager Owning foliage manager reference 
	 */
	void OnSpawned(UCustomFoliageISMC* SetOriginatingFoliageISMC, ACustomFoliageManager* SetFoliageManager);

	/**
	 * @brief Request Removal of foliage actor and replace with foliage instance
	 */
	void RequestRemoval();

protected:

	/**
	 * @brief Foliage ISMC to return instance to. OnRep used to remove instance when actor spawned in for client
	 */
	UPROPERTY(ReplicatedUsing=OnRep_RemoveInstance,BlueprintReadOnly,Category="Foliage")
	UCustomFoliageISMC* OriginatingFoliageISMC;

	/**
	 * @brief Foliage Manager references set from OnSpawned
	 */
	UPROPERTY(BlueprintReadOnly,Category="Foliage")
	ACustomFoliageManager* FoliageManager;

	/**
	 * @brief Called when OriginatingFoliageISMC is set
	 */
	UFUNCTION()
	void OnRep_RemoveInstance() const;

	/**
	 * @brief Used to Add instances back to Originating Foliage ISMC for all players 
	 */
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_AddInstance();
	void Multicast_AddInstance_Implementation();

};

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
	
	void SetReferences(UCustomFoliageISMC* SetOriginatingFoliageISMC, ACustomFoliageManager* SetFoliageManager);


protected:

	UPROPERTY(BlueprintReadOnly,Category="Foliage")
	UCustomFoliageISMC* OriginatingFoliageISMC;

	UPROPERTY(BlueprintReadOnly,Category="Foliage")
	ACustomFoliageManager* FoliageManager;
	

};

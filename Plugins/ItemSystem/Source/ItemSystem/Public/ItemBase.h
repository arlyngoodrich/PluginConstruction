// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ItemBase.generated.h"

UCLASS()
class ITEMSYSTEM_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(Replicated, EditAnywhere,BlueprintReadOnly,Category="Item Data")
	FItemData ItemData;

	//Adds GUID and Item in world class to Item Data struct
	void InitializeItemData();

};

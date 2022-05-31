// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomFoliageManager.generated.h"

UCLASS()
class ENVIRONMENTSYSTEM_API ACustomFoliageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomFoliageManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CheckInActor(AActor* ActorToTrack);

protected:

	TArray<AActor*> TrackedActors;

};

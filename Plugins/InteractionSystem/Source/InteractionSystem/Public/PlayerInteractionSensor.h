// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionSensor.generated.h"

class UInteractableObjectComponent;
class APlayerController;

UCLASS( ClassGroup=(InteractionSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerInteractionSensor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractionSensor();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bInteractableObjectInView;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	UInteractableObjectComponent* InteractableObjectComponentInView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	float InteractionDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;

	bool bShouldCheckForInteractable;

	APlayerController* OwningController;

	AActor* ActorInView;

	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void Interact();

	void TriggerInteraction(UInteractableObjectComponent* ComponentInView) const;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerInteraction(UInteractableObjectComponent* ComponentInView);
	bool Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView);
	void Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView);

	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void ToggleInteraction(bool bShouldCheckForInteraction);

	UFUNCTION()
	void InteractionCheckLoop();

	UFUNCTION()
	bool GetHitActorInView(AActor*& HitActor) const;

	UFUNCTION()
	static bool GetInteractableComponent(const AActor* HitActor, UInteractableObjectComponent*& HitActorInteractableObjectComponent);
		
		
};

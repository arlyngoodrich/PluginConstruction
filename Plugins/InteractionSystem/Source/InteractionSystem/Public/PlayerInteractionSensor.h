// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionSensor.generated.h"

class UInteractableObjectComponent;
class APlayerController;

/**
 * @brief Component attached to player character that will "look" for actors with an Interaction Component.  Will focus on
 * those actors and call interaction events.  
 */
UCLASS( ClassGroup=(InteractionSystem), blueprintable, meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UPlayerInteractionSensor : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets default values for this component's properties
	 */
	UPlayerInteractionSensor();
	
	/**
	 * @brief Tick function for component
	 * @param DeltaTime Time between ticks
	 * @param TickType Type of Tick
	 * @param ThisTickFunction Tick function
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/**
	 * @brief Called when the game starts
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Boolean property for if an interactable object is in view.   
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bInteractableObjectInView;
	
	/**
	 * @brief Pointer to interactable object component that is in view.  Will but null if no actors with an IOC is in view. 
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	UInteractableObjectComponent* InteractableObjectComponentInView;

	
	/**
	 * @brief Float property to see the maximum distance for the line trace to check for actors with IOCs.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	float InteractionDistance;
	
	/**
	 * @brief Boolean property to add a debug line to interaction check trace.  
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;
	
	/**
	 * @brief Boolean that controls the interaction loop. Use case is to turn of interaction check if a UI menu is open.
	 * Set by Toggle Interaction function.
	 */
	UPROPERTY(BlueprintReadOnly,Category = "Debug")
	bool bShouldCheckForInteractable;

	
	/**
	 * @brief Owning player controller for pawn or character component is attached to. 
	 */
	UPROPERTY(BlueprintReadOnly,Category="Interaction System")
	APlayerController* OwningController;


	/**
	* @brief Actor returned by the line trace.  May or may not have an interactable object component.
	* Used for comparisons between frames to determine focus.  Should use Owner of Interactable Object Component to get
	* parent actor in view.  
	 */
	UPROPERTY(BlueprintReadOnly,Category="Interaction System")
	AActor* ActorInView;
	
	/**
	 * @brief Ensures component owner is a player controlled pawn and starts interaction loop. 
	 */
	void Initialize();

	
	/**
	 * @brief Called by player to interact with object in view.  Ideally this is bound to an input binding.
	 * Will do an RPC if player is a client. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void Interact();
	
	/**
	 * @brief Native implementation to interact with IOC in view.  Will do nothing if not in view. 
	 * @param ComponentInView Component from actor that interaction was triggered on
	 */
	void TriggerInteraction(UInteractableObjectComponent* ComponentInView) const;
	
	/**
	 * @brief RPC for the client to trigger interaction.  Component for actor that was interacted with.  
	 * @param ComponentInView 
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerInteraction(UInteractableObjectComponent* ComponentInView);
	bool Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView);
	void Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView);
	
	/**
	 * @brief Function that can toggles interaction.  Useful for when a UI is open.  
	 * @param bShouldCheckForInteraction Toggle for if interaction should occur. True to perform interaction checks, false
	 * to stop them. 
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void ToggleInteraction(bool bShouldCheckForInteraction);

	
	/**
	 * @brief Function loop that ticks every frame.  Checks for an interactable object in view.
	 */
	UFUNCTION()
	void InteractionCheckLoop();
	
	/**
	 * @brief Helper function to get the hit actor from the line trace
	 * @param HitActor Out actor that was hit
	 * @return True if an actor was hit, false if nothing was hit
	 */
	UFUNCTION()
	bool GetHitActorInView(AActor*& HitActor) const;

	
	/**
	 * @brief Helper function to retrieve interactable object from an actor 
	 * @param HitActor Actor to check for interactable
	 * @param HitActorInteractableObjectComponent Out interactable component from actor
	 * @return True if hit actor had an interactable component, false if it did not. 
	 */
	UFUNCTION()
	static bool GetInteractableComponent(const AActor* HitActor, UInteractableObjectComponent*& HitActorInteractableObjectComponent);
		
		
};

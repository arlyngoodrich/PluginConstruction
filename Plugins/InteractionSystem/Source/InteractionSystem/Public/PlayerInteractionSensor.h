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

	FVector GetLookLocation() const;

	FHitResult GetLookHitResult() const;

	//Function that can toggles look checks 
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void ToggleLookChecks(bool bShouldPerformLookChecks);
	
	//Function that can toggles interaction 
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void ToggleInteraction(bool bSetInteractionOK);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Boolean property for if an interactable object is in view.   
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bInteractableObjectInView;

	//Pointer to interactable object component that is in view.  Will but null if no actors with an IOC is in view. 
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	UInteractableObjectComponent* InteractableObjectComponentInView;

	//Float property to see the maximum distance for the line trace to check for actors with IOCs.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction System")
	float InteractionDistance;

	//Boolean property to add a debug line to interaction check trace.  
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;

	//Boolean that controls the interaction loop. Use case is to turn of interaction loop if a UI menu is open.  Set by ToggleInteraction function.  
	UPROPERTY(BlueprintReadOnly,Category = "Interaction System")
	bool bShouldDoLookChecks;
	
	UPROPERTY(BlueprintReadOnly,Category = "Interaction System")
	bool bInteractionOK;

	//Owning player controller for pawn or character component is attached to. 
	UPROPERTY(BlueprintReadOnly,Category="Interaction System")
	APlayerController* OwningController;

	//Actor returned by the line trace.  May or may not have an interactable object component.  Used for comparisons between frames
	//to determine focus.  Should use Owner of Interactable Object Component to get parent actor in view.  
	UPROPERTY(BlueprintReadOnly,Category="Interaction System")
	AActor* ActorInView;

	UPROPERTY(BlueprintReadOnly,Category="Interaction System")
	FVector LookLocation;

	UPROPERTY(BlueprintReadOnly,Category="Interaction System")
	FHitResult LookHitResult;

	//Ensures component owner is a player controlled pawn and starts interaction loop.  
	void Initialize();

	//Called by player to interact with object in view.  Ideally this is bound to an input binding.  Will do an RPC if player is a client. 
	UFUNCTION(BlueprintCallable, Category = "Interaction System")
	void Interact();

	//Native implementation to interact with IOC in view.  Will do nothing if not in view.  
	void TriggerInteraction(UInteractableObjectComponent* ComponentInView) const;

	//RPC for the client to trigger interaction
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TriggerInteraction(UInteractableObjectComponent* ComponentInView);
	bool Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView);
	void Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView);
	

	//Function loop that ticks every frame.  Checks for an interactable object in view.
	UFUNCTION()
	void InteractionCheckLoop();

	//Helper function to get the hit actor from the lince trace
	UFUNCTION()
	bool SetLookLocation(AActor*& HitActor);

	//Helper function to retrieve interactoable object from an actor 
	UFUNCTION()
	static bool GetInteractableComponent(const AActor* HitActor, UInteractableObjectComponent*& HitActorInteractableObjectComponent);
		
		
};

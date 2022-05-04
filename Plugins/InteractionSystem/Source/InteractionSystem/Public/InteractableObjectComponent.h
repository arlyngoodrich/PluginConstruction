// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableObjectComponent.generated.h"

//Delegate for called when interaction is triggered.   Only called on the server
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionTriggeredDelegate, AActor*, InstigatingActor);

class UInteractionWidget;
class APlayerController;

UCLASS( ClassGroup=(InteractionSystem), Blueprintable, meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableObjectComponent();

	//Called by the Player Interaction Sensor and is triggered by the player's input.  Should only be called on the server.
	void Interact(AActor* InstigatingActor);

	//Called by the Player Interaction Sensor during its interaction check loop.  Should only execute on the client. 
	void ToggleFocus(bool bNewIsInFocus);

	//Delegate for when interaction is called.  Should only happen on the server and should not be called on the client.  
	UPROPERTY(BlueprintAssignable, Category = "Interaction System")
	FInteractionTriggeredDelegate OnInteractionTriggered;

	//Boolean default property for if the base mesh components should outline if the object is in focus. 
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	bool bShouldOutline;

	//Stencil vale for the outline when the object is in focus.  Check the Outline material instance to see which colors it coorisponds to. 
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	int32 OutlineStencilValue;

	//Boolean Property for if the interaction UI should be shown
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	bool bShouldShowWidget;

	//Text that should be displayed in the interaction UI when the actor is in focus. 
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System",meta = (EditCondition = "bShouldShowWdiget"))
	FText InteractionText;
	
	//Widget class that should be shown on interaction 
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System", meta = (EditCondition = "bShouldShowWdiget"))
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

	//Pointer to created interaction widget 
	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	//Called when the actor is in view for the player interaction sensor.  Should only be called on the client.	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Focus Start")
	void BP_OnStartFocus();

	//Called when the actor is no longer in view for the player interaction sensor.  Should only be called on the client.	
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On End Focus")
	void BP_OnEndFocus();

	//Called when the player triggers the interaction.  Will only be called on the server.  
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Interaction Triggered")
	void BP_OnInteractionTriggered(AActor* InstigatingActor);
	
	//Boolean property for when the IOC's actor is in foucs.  
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bIsInFocus;

	UPROPERTY()
	AActor* OwningActor;

	void ToggleOutline(bool bStartOutline) const;

	void AddWidget();

	void RemoveWidget();
		
};

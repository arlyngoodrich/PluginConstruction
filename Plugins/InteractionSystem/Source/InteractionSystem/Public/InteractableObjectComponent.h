// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableObjectComponent.generated.h"


/**
 * @brief Delegate for called when interaction is triggered.   Only called on the server
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionTriggeredDelegate, AActor*, InstigatingActor);

class UInteractionWidget;
class APlayerController;

/**
 * @brief Component for any actor that should have interaction triggered by the player
 */
UCLASS( ClassGroup=(InteractionSystem), Blueprintable, meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief Sets default values for this component's properties
	 */
	UInteractableObjectComponent();
	
	/**
	 * @brief Called by the Player Interaction Sensor and is triggered by the player's input.  Should only be called on the server.
	 * @param InstigatingActor Actor that is triggering the interaction.  
	 */
	void Interact(AActor* InstigatingActor);
	
	/**
	 * @brief Called by the Player Interaction Sensor during its interaction check loop.  Should only execute on the client. 
	 * @param bNewIsInFocus True if in focus, false if not
	 */
	void ToggleFocus(bool bNewIsInFocus);
	
	/**
	 * @brief Delegate for when interaction is called.  Should only happen on the server and should not be called on the client. 
	 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction System")
	FInteractionTriggeredDelegate OnInteractionTriggered;

protected:
	/**
	 * @brief Called when the game starts
	 */
	virtual void BeginPlay() override;
	
	 
	/**
	 * @brief Boolean default property for if the base mesh components should outline if the object is in focus.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	bool bShouldOutline;
	
	/**
	 * @brief Stencil vale for the outline when the object is in focus.  Check the Outline material instance to see which
	 * colors it corresponds to. 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	int32 OutlineStencilValue;
	
	/**
	 * @brief Boolean Property for if the interaction UI should be shown
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	bool bShouldShowWidget;
	
	/**
	 * @brief Text that should be displayed in the interaction UI when the actor is in focus. 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System",meta = (EditCondition = "bShouldShowWdiget"))
	FText InteractionText;
	
	/**
	 * @brief Widget class that should be shown on interaction 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System", meta = (EditCondition = "bShouldShowWdiget"))
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	
	/**
	 * @brief Widget class that should be shown on interaction 
	 */
	UPROPERTY()
	UInteractionWidget* InteractionWidget;
	
	/**
	 * @brief Called when the actor is in view for the player interaction sensor.  Should only be called on the client.	
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Focus Start")
	void BP_OnStartFocus();
	
	/**
	 * @brief Called when the actor is no longer in view for the player interaction sensor.  Should only be called on the client.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On End Focus")
	void BP_OnEndFocus();
	
	/**
	 * @brief Called when the player triggers the interaction.  Will only be called on the server.  
	 * @param InstigatingActor Actor causing the interaction
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Interaction Triggered")
	void BP_OnInteractionTriggered(AActor* InstigatingActor);
	
	/**
	 * @brief Boolean property for when the IOC's actor is in focus.  
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bIsInFocus;

	/**
	 * @brief Helper function to toggle outline on the actor's mesh components
	 * @param bStartOutline True if the outline should start, false if not
	 */
	void ToggleOutline(bool bStartOutline) const;

	/**
	 * @brief Adds the interaction widget to the viewport when the owning actor is in focus
	 */
	void AddWidget();

	/**
	 * @brief Removes the widget from viewport when focus is lost
	 */
	void RemoveWidget();
		
};

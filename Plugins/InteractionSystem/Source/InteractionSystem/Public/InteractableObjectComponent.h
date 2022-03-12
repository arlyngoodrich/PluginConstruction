// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableObjectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionTriggeredDelegate, AActor*, InstigatingActor);

UCLASS( ClassGroup=(InteractionSystem), Blueprintable, meta=(BlueprintSpawnableComponent) )
class INTERACTIONSYSTEM_API UInteractableObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableObjectComponent();

	void Interact(AActor* InstigatingActor);

	void ToggleFocus(bool bNewIsInFocus);

	UPROPERTY(BlueprintAssignable, Category = "Interaction System")
	FInteractionTriggeredDelegate OnInteractionTriggered;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	bool bShouldOutline;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	FText InteractionText;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction System")
	int32 OutlineStencilValue;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Focus Start")
	void BP_OnStartFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On End Focus")
	void BP_OnEndFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction System", DisplayName = "On Interaction Triggered")
	void BP_OnInteractionTriggered(AActor* InstigatingActor);
	
	UPROPERTY(BlueprintReadOnly, Category = "Interaction System")
	bool bIsInFocus;

	void ToggleOutline(bool bStartOutline) const;
		
};

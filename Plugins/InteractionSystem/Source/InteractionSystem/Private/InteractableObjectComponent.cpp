// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjectComponent.h"
#include "InteractionSystem.h"

//UE4 Includes
#include "Components/MeshComponent.h"

// Sets default values for this component's properties
UInteractableObjectComponent::UInteractableObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bShouldOutline = true;
	OutlineStencilValue = 1;

	// ...
}


void UInteractableObjectComponent::Interact(AActor* InstigatingActor)
{

	if (!InstigatingActor)
	{
		UE_LOG(LogInteractionSystem, Error, TEXT("Interaction triggered on %s by null instigator"), *GetOwner()->GetName())
		return;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("Interaction called on %s"), *GetOwner()->GetName())

		BP_OnInteractionTriggered(InstigatingActor);
		OnInteractionTriggered.Broadcast(InstigatingActor);
	}
	else
	{
		UE_LOG(LogInteractionSystem,Warning,TEXT("Attempting to call interact with non-authority for %s"),*GetOwner()->GetName())
	}

}

void UInteractableObjectComponent::ToggleFocus(const bool bNewIsInFocus)
{
	bIsInFocus = bNewIsInFocus;

	if (bNewIsInFocus)
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("%s is now in focus"), *GetOwner()->GetName())
		BP_OnStartFocus();
		
		if (bShouldOutline)
		{
			ToggleOutline(true);
		}

	}
	else
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("%s is no longer in focus"), *GetOwner()->GetName())
		BP_OnEndFocus();

		if (bShouldOutline)
		{
			ToggleOutline(false);
		}

	}

	

}

// Called when the game starts
void UInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UInteractableObjectComponent::ToggleOutline(bool bStartOutline) const
{
	TArray<UMeshComponent*> MeshComponents;
	GetOwner()->GetComponents<UMeshComponent>(MeshComponents);

	for (int i = 0; i < MeshComponents.Num(); i++)
	{
		MeshComponents[i]->SetRenderCustomDepth(bStartOutline);
		MeshComponents[i]->SetCustomDepthStencilValue(OutlineStencilValue);
	}
}


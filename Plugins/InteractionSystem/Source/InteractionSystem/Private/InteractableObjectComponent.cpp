// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjectComponent.h"
#include "InteractionSystem.h"

//UE4 Includes
#include "InteractionWidget.h"
#include "Blueprint/UserWidget.h"
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

void UInteractableObjectComponent::OnOwnerDestroy(AActor* DestroyedActor)
{
	RemoveWidget();
}

// Called when the game starts
void UInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningActor = GetOwner();
	OwningActor->OnDestroyed.AddDynamic(this,&UInteractableObjectComponent::OnOwnerDestroy);
	// ...
}

void UInteractableObjectComponent::Interact(AActor* InstigatingActor)
{

	if (!InstigatingActor)
	{
		UE_LOG(LogInteractionSystem, Error, TEXT("Interaction triggered on %s by null instigator"), *GetOwner()->GetName())
		return;
	}

	if(IsValid(OwningActor) ==false)
	{
		RemoveWidget();
		return;
	}

	if (OwningActor->HasAuthority())
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

	if(IsValid(OwningActor) ==false)
	{
		RemoveWidget();
		return;
	}
	
	bIsInFocus = bNewIsInFocus;
	
	if (bNewIsInFocus)
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("%s is now in focus"), *OwningActor->GetName())

		BP_OnStartFocus();
		
		if (bShouldOutline)
		{
			ToggleOutline(true);
		}

		if(bShouldShowWidget)
		{
			AddWidget();
		}
	}
	else
	{
		UE_LOG(LogInteractionSystem, Log, TEXT("%s is no longer in focus"), *OwningActor->GetName())

		BP_OnEndFocus();

		if (bShouldOutline)
		{
			ToggleOutline(false);
		}

		if(bShouldShowWidget)
		{
			RemoveWidget();
		}
	}
}


void UInteractableObjectComponent::ToggleOutline(bool bStartOutline) const
{

	if(IsValid(OwningActor) ==false){return;}
	
	TArray<UMeshComponent*> MeshComponents;
	OwningActor->GetComponents<UMeshComponent>(MeshComponents);

	for (int i = 0; i < MeshComponents.Num(); i++)
	{
		MeshComponents[i]->SetRenderCustomDepth(bStartOutline);
		MeshComponents[i]->SetCustomDepthStencilValue(OutlineStencilValue);
	}
}

void UInteractableObjectComponent::AddWidget()
{
	if(IsValid(OwningActor) ==false){return;}
	
	if(bShouldShowWidget == false) {return;}
	
	//Make sure interaction widget class is valid
	if(InteractionWidgetClass == nullptr)
		{
			UE_LOG(LogInteractionSystem,Error,TEXT("Set Interaction Widget Class for %s"),*GetClass()->GetName())

			return;
		}

	InteractionWidget = Cast<UInteractionWidget>(CreateWidget(GetWorld(), InteractionWidgetClass.Get()));

	//Add to viewport if widget successfully created
	if(InteractionWidget)
	{
		InteractionWidget->AddToViewport();
		InteractionWidget->InteractionText = InteractionText;
	}
	
}

void UInteractableObjectComponent::RemoveWidget()
{
	
	if(InteractionWidget)
	{
		InteractionWidget->RemoveFromParent();
		InteractionWidget = nullptr;
	}
}


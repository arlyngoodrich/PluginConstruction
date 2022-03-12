// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractionSensor.h"
#include "InteractionSystem.h"

///UE4 Includes 
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "InteractableObjectComponent.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
UPlayerInteractionSensor::UPlayerInteractionSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	InteractionDistance = 500.f;
	bDrawDebug = true;
	// ...
}


// Called when the game starts
void UPlayerInteractionSensor::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

// Called every frame
void UPlayerInteractionSensor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bShouldCheckForInteractable)
	{
		InteractionCheckLoop();
	}

}

void UPlayerInteractionSensor::Initialize()
{
	ACharacter* OwningCharacterCheck = Cast<ACharacter>(GetOwner());

	if(!OwningCharacterCheck)
	{
		//UE_LOG(LogInteractionSystem, Error, 
				//TEXT("Interaction Sensor component does not have a character as owner.  Owner is %s"),
				//*GetOwner()->GetName()) 
	}
	else
	{
		APlayerController* OwningControllerCheck = Cast<APlayerController>(OwningCharacterCheck->GetController());

		if (!OwningControllerCheck)
		{
			//Not controlled by player character

		}
		else
		{
			OwningController = OwningControllerCheck;
			//UE_LOG(LogInteractionSystem, Log, TEXT("Interaction Sensor Component successfully initialized on %s"), *GetOwner()->GetName())
				
			ToggleInteraction(true);
		}
	}
}

void UPlayerInteractionSensor::Interact()
{
	if (bInteractableObjectInView)
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			TriggerInteraction(InteractableObjectComponentInView);
		}
		else
		{
			Server_TriggerInteraction(InteractableObjectComponentInView);
		}
	}
	else
	{
		//UE_LOG(LogInteractionSystem, Log, TEXT("No interactable in view for %s.  Disregarding request."), *GetOwner()->GetName())
	}
}

void UPlayerInteractionSensor::TriggerInteraction(UInteractableObjectComponent* ComponentInView) const
{
	ComponentInView->Interact(GetOwner());
}

bool UPlayerInteractionSensor::Server_TriggerInteraction_Validate(UInteractableObjectComponent* ComponentInView)
{
	return true;
}

void UPlayerInteractionSensor::Server_TriggerInteraction_Implementation(UInteractableObjectComponent* ComponentInView)
{
	TriggerInteraction(ComponentInView);
}

void UPlayerInteractionSensor::ToggleInteraction(bool bShouldCheckForInteraction)
{
	bShouldCheckForInteractable = bShouldCheckForInteraction;

	if (bShouldCheckForInteraction)
	{
		UE_LOG(LogInteractionSystem,Log,TEXT("Interaction Started for %s"),*GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogInteractionSystem,Log,TEXT("Interaction Stopped for %s"),*GetOwner()->GetName())
	}
}

void UPlayerInteractionSensor::InteractionCheckLoop()
{
	AActor* HitActorInView;

	//Perform hit scan to get hit actor
	if (GetHitActorInView(HitActorInView))
	{
		//If Actor in view is not the same as the new actor in view, set as new one.  
		//Otherwise don't check for interactable component if it's the same
		if (HitActorInView != ActorInView)
		{

			ActorInView = HitActorInView;

			//check if new actor has interactable actor
			UInteractableObjectComponent* NewInteractableObject;

			if (GetInteractableComponent(HitActorInView, NewInteractableObject))
			{
				//Tell old interactable no longer in focus
				if (InteractableObjectComponentInView)
				{
					InteractableObjectComponentInView->ToggleFocus(false);
				}
				
				
				InteractableObjectComponentInView = NewInteractableObject;
				InteractableObjectComponentInView->ToggleFocus(true);
				bInteractableObjectInView = true;
				return;
			}
		}
		else
		{
			//If the actor is the same, no need to make any changes
			return;
		}
	} 	//If there was previously an interactable in view but is no longer
	else if (InteractableObjectComponentInView)
	{
		InteractableObjectComponentInView->ToggleFocus(false);
		InteractableObjectComponentInView = nullptr;
		ActorInView = nullptr;
		bInteractableObjectInView = false;
	}

}

bool UPlayerInteractionSensor::GetHitActorInView(AActor*& HitActor) const
{

	if (!OwningController) { return false; }


	FVector StartLocation;
	FRotator ViewRotation;
	FVector EndLocation;
	FHitResult Hit;
	bool TraceResult;
	FCollisionQueryParams TraceParams;

	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(GetOwner());

	OwningController->GetPlayerViewPoint(StartLocation, ViewRotation);
	EndLocation = StartLocation + (ViewRotation.Vector() * InteractionDistance);

	TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, .1f);
	}

	if (TraceResult)
	{
		HitActor = Hit.GetActor();

		if (bDrawDebug)
		{
			DrawDebugBox(GetWorld(), Hit.Location, FVector(10.f, 10.f, 10.f), FColor::Red, false, .1f);
		}

		return true;
	}
	else
	{
		HitActor = nullptr;
		return false;
	}
}

bool UPlayerInteractionSensor::GetInteractableComponent(const AActor* HitActor, UInteractableObjectComponent*& HitActorInteractableObjectComponent)
{

	HitActorInteractableObjectComponent = HitActor->FindComponentByClass<UInteractableObjectComponent>();

	if (HitActorInteractableObjectComponent)
	{
		return true;
	}
	else
	{
		return false;
	}
}


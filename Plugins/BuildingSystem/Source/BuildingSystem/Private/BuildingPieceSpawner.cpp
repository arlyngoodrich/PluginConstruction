// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingPieceSpawner.h"

#include "BuildingPiece.h"
#include "BuildingPieceSnapPoint.h"
#include "BuildingSystem.h"
#include "CustomPlayerController.h"
#include "PlayerInteractionSensor.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UBuildingPieceSpawner::UBuildingPieceSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	//C:/GameDev/Projects/PluginConstruction/Plugins/BuildingSystem/Content/Materials/MI_BadPlacement.uasset

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	GoodMaterial(TEXT("MaterialInstanceConstant'/BuildingSystem/Materials/MI_GoodPlacement.MI_GoodPlacement'"));

	if(GoodMaterial.Object)
	{
		GoodPlacementMaterial = Cast<UMaterialInterface>(GoodMaterial.Object);
	}
	else
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("%s could not set default good spawn material"),*GetClass()->GetName())
	}

	//Get bad spawn default material
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	BadMaterial(TEXT("MaterialInstanceConstant'/BuildingSystem/Materials/MI_BadPlacement.MI_BadPlacement'"));

	if(BadMaterial.Object)
	{
		BadPlacementMaterial = Cast<UMaterialInterface>(BadMaterial.Object);
	}
	else
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("%s could not set default bad spawn material"),*GetClass()->GetName())
	}

	// ...
}

void UBuildingPieceSpawner::RequestBuildingSpawn(const TSubclassOf<ABuildingPiece> PieceToSpawn)
{
	if(bReferencesSet == false)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("Building Component attempted to spawn building peice without references set"))
		return;
	}
	
	if(PieceToSpawn == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("%s attempted to spawn null building piece class"),
			*GetOwner()->GetName())
		return;
	}

	if(bIsPlacingBuildingPiece == true)
	{
		UE_LOG(LogBuildingSystem,Warning,TEXT("%s is alreadying placing a piece"),
			*GetOwner()->GetName())
		return;
	}
	
	BuildingPieceClass = PieceToSpawn;
	StartSpawnLoop();
}


// Called when the game starts
void UBuildingPieceSpawner::BeginPlay()
{
	Super::BeginPlay();

	SetReferences();

	// ...
	
}



void UBuildingPieceSpawner::SetReferences()
{
	const APawn* OwningPawn = Cast<APawn>(GetOwner());
	if(OwningPawn == nullptr)
	{
		//UE_LOG(LogBuildingSystem,Error,TEXT("Building System Component not owned by pawn"))
		return;
	}

	ACustomPlayerController* Controller = Cast<ACustomPlayerController>(OwningPawn->GetController());
	if(Controller == nullptr)
	{
		//UE_LOG(LogBuildingSystem,Error,TEXT("Building System Component pawn controller not Custom Player Controller"))
		return;
	}

	CustomPlayerController = Controller;

	UPlayerInteractionSensor* InteractionSensor = OwningPawn->FindComponentByClass<UPlayerInteractionSensor>();
	if(InteractionSensor == nullptr)
	{UE_LOG(LogBuildingSystem,Error,TEXT("Building System Component pawn does not have player interaction system component"))}

	PlayerInteractionSensor = InteractionSensor;
	bReferencesSet = true;

}

void UBuildingPieceSpawner::SetBindings()
{
	if(bReferencesSet == false)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("Building Piece Spawner component cannot set bindings until references are set"))
		return;
	}

	CustomPlayerController->OnLMBPressed.AddDynamic(this,&UBuildingPieceSpawner::FinalizePlacement);
	CustomPlayerController->OnRMBPressed.AddDynamic(this,&UBuildingPieceSpawner::EndSpawnLoop);
}

void UBuildingPieceSpawner::ClearBindings()
{
	if(bReferencesSet == false)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("Building Piece Spawner component cannot clear bindings until references are set"))
		return;
	}

	CustomPlayerController->OnLMBPressed.RemoveDynamic(this,&UBuildingPieceSpawner::FinalizePlacement);
	CustomPlayerController->OnRMBPressed.RemoveDynamic(this,&UBuildingPieceSpawner::EndSpawnLoop);
}

void UBuildingPieceSpawner::StartSpawnLoop()
{
	SpawnGhostPiece(GhostPiece);
	GhostPiece->OnPlacementStart();
	PlayerInteractionSensor->ToggleInteraction(false);
	bIsPlacingBuildingPiece = true;
	SetBindings();

	GetWorld()->GetTimerManager().SetTimer(SpawnLoopTimer, this, &UBuildingPieceSpawner::SpawnLoop, SpawnLoopRate, true,
	                                       .1f);
}

void UBuildingPieceSpawner::SpawnGhostPiece(ABuildingPiece*& OutBuildingPiece) const
{
	if(BuildingPieceClass == nullptr) {return;}
	
	const FVector Location = PlayerInteractionSensor->GetLookLocation();
	const FRotator Rotation(0.f,0.f,0.f);
	const FActorSpawnParameters SpawnParameters;

	OutBuildingPiece = GetWorld()->SpawnActor<ABuildingPiece>(BuildingPieceClass,Location,Rotation,SpawnParameters);
	if(OutBuildingPiece == nullptr)
	{
		UE_LOG(LogBuildingSystem,Error,TEXT("%s could not spawn ghost mesh for %s building piece class"),
			*GetOwner()->GetName(),*BuildingPieceClass->GetName())
	}

	//Set all mesh components to ignore visibility so it doesn't show up in trace
	TArray<UMeshComponent*> MeshComponents;
	OutBuildingPiece->GetComponents<UMeshComponent>(MeshComponents);
	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		MeshComponents[i]->SetCollisionObjectType(ECC_WorldDynamic);
		MeshComponents[i]->SetCollisionResponseToChannels(ECR_Overlap);
		MeshComponents[i]->SetCollisionResponseToChannel(ECC_Visibility,ECR_Ignore);
	}

	SetMaterial(OutBuildingPiece,GoodPlacementMaterial);

	//Remove Snap Points
	TArray<UBuildingPieceSnapPoint*> SnapPoints;
	OutBuildingPiece->GetComponents<UBuildingPieceSnapPoint>(SnapPoints);
	for (int i = SnapPoints.Num() - 1; i >= 0; --i)
	{
		SnapPoints[i]->DestroyComponent();
	}
	

	OutBuildingPiece->SetOwner(GetOwner());
	
}

void UBuildingPieceSpawner::SpawnLoop()
{
	if(GhostPiece == nullptr)
	{
		SpawnLoopTimer.Invalidate();
		return;
	}

	const FVector Location = PlayerInteractionSensor->GetLookLocation();
	GhostPiece->SetActorLocation(Location);
	if(GhostPiece->GetShouldCheckForSnaps())
	{
		FTransform Transform;
		if(SnapPointFound(Transform,Location,GhostPiece))
		{
			GhostPiece->SetActorTransform(Transform);
		}
	}

	bIsOKToPlace = GhostPiece->CheckPlacement();
	SetMaterial(GhostPiece,bIsOKToPlace ? GoodPlacementMaterial : BadPlacementMaterial);

}

bool UBuildingPieceSpawner::SnapPointFound(FTransform& OutTransform, UBuildingPieceSnapPoint*& OutSnapPoint,
                                           const FVector ViewLocation, const ABuildingPiece* BuildingPiece) const
{
	if(BuildingPiece == nullptr){return false;}

	//Get Mesh Components
	TArray<UMeshComponent*> MeshComponents;
	BuildingPiece->GetComponents<UMeshComponent>(MeshComponents);
	if(MeshComponents.Num() == 0 ){return false;}

	TArray<UBuildingPieceSnapPoint*> BuildingPieceSnapPoints;
	BuildingPiece->GetComponents<UBuildingPieceSnapPoint>(BuildingPieceSnapPoints);
	
	//Get Overlapping Snap Points
	TArray<UBuildingPieceSnapPoint*> SnapPoints;
	
	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		TArray<UPrimitiveComponent*> Components;
		MeshComponents[i]->GetOverlappingComponents(Components);

		for (int c = 0; c < Components.Num(); ++c)
		{
			if(UBuildingPieceSnapPoint* SnapPoint = Cast<UBuildingPieceSnapPoint>(Components[c]))
			{
				if(SnapPoint->IsEligibleForSnap(BuildingPiece->GetClass()))
				{
					
					//Make sure it's not one of the building pieces snap points
					if(BuildingPieceSnapPoints.Contains(SnapPoint) == false)
					{
						SnapPoints.Add(SnapPoint);
					}
					
				}
			}
		}
	}
	
	//If none, return false
	if(SnapPoints.Num() == 0) {return false;}
	
	//if true, see which one is closest
	TArray<float> SnapPointDistances;
	for (int i = 0; i < SnapPoints.Num(); ++i)
	{
		float Dist = FVector::Dist(ViewLocation,SnapPoints[i]->GetComponentLocation());
		SnapPointDistances.Add(Dist);
	}

	//return true with transform
	int32 MinIndex;
	FMath::Min<float>(SnapPointDistances,&MinIndex);
	OutTransform = SnapPoints[MinIndex]->GetComponentTransform();
	OutSnapPoint = SnapPoints[MinIndex];
	return true;
}

bool UBuildingPieceSpawner::SnapPointFound(FTransform& OutTransform, const FVector ViewLocation, const ABuildingPiece* BuildingPiece) const
{
	UBuildingPieceSnapPoint* SnapPoint;
	const bool bSnapPointFound = SnapPointFound(OutTransform,SnapPoint,ViewLocation,BuildingPiece);
	return bSnapPointFound;
	
}


void UBuildingPieceSpawner::FinalizePlacement()
{
	if(bIsOKToPlace == false){return;}
	
	const FTransform Transform = GhostPiece->GetTransform();
	const TSubclassOf<ABuildingPiece> Class = GhostPiece->GetClass();
	EndSpawnLoop();
	Server_SpawnBuildingPiece(Class,Transform);
	
}

void UBuildingPieceSpawner::EndSpawnLoop()
{
	GhostPiece->Destroy();
	GhostPiece = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(SpawnLoopTimer);
	bIsPlacingBuildingPiece = false;
	BuildingPieceClass = nullptr;
	PlayerInteractionSensor->ToggleInteraction(true);
	ClearBindings();
}

void UBuildingPieceSpawner::SetMaterial(const AActor* Actor, UMaterialInterface* NewMaterial)
{
	if(Actor == nullptr || NewMaterial == nullptr) {return;}
	
	TArray<UMeshComponent*> MeshComponents;
	Actor->GetComponents<UMeshComponent>(MeshComponents);
	for (int i = 0; i < MeshComponents.Num(); ++i)
	{
		const int32 NumMaterials = MeshComponents[i]->GetNumMaterials();

		for (int n = 0; n < NumMaterials; ++n)
		{
			MeshComponents[i]->SetMaterial(n,NewMaterial);
		}
	}
}

bool UBuildingPieceSpawner::Server_SpawnBuildingPiece_Validate(TSubclassOf<ABuildingPiece> Class, FTransform Transform)
{
	return true;
}

void UBuildingPieceSpawner::Server_SpawnBuildingPiece_Implementation(const TSubclassOf<ABuildingPiece> Class,
                                                                     const FTransform Transform)
{
	const FActorSpawnParameters SpawnParameters;

	ABuildingPiece* BuildingPiece = GetWorld()->SpawnActor<ABuildingPiece>(Class, Transform, SpawnParameters);
	bool bPieceSnapped = false;
	
	if(BuildingPiece->GetShouldCheckForSnaps())
	{
		UBuildingPieceSnapPoint* SnapPoint;
		FTransform SnapTransform;;
		if(SnapPointFound(SnapTransform,SnapPoint,Transform.GetLocation(),BuildingPiece))
		{
			SnapPoint->AddSnappedPiece(BuildingPiece);
			bPieceSnapped = true;
		}
	}

	BuildingPiece->OnPlaced(bPieceSnapped);

	UE_LOG(LogBuildingSystem,Log,TEXT("%s placed %s building piece"),
		*GetOwner()->GetName(),*BuildingPiece->GetName())
	
}


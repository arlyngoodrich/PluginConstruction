// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFoliageISMC.h"



void UCustomFoliageISMC::GetInstancesInRange(const FVector Center, const float Range, TArray<int32>& Instances) const
{
	
	for (int i = 0; i < GetInstanceCount(); ++i)
	{
		FTransform Transform;
		if(GetInstanceTransform(i,Transform,true))
		{	
			const float Distance = FVector::Dist(Center,Transform.GetLocation());
			if(Distance <= Range)
			{
				Instances.Add(i);
			}
		}
	}
}

void UCustomFoliageISMC::GetInstancesInRangeOfLocations(TArray<FVector> Locations, const float Range,
	TArray<int32>& Instances) const
{
	for (int i = 0; i < GetInstanceCount(); ++i)
	{
		FTransform Transform;
		if(GetInstanceTransform(i,Transform,true))
		{
			//Cycle through provided locations to see if any instances are in range
			TArray<bool> RangeChecks;
			for (int l = 0; l < Locations.Num(); ++l)
			{
				const float Distance = FVector::Dist(Locations[l],Transform.GetLocation());

				//Add true if in range and false if not
				RangeChecks.Add(Distance<=Range);
			}

			//See if any are true, if yes then add 
			if(RangeChecks.Contains(true))
			{
				Instances.Add(i);
			}
		}
	}
}

bool UCustomFoliageISMC::RemoveInstanceAtLocation(const FVector Location)
{
	
	for (int i = 0; i < GetInstanceCount(); ++i)
	{
		FTransform Transform;
		if(GetInstanceTransform(i,Transform,true))
		{
			const float Distance = FVector::Dist(Transform.GetLocation(),Location);
			if(Distance<=1)
			{

				FTransform NullTransform;
				NullTransform.SetLocation(FVector(0,0,0));
				NullTransform.SetScale3D(FVector(0,0,0));
				UpdateInstanceTransform(i,NullTransform,true);
				RemoveInstance(i);

				return true;
			}
		}
	}


	return false;
}

bool UCustomFoliageISMC::GetInstanceIndexFromLocation(const FVector Location, int32& OutIndex) const
{
	for (int i = 0; i < GetInstanceCount(); ++i)
	{
		FTransform Transform;
		if(GetInstanceTransform(i,Transform,true))
		{
			const float Distance = FVector::Dist(Transform.GetLocation(),Location);
			if(Distance<=1)
			{
				OutIndex = i;
				return true;
			}
		}
	}


	return false;
}

void UCustomFoliageISMC::AddInstance_Safe(FTransform Transform)
{
	StartSafeRemoveTimer();
	AddInstance(Transform,true);	
}

void UCustomFoliageISMC::RemoveInstance_Safe(int32 Index)
{
	FTransform NullTransform;
	NullTransform.SetLocation(FVector(0,0,-5000));
	NullTransform.SetScale3D(FVector(0,0,0));

	UpdateInstanceTransform(Index,NullTransform,true);
	InstancesPendingRemove.Add(Index);
	StartSafeRemoveTimer();
}

void UCustomFoliageISMC::StartSafeRemoveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SafeRemoveTimerHandle,this,&UCustomFoliageISMC::RemovePendingInstances,PendingRemoveTime);
}

void UCustomFoliageISMC::RemovePendingInstances()
{
	if(InstancesPendingRemove.Num()>0)
	{
		RemoveInstances(InstancesPendingRemove);
		InstancesPendingRemove.Empty();
	}
}

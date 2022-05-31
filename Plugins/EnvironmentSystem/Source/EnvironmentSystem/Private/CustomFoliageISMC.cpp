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

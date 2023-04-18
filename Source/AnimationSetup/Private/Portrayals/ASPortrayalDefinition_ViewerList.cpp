// Fill out your copyright notice in the Description page of Project Settings.


#include "Portrayals/ASPortrayalDefinition_ViewerList.h"

#include "Interfaces/ASViewActorFilterInterface.h"
#include "Subobjects/ASObject_ViewActorFilter.h"



UASPortrayalDefinition_ViewerList::UASPortrayalDefinition_ViewerList()
{
	bInstanced = true;
}

void UASPortrayalDefinition_ViewerList::ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		for (const TWeakObjectPtr<AActor>& Actor : ActorList)
		{
			ViewActorFilterInterface->GetViewActorFilter()->AddToViewActorWhitelist(Actor.Get());
		}
	}
}

void UASPortrayalDefinition_ViewerList::UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		for (const TWeakObjectPtr<AActor>& Actor : ActorList)
		{
			ViewActorFilterInterface->GetViewActorFilter()->RemoveFromViewActorWhitelist(Actor.Get());
		}
	}
}

void UASPortrayalDefinition_ViewerList::ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		for (const TWeakObjectPtr<AActor>& Actor : ActorList)
		{
			ViewActorFilterInterface->GetViewActorFilter()->AddToViewActorBlacklist(Actor.Get());
		}
	}
}

void UASPortrayalDefinition_ViewerList::UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		for (const TWeakObjectPtr<AActor>& Actor : ActorList)
		{
			ViewActorFilterInterface->GetViewActorFilter()->RemoveFromViewActorBlacklist(Actor.Get());
		}
	}
}

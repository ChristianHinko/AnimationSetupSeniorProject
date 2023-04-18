// Fill out your copyright notice in the Description page of Project Settings.


#include "Portrayals/ASPortrayalDefinition_SelfViewer.h"

#include "Interfaces/ASViewActorFilterInterface.h"
#include "Subobjects/ASObject_ViewActorFilter.h"



void UASPortrayalDefinition_SelfViewer::ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		ViewActorFilterInterface->GetViewActorFilter()->AddToViewActorWhitelist(InActorComponent->GetOwner());
	}
}

void UASPortrayalDefinition_SelfViewer::UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		ViewActorFilterInterface->GetViewActorFilter()->RemoveFromViewActorWhitelist(InActorComponent->GetOwner());
	}
}

void UASPortrayalDefinition_SelfViewer::ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		ViewActorFilterInterface->GetViewActorFilter()->AddToViewActorBlacklist(InActorComponent->GetOwner());
	}
}

void UASPortrayalDefinition_SelfViewer::UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
	if (IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InActorComponent))
	{
		ViewActorFilterInterface->GetViewActorFilter()->RemoveFromViewActorBlacklist(InActorComponent->GetOwner());
	}
}

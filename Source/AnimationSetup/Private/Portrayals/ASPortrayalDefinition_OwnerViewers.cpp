// Fill out your copyright notice in the Description page of Project Settings.


#include "Portrayals/ASPortrayalDefinition_OwnerViewers.h"

#include "Components/PrimitiveComponent.h"



void UASPortrayalDefinition_OwnerViewers::ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InActorComponent);
	if (IsValid(PrimitiveComponent))
	{
		PrimitiveComponent->SetOnlyOwnerSee(true);
	}
}

void UASPortrayalDefinition_OwnerViewers::UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InActorComponent);
	if (IsValid(PrimitiveComponent))
	{
		PrimitiveComponent->SetOnlyOwnerSee(false);
	}
}

void UASPortrayalDefinition_OwnerViewers::ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InActorComponent);
	if (IsValid(PrimitiveComponent))
	{
		PrimitiveComponent->SetOwnerNoSee(true);
	}
}

void UASPortrayalDefinition_OwnerViewers::UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(InActorComponent);
	if (IsValid(PrimitiveComponent))
	{
		PrimitiveComponent->SetOwnerNoSee(false);
	}
}

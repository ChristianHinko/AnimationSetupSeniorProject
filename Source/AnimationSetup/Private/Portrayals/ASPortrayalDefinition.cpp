// Fill out your copyright notice in the Description page of Project Settings.


#include "Portrayals/ASPortrayalDefinition.h"



UASPortrayalDefinition::UASPortrayalDefinition()
	: bInstanced(false)
{
}

void UASPortrayalDefinition::ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const
{
}

void UASPortrayalDefinition::UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const
{
}

void UASPortrayalDefinition::ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
}

void UASPortrayalDefinition::UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const
{
}

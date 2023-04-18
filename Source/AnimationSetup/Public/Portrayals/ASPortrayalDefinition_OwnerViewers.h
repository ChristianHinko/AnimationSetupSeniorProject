// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portrayals/ASPortrayalDefinition.h"

#include "ASPortrayalDefinition_OwnerViewers.generated.h"



/**
 * Portrayal that can be used for showing components only to their tree of owner actors.
 */
UCLASS()
class ANIMATIONSETUP_API UASPortrayalDefinition_OwnerViewers : public UASPortrayalDefinition
{
	GENERATED_BODY()

public:
	virtual void ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const override;
	virtual void UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const override;
	virtual void ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const override;
	virtual void UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const override;
};

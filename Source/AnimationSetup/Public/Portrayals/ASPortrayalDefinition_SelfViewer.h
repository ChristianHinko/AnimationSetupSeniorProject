// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portrayals/ASPortrayalDefinition.h"

#include "ASPortrayalDefinition_SelfViewer.generated.h"



/**
 * Portrayal that can be used for showing components only to their actor.
 * 
 * NOTE: This won't update automatically if the component gets registered with a different actor during runtime. Maybe we could use actor component activate delegates or something if we need this?
 */
UCLASS()
class ANIMATIONSETUP_API UASPortrayalDefinition_SelfViewer : public UASPortrayalDefinition
{
	GENERATED_BODY()

public:
	virtual void ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const override;
	virtual void UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const override;
	virtual void ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const override;
	virtual void UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portrayals/ASPortrayalDefinition.h"

#include "ASPortrayalDefinition_ViewerList.generated.h"



/**
 * General-purpose instanced portrayal shown only to actors in the configurable list.
 */
UCLASS(Abstract)
class ANIMATIONSETUP_API UASPortrayalDefinition_ViewerList : public UASPortrayalDefinition
{
	GENERATED_BODY()

public:
	UASPortrayalDefinition_ViewerList();

	UPROPERTY()
		TArray<TWeakObjectPtr<AActor>> ActorList;

	virtual void ApplyToActorComponent_Implementation(UActorComponent* InActorComponent) const override;
	virtual void UnapplyFromActorComponent_Implementation(UActorComponent* InActorComponent) const override;
	virtual void ApplyToActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const override;
	virtual void UnapplyFromActorComponentOfDifferentPortrayal_Implementation(UActorComponent* InActorComponent) const override;
};

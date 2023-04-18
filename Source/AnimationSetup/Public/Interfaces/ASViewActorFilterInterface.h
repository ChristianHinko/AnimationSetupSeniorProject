// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ASViewActorFilterInterface.generated.h"


class UASObject_ViewActorFilter;



/**
 * 
 */
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UASViewActorFilterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implement this on your custom primitive component to be able to use
 * common ViewActorFilter features.
 */
class ANIMATIONSETUP_API IASViewActorFilterInterface
{
	GENERATED_BODY()

public:
	virtual UASObject_ViewActorFilter* GetViewActorFilter() const PURE_VIRTUAL(IASViewActorFilterInterface::GetViewActorFilter, return nullptr;)
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ASPrimaryDataAsset_PluginObjectReferenceCollection.generated.h"


struct FGameplayTag;
class UASPortrayalDefinition;



/**
 * This asset must be used as "Animation/PORC_PluginName" where "PluginName" is the name of your plugin.
 */
UCLASS(Const)
class ANIMATIONSETUP_API UASPrimaryDataAsset_PluginObjectReferenceCollection : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TMap<FGameplayTag, TSubclassOf<UASPortrayalDefinition>> PluginPortrayalDefinitionReferences;
};

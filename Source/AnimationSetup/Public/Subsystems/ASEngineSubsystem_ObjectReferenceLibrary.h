// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"

#include "ASEngineSubsystem_ObjectReferenceLibrary.generated.h"


class UASPortrayalDefinition;
class UASPrimaryDataAsset_PluginObjectReferenceCollection;
class IPlugin;



/**
 * Subsystem holding references to important objects and/or assets which can be retrieved via Gameplay Tag.
 */
UCLASS(Config="ASObjectReferenceLibrary")
class ANIMATIONSETUP_API UASEngineSubsystem_ObjectReferenceLibrary : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	/** Contributions from plugins to our object references. */
	UPROPERTY(VisibleDefaultsOnly)
		TSet<TObjectPtr<const UASPrimaryDataAsset_PluginObjectReferenceCollection>> PluginObjectReferenceCollections;


	/** Portrayal references from the game project added via config (project settings). */
	UPROPERTY(EditDefaultsOnly, Config, Category="PortrayalDefinition")
		TMap<FGameplayTag, TSoftClassPtr<UASPortrayalDefinition>> GameProjectPortrayalDefinitionReferences;

	/**
	 * Combined map of portrayal references from the game project and the plugins.
	 * 
	 * NOTE: These portrayal definition instances are CDOs.
	 */
	UPROPERTY(Transient)
		TMap<FGameplayTag, TObjectPtr<const UASPortrayalDefinition>> PortrayalDefinitionReferences;

public:
	UASEngineSubsystem_ObjectReferenceLibrary();

	//  BEGIN UObject interface
	virtual void PostInitProperties() override;
	//  END UObject interface

	//  BEGIN USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//  END USubsystem interface

	/**
	 * Get a portrayal definition via portrayal tag.
	 * 
	 * NOTE: These portrayal definitions are CDOs.
	 */
	UFUNCTION(BlueprintPure, Category="PortrayalDefinition")
		const UASPortrayalDefinition* GetPortrayalDefinition(const FGameplayTag& InPortrayalTag) const;

	const TMap<FGameplayTag, TObjectPtr<const UASPortrayalDefinition>>& GetPortrayals() const { return PortrayalDefinitionReferences; }

	static FSoftObjectPath GetPluginObjectReferenceCollectionAssetPath(const IPlugin& InPlugin);

protected:
	virtual void OnAssetManagerCreated();

	virtual void OnPluginAddContent(const IPlugin& InPlugin);
	virtual void OnPluginRemoveContent(const IPlugin& InPlugin);
};

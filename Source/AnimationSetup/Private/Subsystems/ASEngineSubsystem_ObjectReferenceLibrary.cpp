// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/ASEngineSubsystem_ObjectReferenceLibrary.h"

#include "Portrayals/ASPortrayalDefinition.h"
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif // WITH_EDITOR
#include "Subsystems/ASPrimaryDataAsset_PluginObjectReferenceCollection.h"
#include "Engine/AssetManager.h"
#include "BlueprintFunctionLibraries/GCBlueprintFunctionLibrary_ContentTools.h"
#include "Interfaces/IPluginManager.h"



UASEngineSubsystem_ObjectReferenceLibrary::UASEngineSubsystem_ObjectReferenceLibrary()
{
}


void UASEngineSubsystem_ObjectReferenceLibrary::PostInitProperties()
{
	Super::PostInitProperties();

	// Load the game project's configged references and add them
	for (const TPair<FGameplayTag, TSoftClassPtr<const UASPortrayalDefinition>>& TagPortrayalPair : GameProjectPortrayalDefinitionReferences)
	{
		TSubclassOf<UASPortrayalDefinition> PortrayalDefinitionClass = TagPortrayalPair.Value.LoadSynchronous();
		PortrayalDefinitionReferences.Add(TPair<FGameplayTag, TObjectPtr<const UASPortrayalDefinition>>(TagPortrayalPair.Key, PortrayalDefinitionClass.GetDefaultObject()));
	}

	check(PluginObjectReferenceCollections.Num() <= 0)
}

void UASEngineSubsystem_ObjectReferenceLibrary::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

#if WITH_EDITOR
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"));
	SettingsModule->RegisterSettings(
		FName(TEXT("Project")),
		FName(TEXT("Plugins")),
		FName(TEXT("AnimationSetup")),
		FText::FromString(TEXT("Animation Setup")),
		FText::FromString(TEXT("Object Reference Library")),
		this
	);
#endif // WITH_EDITOR

	UAssetManager::CallOrRegister_OnAssetManagerCreated(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAssetManagerCreated));
}
void UASEngineSubsystem_ObjectReferenceLibrary::Deinitialize()
{
	Super::Deinitialize();

#if WITH_EDITOR
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings"));
	SettingsModule->UnregisterSettings(
		FName(TEXT("Project")),
		FName(TEXT("Plugins")),
		FName(TEXT("AnimationSetup"))
	);
#endif // WITH_EDITOR
}

const UASPortrayalDefinition* UASEngineSubsystem_ObjectReferenceLibrary::GetPortrayalDefinition(const FGameplayTag& InPortrayalTag) const
{
	if (const TObjectPtr<const UASPortrayalDefinition>* FoundPortrayalDefinition = PortrayalDefinitionReferences.Find(InPortrayalTag))
	{
		return FoundPortrayalDefinition->Get();
	}

	return nullptr;
}

void UASEngineSubsystem_ObjectReferenceLibrary::OnAssetManagerCreated()
{
	UGCBlueprintFunctionLibrary_ContentTools::UseContentFromDependentPlugins(UE_PLUGIN_NAME,
		TDelegate<void(const IPlugin&)>::CreateUObject(this, &ThisClass::OnPluginAddContent),
		TDelegate<void(const IPlugin&)>::CreateUObject(this, &ThisClass::OnPluginRemoveContent)
	);
}

FSoftObjectPath UASEngineSubsystem_ObjectReferenceLibrary::GetPluginObjectReferenceCollectionAssetPath(const IPlugin& InPlugin)
{
	const FString AssetName = (TEXT("PORC_") + InPlugin.GetName());
	return FSoftObjectPath(InPlugin.GetMountedAssetPath() / TEXT("Animation") / AssetName + TEXT('.') + AssetName);
}

void UASEngineSubsystem_ObjectReferenceLibrary::OnPluginAddContent(const IPlugin& InPlugin)
{
	const FSoftObjectPath& PluginObjectReferenceCollectionAssetPath = GetPluginObjectReferenceCollectionAssetPath(InPlugin);
	const UObject* LoadedAsset = UAssetManager::GetIfValid()->GetStreamableManager().LoadSynchronous(PluginObjectReferenceCollectionAssetPath);
	const UASPrimaryDataAsset_PluginObjectReferenceCollection* PluginObjectReferenceCollection = Cast<UASPrimaryDataAsset_PluginObjectReferenceCollection>(LoadedAsset);

	if (!IsValid(PluginObjectReferenceCollection))
	{
		return;
	}

	if (PluginObjectReferenceCollections.Contains(PluginObjectReferenceCollection))
	{
		UE_LOG(LogASObjectReferenceLibrarySubsystem, Error, TEXT("%s() PluginObjectReferenceCollection [%s] has already been added."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginObjectReferenceCollection->GetName()));
		check(0);
		return;
	}

	// Make sure the data asset doesn't contain any already-added portrayal definition references
	for (const TPair<FGameplayTag, TSubclassOf<UASPortrayalDefinition>>& PluginPortrayalDefinitionReference : PluginObjectReferenceCollection->PluginPortrayalDefinitionReferences)
	{
		if (PortrayalDefinitionReferences.Contains(PluginPortrayalDefinitionReference.Key))
		{
			UE_LOG(LogASObjectReferenceLibrarySubsystem, Error, TEXT("%s() Tried adding PluginObjectReferenceCollection but it contains a Gameplay Tag that is already being used. Aborting. Culprit tag [%s] from data asset [%s]."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginPortrayalDefinitionReference.Key.ToString()), *(PluginObjectReferenceCollection->GetName()));
			check(0);
			return;
		}
	}

	// Add the data asset and all of its portrayal definition references
	UE_LOG(LogASObjectReferenceLibrarySubsystem, Log, TEXT("%s() Adding PluginObjectReferenceCollection [%s]."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginObjectReferenceCollection->GetName()));

	PluginObjectReferenceCollections.Add(PluginObjectReferenceCollection);
	for (const TPair<FGameplayTag, TSubclassOf<UASPortrayalDefinition>>& PluginPortrayalDefinitionReference : PluginObjectReferenceCollection->PluginPortrayalDefinitionReferences)
	{
		PortrayalDefinitionReferences.Add(TPair<FGameplayTag, TObjectPtr<const UASPortrayalDefinition>>(PluginPortrayalDefinitionReference.Key, PluginPortrayalDefinitionReference.Value.GetDefaultObject()));
		UE_LOG(LogASObjectReferenceLibrarySubsystem, Log, TEXT("%s() New plugin portrayal definition reference [%s] added by data asset [%s]."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginPortrayalDefinitionReference.Key.ToString()), *(PluginObjectReferenceCollection->GetName()));
	}
}

void UASEngineSubsystem_ObjectReferenceLibrary::OnPluginRemoveContent(const IPlugin& InPlugin)
{
	const FSoftObjectPath& PluginObjectReferenceCollectionAssetPath = GetPluginObjectReferenceCollectionAssetPath(InPlugin);
	const UObject* LoadedAsset = UAssetManager::GetIfValid()->GetStreamableManager().LoadSynchronous(PluginObjectReferenceCollectionAssetPath);
	const UASPrimaryDataAsset_PluginObjectReferenceCollection* PluginObjectReferenceCollection = Cast<UASPrimaryDataAsset_PluginObjectReferenceCollection>(LoadedAsset);

	if (!IsValid(PluginObjectReferenceCollection))
	{
		return;
	}

	if (PluginObjectReferenceCollections.Contains(PluginObjectReferenceCollection) == false)
	{
		UE_LOG(LogASObjectReferenceLibrarySubsystem, Error, TEXT("%s() PluginObjectReferenceCollection [%s] does not exist in the set. Nothing to remove."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginObjectReferenceCollection->GetName()));
		check(0);
		return;
	}

	// Remove the data asset and all of its portrayal definition references
	UE_LOG(LogASObjectReferenceLibrarySubsystem, Log, TEXT("%s() Removing PluginObjectReferenceCollection [%s]."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginObjectReferenceCollection->GetName()));

	PluginObjectReferenceCollections.Remove(PluginObjectReferenceCollection);
	for (const TPair<FGameplayTag, TSubclassOf<UASPortrayalDefinition>>& PluginPortrayalDefinitionReference : PluginObjectReferenceCollection->PluginPortrayalDefinitionReferences)
	{
		PortrayalDefinitionReferences.Remove(PluginPortrayalDefinitionReference.Key);
		UE_LOG(LogASObjectReferenceLibrarySubsystem, Log, TEXT("%s() Plugin portrayal definition reference [%s] removed at by data asset [%s]."), ANSI_TO_TCHAR(__FUNCTION__), *(PluginPortrayalDefinitionReference.Key.ToString()), *(PluginObjectReferenceCollection->GetName()));
	}
}

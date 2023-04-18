// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ASActorComponent_PortrayalAssignment.h"

#include "Subsystems/ASEngineSubsystem_ObjectReferenceLibrary.h"
#include "Portrayals/ASPortrayalDefinition.h"



UASActorComponent_PortrayalAssignment::UASActorComponent_PortrayalAssignment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UASActorComponent_PortrayalAssignment::ApplyPortrayals()
{
	const UASEngineSubsystem_ObjectReferenceLibrary* ASObjectReferenceLibrary = GEngine->GetEngineSubsystem<UASEngineSubsystem_ObjectReferenceLibrary>();
	if (!IsValid(ASObjectReferenceLibrary))
	{
		return;
	}

	// Apply portrayals for each portrayal group
	for (FASActorComponentDepiction& ActorComponentDepiction : ActorComponentDepictions)
	{
		// Apply each portrayal for themselves and to each other.
		// NOTE: O(n) = n^2. Where n = PortrayedActorComponents.Num().
		for (FASPortrayedActorComponent& ActorComponentPortrayal : ActorComponentDepiction.PortrayedActorComponents)
		{
			UActorComponent* Component = ActorComponentPortrayal.ActorComponent.GetComponent(GetOwner());

			const UASPortrayalDefinition* PortrayalDefinition = GetInstancedPortrayalDefinition(ActorComponentPortrayal.PortrayalTag);
			if (!IsValid(PortrayalDefinition))
			{
				PortrayalDefinition = ASObjectReferenceLibrary->GetPortrayalDefinition(ActorComponentPortrayal.PortrayalTag);
			}

			if (LIKELY(IsValid(PortrayalDefinition)))
			{
				// Apply our portrayal definition to our component
				PortrayalDefinition->ApplyToActorComponent(Component);

				// Apply our portrayal definition to other components in this group
				for (const FASPortrayedActorComponent& OtherComponentPortrayal : ActorComponentDepiction.PortrayedActorComponents)
				{
					UActorComponent* OtherComponent = OtherComponentPortrayal.ActorComponent.GetComponent(GetOwner());
					if (OtherComponent == Component)
					{
						continue;
					}

					PortrayalDefinition->ApplyToActorComponentOfDifferentPortrayal(OtherComponent);
				}
			}
		}
	}
}

void UASActorComponent_PortrayalAssignment::UnapplyPortrayals()
{
	const UASEngineSubsystem_ObjectReferenceLibrary* ASObjectReferenceLibrary = GEngine->GetEngineSubsystem<UASEngineSubsystem_ObjectReferenceLibrary>();
	if (!IsValid(ASObjectReferenceLibrary))
	{
		return;
	}

	for (FASActorComponentDepiction& ActorComponentDepiction : ActorComponentDepictions)
	{
		for (FASPortrayedActorComponent& ActorComponentPortrayal : ActorComponentDepiction.PortrayedActorComponents)
		{
			UActorComponent* Component = ActorComponentPortrayal.ActorComponent.GetComponent(GetOwner());

			const UASPortrayalDefinition* PortrayalDefinition = GetInstancedPortrayalDefinition(ActorComponentPortrayal.PortrayalTag);
			if (!IsValid(PortrayalDefinition))
			{
				PortrayalDefinition = ASObjectReferenceLibrary->GetPortrayalDefinition(ActorComponentPortrayal.PortrayalTag);
			}

			if (LIKELY(IsValid(PortrayalDefinition)))
			{
				PortrayalDefinition->UnapplyFromActorComponent(Component);

				for (const FASPortrayedActorComponent& OtherComponentPortrayal : ActorComponentDepiction.PortrayedActorComponents)
				{
					UActorComponent* OtherComponent = OtherComponentPortrayal.ActorComponent.GetComponent(GetOwner());
					if (OtherComponent == Component)
					{
						continue;
					}

					PortrayalDefinition->UnapplyFromActorComponentOfDifferentPortrayal(OtherComponent);
				}
			}
		}
	}
}

UASPortrayalDefinition* UASActorComponent_PortrayalAssignment::GetInstancedPortrayalDefinition(const FGameplayTag& InPortrayalTag) const
{
	if (const TObjectPtr<UASPortrayalDefinition>* InstancedPortrayalDefinition = InstancedPortrayalDefinitions.Find(InPortrayalTag))
	{
		return *InstancedPortrayalDefinition;
	}

	return nullptr;
}

void UASActorComponent_PortrayalAssignment::OnRegister()
{
	Super::OnRegister();

	CreateInstancedPortrayalDefinitions();
	ApplyPortrayals();
}

void UASActorComponent_PortrayalAssignment::OnUnregister()
{
	Super::OnUnregister();

	UnapplyPortrayals();
	DestoryInstancedPortrayalDefinitions();
}

void UASActorComponent_PortrayalAssignment::CreateInstancedPortrayalDefinitions()
{
	const UASEngineSubsystem_ObjectReferenceLibrary* ASObjectReferenceLibrary = GEngine->GetEngineSubsystem<UASEngineSubsystem_ObjectReferenceLibrary>();
	if (!IsValid(ASObjectReferenceLibrary))
	{
		return;
	}

	for (FASActorComponentDepiction& ActorComponentDepiction : ActorComponentDepictions)
	{
		for (FASPortrayedActorComponent& ActorComponentPortrayal : ActorComponentDepiction.PortrayedActorComponents)
		{
			const UASPortrayalDefinition* PortrayalDefinition = ASObjectReferenceLibrary->GetPortrayalDefinition(ActorComponentPortrayal.PortrayalTag);
			if (LIKELY(IsValid(PortrayalDefinition)))
			{
				if (PortrayalDefinition->bInstanced)
				{
					if (InstancedPortrayalDefinitions.Contains(ActorComponentPortrayal.PortrayalTag) == false)
					{
						UASPortrayalDefinition* NewPortrayalDefinitionInstance = NewObject<UASPortrayalDefinition>(GetOwner(), PortrayalDefinition->GetClass());
						InstancedPortrayalDefinitions.Emplace(ActorComponentPortrayal.PortrayalTag, NewPortrayalDefinitionInstance);
					}
				}
			}
		}
	}
}

void UASActorComponent_PortrayalAssignment::DestoryInstancedPortrayalDefinitions()
{
	for (const TPair<FGameplayTag, TObjectPtr<UASPortrayalDefinition>>& InstancedPortrayalDefinition : InstancedPortrayalDefinitions)
	{
		if (IsValid(InstancedPortrayalDefinition.Value))
		{
			InstancedPortrayalDefinition.Value->MarkAsGarbage();
		}
	}

	InstancedPortrayalDefinitions.Empty();
}

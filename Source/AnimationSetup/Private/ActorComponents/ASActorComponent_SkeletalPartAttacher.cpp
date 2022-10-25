// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ASActorComponent_SkeletalPartAttacher.h"

#include "BlueprintFunctionLibraries/ASBlueprintFunctionLibrary_SkeletalMeshHelpers.h"
#include "Types/ASSkeletalPartDefinition.h"



UASActorComponent_SkeletalPartAttacher::UASActorComponent_SkeletalPartAttacher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UASActorComponent_SkeletalPartAttacher::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Initialize the transient SkinlessSkeletalMesh property (done on the CDO)
		CreateSkinlessSkeletalMesh();
	}
}
void UASActorComponent_SkeletalPartAttacher::PostLoad()
{
	Super::PostLoad();

	// Initialize the transient SkinlessSkeletalMesh property (done on the blueprint CDO)
	CreateSkinlessSkeletalMesh();
}

#if WITH_EDITOR
void UASActorComponent_SkeletalPartAttacher::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, Skeleton))
	{
		CreateSkinlessSkeletalMesh();
	}
}
#endif // WITH_EDITOR


void UASActorComponent_SkeletalPartAttacher::UseSkeletalMeshComponent(USkinnedMeshComponent* InSkeletalMeshComponent)
{
	if (!IsValid(InSkeletalMeshComponent))
	{
		UE_LOG(LogASSkeletalPartAttacherComponent, Error, TEXT("%s() the given InSkeletalMeshComponent was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	SkeletalMeshComponent = InSkeletalMeshComponent;

	// Always animate so that our attached part Actors can use our pose
	SkeletalMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void UASActorComponent_SkeletalPartAttacher::CreateSkinlessSkeletalMesh()
{
	// This is intended to be created on the CDO and then get copied over to new instances
	SkinlessSkeletalMesh = UASBlueprintFunctionLibrary_SkeletalMeshHelpers::CreateSkinlessSkeletalMeshFromSkeleton(Skeleton);

	if (SkeletalMeshComponent.IsValid())
	{
		SkeletalMeshComponent->SetSkeletalMesh(SkinlessSkeletalMesh);
	}
}

void UASActorComponent_SkeletalPartAttacher::SpawnSkeletalPartActors()
{
	// Spawn the part Actors
	for (const FASSkeletalPartDefinition& SkeletalPartDefinition : SkeletalPartDefinitions)
	{
		AActor* PartActor = SkeletalPartDefinition.SpawnAndAttachPartActor(GetOwner(), SkeletalMeshComponent.Get());
		SkeletalPartActors.Add(PartActor);
	}
}

void UASActorComponent_SkeletalPartAttacher::OnUnregister()
{
	Super::OnUnregister();

	// Destroy all part Actors
	for (TObjectPtr<AActor>& SkeletalPartActor : SkeletalPartActors)
	{
		if (IsValid(SkeletalPartActor))
		{
			const bool bDestroyed = SkeletalPartActor->Destroy();
			if (bDestroyed)
			{
				SkeletalPartActor = nullptr;
			}
			else
			{
				UE_LOG(LogASSkeletalPartAttacherComponent, Warning, TEXT("%s() SkeletalPartActor [%s] was not destroyed"), ANSI_TO_TCHAR(__FUNCTION__), *(SkeletalPartActor->GetName()));
			}
		}
	}

	SkeletalPartActors.Empty();
}

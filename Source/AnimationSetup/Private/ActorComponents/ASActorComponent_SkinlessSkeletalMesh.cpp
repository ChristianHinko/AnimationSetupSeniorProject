// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ASActorComponent_SkinlessSkeletalMesh.h"

#include "BlueprintFunctionLibraries/ASBlueprintFunctionLibrary_SkeletalMeshHelpers.h"



UASActorComponent_SkinlessSkeletalMesh::UASActorComponent_SkinlessSkeletalMesh(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bAddToRendererScene(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UASActorComponent_SkinlessSkeletalMesh::PostInitProperties()
{
	Super::PostInitProperties();

	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		// Initialize the transient SkinlessSkeletalMesh property (done on the CDO)
		CreateSkinlessSkeletalMesh();
	}
}
void UASActorComponent_SkinlessSkeletalMesh::PostLoad()
{
	Super::PostLoad();

	// Initialize the transient SkinlessSkeletalMesh property (done on the blueprint CDO)
	CreateSkinlessSkeletalMesh();
}

#if WITH_EDITOR
void UASActorComponent_SkinlessSkeletalMesh::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);


	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, Skeleton))
	{
		CreateSkinlessSkeletalMesh();
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, bAddToRendererScene))
	{
		CreateSkinlessSkeletalMesh();
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, SkeletalMeshComponentReference))
	{
		USkeletalMeshComponent* const OldSkeletalMeshComponent = SkeletalMeshComponent.Get();
		if (IsValid(OldSkeletalMeshComponent))
		{
			// Revert the old Skeletal Mesh Component's properties that we changed back to defaults
			const USkeletalMeshComponent* OldSkeletalMeshComponentDefaultObject = OldSkeletalMeshComponent->GetClass()->GetDefaultObject<USkeletalMeshComponent>();
			OldSkeletalMeshComponent->SetSkeletalMesh(OldSkeletalMeshComponentDefaultObject->GetSkeletalMeshAsset());
			OldSkeletalMeshComponent->VisibilityBasedAnimTickOption = OldSkeletalMeshComponentDefaultObject->VisibilityBasedAnimTickOption;
			OldSkeletalMeshComponent->bHideSkin = OldSkeletalMeshComponentDefaultObject->bHideSkin;
		}

		CreateSkinlessSkeletalMesh();
	}
}
#endif // WITH_EDITOR

void UASActorComponent_SkinlessSkeletalMesh::CreateSkinlessSkeletalMesh()
{
	// Get the Skeletal Mesh Component to use
	SkeletalMeshComponent = Cast<USkeletalMeshComponent>(SkeletalMeshComponentReference.GetComponent(GetOwner()));

	// This is intended to be created on the CDO and then get copied over to new instances
	SkinlessSkeletalMesh = UASBlueprintFunctionLibrary_SkeletalMeshHelpers::CreateSkinlessSkeletalMeshFromSkeleton(Skeleton, bAddToRendererScene);

	if (SkeletalMeshComponent.IsValid())
	{
		// Use our skinless Skeletal Mesh
		SkeletalMeshComponent->SetSkeletalMesh(SkinlessSkeletalMesh);

		// Always animate so that our attached part Actors can use our pose
		// TODO: I want to find a way to avoid this but it seems that the only way to use complex logic for this would be to override functions from the SkinnedMeshComponent or maybe we could just keep setting USkinnedMeshComponent::VisibilityBasedAnimTickOption externally to achieve what we want?
		SkeletalMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

		// Hide skin if !bAddToRendererScene
		SkeletalMeshComponent->bHideSkin = !bAddToRendererScene;
	}
}

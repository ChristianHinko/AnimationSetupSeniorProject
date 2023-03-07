// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SkinnedMeshComponent.h"
#include "SkeletalRenderPublic.h"

#include "ASBlueprintFunctionLibrary_SkinnedMeshComponentHelpers.generated.h"



/**
 * Collection of helpful functions related to Skinned Mesh Components
 */
UCLASS()
class ANIMATIONSETUP_API UASBlueprintFunctionLibrary_SkinnedMeshComponentHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Version of USkinnedMeshComponent::CreateSceneProxy() that allows a custom FSkeletalMeshSceneProxy type.
	 * This is duplicate and modified engine code, and thus is effectively a forked portion of the engine.
	 */
	template <class TSkeletalMeshSceneProxyType>
	static FPrimitiveSceneProxy* SkinnedMeshComponentCreateSceneProxy(USkinnedMeshComponent* InSelfSkinnedMeshComponent);
};


template <class TSkeletalMeshSceneProxyType>
FPrimitiveSceneProxy* UASBlueprintFunctionLibrary_SkinnedMeshComponentHelpers::SkinnedMeshComponentCreateSceneProxy(USkinnedMeshComponent* InSelfSkinnedMeshComponent)
{
	LLM_SCOPE(ELLMTag::SkeletalMesh);
	ERHIFeatureLevel::Type SceneFeatureLevel = InSelfSkinnedMeshComponent->GetWorld()->FeatureLevel;
	TSkeletalMeshSceneProxyType* Result = nullptr;
	FSkeletalMeshRenderData* SkelMeshRenderData = InSelfSkinnedMeshComponent->GetSkeletalMeshRenderData();

	// Only create a scene proxy for rendering if properly initialized
	if (SkelMeshRenderData &&
		SkelMeshRenderData->LODRenderData.IsValidIndex(InSelfSkinnedMeshComponent->GetPredictedLODLevel()) &&
		!InSelfSkinnedMeshComponent->bHideSkin &&
		InSelfSkinnedMeshComponent->MeshObject)
	{
		// Only create a scene proxy if the bone count being used is supported, or if we don't have a skeleton (this is the case with destructibles)
		int32 MinLODIndex = InSelfSkinnedMeshComponent->ComputeMinLOD();
		int32 MaxBonesPerChunk = SkelMeshRenderData->GetMaxBonesPerSection(MinLODIndex);
		int32 MaxSupportedNumBones = InSelfSkinnedMeshComponent->MeshObject->IsCPUSkinned() ? MAX_int32 : FGPUBaseSkinVertexFactory::GetMaxGPUSkinBones();
		if (MaxBonesPerChunk <= MaxSupportedNumBones)
		{
			Result = ::new TSkeletalMeshSceneProxyType(InSelfSkinnedMeshComponent, SkelMeshRenderData);
		}
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	InSelfSkinnedMeshComponent->SendRenderDebugPhysics(Result);
#endif

	return Result;
}

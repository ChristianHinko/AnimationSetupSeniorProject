// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SkinnedMeshComponent.h"
#include "SkeletalRenderPublic.h"
#include "NaniteSceneProxy.h"

#include "ASBlueprintFunctionLibrary_CreateSceneProxyTemplates.generated.h"



/**
 * Versions of UPrimitiveComponent::CreateSceneProxy() that allow for custom FPrimitiveSceneProxy types.
 * These implementations are duplicated and modified engine code, and thus are effectively forked portions of the engine that must be maintained.
 */
UCLASS()
class ANIMATIONSETUP_API UASBlueprintFunctionLibrary_CreateSceneProxyTemplates : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * SkinnedMeshComponent implementation.
	 */
	template <class TPrimitiveSceneProxyType>
	static FPrimitiveSceneProxy* SkinnedMeshComponentCreateSceneProxy(USkinnedMeshComponent* InSelfPrimitiveComponent);

	/**
	 * StaticMeshComponent implementation.
	 * NOTE: Requires caller to provide the protected ShouldCreateNaniteProxy() member function.
	 */
	template <class TPrimitiveSceneProxyType>
	static FPrimitiveSceneProxy* StaticMeshComponentCreateSceneProxy(UStaticMeshComponent* InSelfPrimitiveComponent, bool (UStaticMeshComponent::* InShouldCreateNaniteProxyMemberFunction)() const);
};


template <class TPrimitiveSceneProxyType>
FPrimitiveSceneProxy* UASBlueprintFunctionLibrary_CreateSceneProxyTemplates::SkinnedMeshComponentCreateSceneProxy(USkinnedMeshComponent* InSelfPrimitiveComponent)
{
	LLM_SCOPE(ELLMTag::SkeletalMesh);
	ERHIFeatureLevel::Type SceneFeatureLevel = InSelfPrimitiveComponent->GetWorld()->FeatureLevel;
	TPrimitiveSceneProxyType* Result = nullptr;
	FSkeletalMeshRenderData* SkelMeshRenderData = InSelfPrimitiveComponent->GetSkeletalMeshRenderData();

	// Only create a scene proxy for rendering if properly initialized
	if (SkelMeshRenderData &&
		SkelMeshRenderData->LODRenderData.IsValidIndex(InSelfPrimitiveComponent->GetPredictedLODLevel()) &&
		!InSelfPrimitiveComponent->bHideSkin &&
		InSelfPrimitiveComponent->MeshObject)
	{
		// Only create a scene proxy if the bone count being used is supported, or if we don't have a skeleton (this is the case with destructibles)
		int32 MinLODIndex = InSelfPrimitiveComponent->ComputeMinLOD();
		int32 MaxBonesPerChunk = SkelMeshRenderData->GetMaxBonesPerSection(MinLODIndex);
		int32 MaxSupportedNumBones = InSelfPrimitiveComponent->MeshObject->IsCPUSkinned() ? MAX_int32 : FGPUBaseSkinVertexFactory::GetMaxGPUSkinBones();
		if (MaxBonesPerChunk <= MaxSupportedNumBones)
		{
			Result = ::new TPrimitiveSceneProxyType(InSelfPrimitiveComponent, SkelMeshRenderData);
		}
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	InSelfPrimitiveComponent->SendRenderDebugPhysics(Result);
#endif

	return Result;
}

template <class TPrimitiveSceneProxyType>
FPrimitiveSceneProxy* UASBlueprintFunctionLibrary_CreateSceneProxyTemplates::StaticMeshComponentCreateSceneProxy(UStaticMeshComponent* InSelfPrimitiveComponent, bool (UStaticMeshComponent::* InShouldCreateNaniteProxyMemberFunction)() const)
{
	static const auto NaniteProxyRenderModeVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite.ProxyRenderMode"));
	const int32 NaniteProxyRenderMode = (NaniteProxyRenderModeVar != nullptr) ? (NaniteProxyRenderModeVar->GetInt() != 0) : 0;

	if (InSelfPrimitiveComponent->GetStaticMesh() == nullptr)
	{
		UE_LOG(LogStaticMesh, Verbose, TEXT("Skipping CreateSceneProxy for StaticMeshComponent %s (StaticMesh is null)"), *InSelfPrimitiveComponent->GetFullName());
		return nullptr;
	}

	// Prevent accessing the RenderData during async compilation. The RenderState will be recreated when compilation finishes.
	if (InSelfPrimitiveComponent->GetStaticMesh()->IsCompiling())
	{
		UE_LOG(LogStaticMesh, Verbose, TEXT("Skipping CreateSceneProxy for StaticMeshComponent %s (StaticMesh is not ready)"), *InSelfPrimitiveComponent->GetFullName());
		return nullptr;
	}

	if (InSelfPrimitiveComponent->GetStaticMesh()->GetRenderData() == nullptr)
	{
		UE_LOG(LogStaticMesh, Verbose, TEXT("Skipping CreateSceneProxy for StaticMeshComponent %s (RenderData is null)"), *InSelfPrimitiveComponent->GetFullName());
		return nullptr;
	}

	if (!InSelfPrimitiveComponent->GetStaticMesh()->GetRenderData()->IsInitialized())
	{
		UE_LOG(LogStaticMesh, Verbose, TEXT("Skipping CreateSceneProxy for StaticMeshComponent %s (RenderData is not initialized)"), *InSelfPrimitiveComponent->GetFullName());
		return nullptr;
	}

	// Is Nanite supported, and is there built Nanite data for this static mesh?
	if ((InSelfPrimitiveComponent->*InShouldCreateNaniteProxyMemberFunction)()
#if WITH_EDITORONLY_DATA
			&& !InSelfPrimitiveComponent->bDisplayNaniteFallbackMesh
#endif
		)
	{
		LLM_SCOPE(ELLMTag::StaticMesh);

		// Nanite is fully supported
		return ::new Nanite::FSceneProxy(InSelfPrimitiveComponent);
	}

	// If we didn't get a proxy, but Nanite was enabled on the asset when it was built, evaluate proxy creation
	if (InSelfPrimitiveComponent->HasValidNaniteData())
	{
		const bool bAllowProxyRender = NaniteProxyRenderMode == 0
#if WITH_EDITORONLY_DATA
			// Check for specific case of static mesh editor "proxy toggle"
			|| (InSelfPrimitiveComponent->bDisplayNaniteFallbackMesh && NaniteProxyRenderMode == 2)
#endif
			;

		if (!bAllowProxyRender) // Never render proxies
		{
			// We don't want to fall back to Nanite proxy rendering, so just make the mesh invisible instead.
			return nullptr;
		}

		// Fall back to rendering Nanite proxy meshes with traditional static mesh scene proxies
	}

	const FStaticMeshLODResourcesArray& LODResources = InSelfPrimitiveComponent->GetStaticMesh()->GetRenderData()->LODResources;
	const int32 SMCurrentMinLOD = InSelfPrimitiveComponent->GetStaticMesh()->GetMinLODIdx();
	const int32 EffectiveMinLOD = InSelfPrimitiveComponent->bOverrideMinLOD ? InSelfPrimitiveComponent->MinLOD : SMCurrentMinLOD;
	if (LODResources.Num() == 0 || LODResources[FMath::Clamp<int32>(EffectiveMinLOD, 0, LODResources.Num() - 1)].VertexBuffers.StaticMeshVertexBuffer.GetNumVertices() == 0)
	{
		UE_LOG(LogStaticMesh, Verbose, TEXT("Skipping CreateSceneProxy for StaticMeshComponent %s (LOD problems)"), *InSelfPrimitiveComponent->GetFullName());
		return nullptr;
	}

	LLM_SCOPE(ELLMTag::StaticMesh);

	FPrimitiveSceneProxy* Proxy = ::new TPrimitiveSceneProxyType(InSelfPrimitiveComponent, false);
#if STATICMESH_ENABLE_DEBUG_RENDERING
	InSelfPrimitiveComponent->SendRenderDebugPhysics(Proxy);
#endif

	return Proxy;
}

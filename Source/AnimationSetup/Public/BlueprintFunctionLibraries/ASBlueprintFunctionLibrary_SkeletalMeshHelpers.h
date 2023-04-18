// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ASBlueprintFunctionLibrary_SkeletalMeshHelpers.generated.h"



/**
 * Collection of helpful functions related to Skeletal Meshes
 */
UCLASS()
class ANIMATIONSETUP_API UASBlueprintFunctionLibrary_SkeletalMeshHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Create a Skeletal Mesh made up of Skeleton data only.
	 * 
	 * NOTE: You are going to want to use EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones for the Skinned Mesh Component using this.
	 * NOTE: Use USkinnedMeshComponent::bHideSkin unless bInPrimitiveInRendererScene is true.
	 * 
	 * @param InSkeleton                     The Skeleton to resemble
	 * @param bInPrimitiveInRendererScene    Whether to support being in the renderer scene - this shouldn't be necessary
	 * 
	 * @return A procedurally created Skeletal Mesh that resembles the given Skeleton
	 */
	UFUNCTION(BlueprintCallable, Category="SkeletalMeshGenerating")
		static USkeletalMesh* CreateSkinlessSkeletalMeshFromSkeleton(USkeleton* InSkeleton, const bool bInPrimitiveInRendererScene = false);

	/**
	 * Make a Skeletal Mesh containing Skeleton data only.
	 * @param InOutSkeletalMesh              The caller-instantiated Skeletal Mesh for us to initialize - expected to be a fresh USkeletalMesh
	 * @param InSkeleton                     The Skeleton to resemble
	 * @param bInPrimitiveInRendererScene    Whether to support being in the renderer scene - this shouldn't be necessary
	 */
	static void InitializeSkinlessSkeletalMeshFromSkeleton(USkeletalMesh* InOutSkeletalMesh, USkeleton* InSkeleton, const bool bInPrimitiveInRendererScene = false);
};

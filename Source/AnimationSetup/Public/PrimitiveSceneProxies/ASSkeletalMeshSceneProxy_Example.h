// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMeshTypes.h"
#include "PrimitiveSceneProxies/ASPrimitiveSceneProxyData_ViewActorRelevance.h"

//#include "ASSkeletalMeshSceneProxy_Example.generated.h"



/**
 * Skeletal Mesh Scene Proxy that implements ViewActorRelevance functionality.
 * 
 * You are not required to use this and can make your own if you desire.
 */
class ANIMATIONSETUP_API FASSkeletalMeshSceneProxy_Example : public FSkeletalMeshSceneProxy
{
protected:
	FASPrimitiveSceneProxyData_ViewActorRelevance ViewActorRelevance;

public:
	FASSkeletalMeshSceneProxy_Example(const USkinnedMeshComponent* InSkinnedMeshComponent, FSkeletalMeshRenderData* InSkeletalMeshRenderData);

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* InSceneView) const;
};

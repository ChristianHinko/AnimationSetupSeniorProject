// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticMeshResources.h"
#include "PrimitiveSceneProxies/ASPrimitiveSceneProxyData_ViewActorRelevance.h"

//#include "ASStaticMeshSceneProxy_Example.generated.h"



/**
 * Primitive scene proxy that implements ViewActorRelevance functionality.
 * 
 * You are not required to use this and can make your own if you desire.
 */
class ANIMATIONSETUP_API FASStaticMeshSceneProxy_Example : public FStaticMeshSceneProxy
{
protected:
	FASPrimitiveSceneProxyData_ViewActorRelevance ViewActorRelevance;

public:
	FASStaticMeshSceneProxy_Example(UStaticMeshComponent* InStaticMeshComponent, bool bInForceLODsShareStaticLighting);

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* InSceneView) const;
};

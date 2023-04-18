// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveSceneProxies/ASStaticMeshSceneProxy_Example.h"

#include "Interfaces/ASViewActorFilterInterface.h"
#include "Subobjects/ASObject_ViewActorFilter.h"
#include "PrimitiveSceneProxies/ASPrimitiveSceneProxyData_ViewActorRelevance.h"



FASStaticMeshSceneProxy_Example::FASStaticMeshSceneProxy_Example(UStaticMeshComponent* InStaticMeshComponent, bool bInForceLODsShareStaticLighting)
	: FStaticMeshSceneProxy(InStaticMeshComponent, bInForceLODsShareStaticLighting)
{
	if (const IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InStaticMeshComponent))
	{
		// Copy the scene proxy data for ourselves on the render thread
		ViewActorRelevance = ViewActorFilterInterface->GetViewActorFilter()->GetViewActorRelevance();
	}
}

FPrimitiveViewRelevance FASStaticMeshSceneProxy_Example::GetViewRelevance(const FSceneView* InSceneView) const
{
	FPrimitiveViewRelevance PrimitiveViewRelevance = FStaticMeshSceneProxy::GetViewRelevance(InSceneView);

	// Apply our whitelist and blacklist
	ViewActorRelevance.GetViewRelevance(PrimitiveViewRelevance, InSceneView);

	return PrimitiveViewRelevance;
}

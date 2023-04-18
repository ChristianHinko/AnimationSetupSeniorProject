// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveSceneProxies/ASSkeletalMeshSceneProxy_Example.h"

#include "Interfaces/ASViewActorFilterInterface.h"
#include "Subobjects/ASObject_ViewActorFilter.h"
#include "PrimitiveSceneProxies/ASPrimitiveSceneProxyData_ViewActorRelevance.h"



FASSkeletalMeshSceneProxy_Example::FASSkeletalMeshSceneProxy_Example(const USkinnedMeshComponent* InSkinnedMeshComponent, FSkeletalMeshRenderData* InSkeletalMeshRenderData)
	: FSkeletalMeshSceneProxy(InSkinnedMeshComponent, InSkeletalMeshRenderData)
{
	if (const IASViewActorFilterInterface* ViewActorFilterInterface = Cast<IASViewActorFilterInterface>(InSkinnedMeshComponent))
	{
		// Copy the scene proxy data for ourselves on the render thread
		ViewActorRelevance = ViewActorFilterInterface->GetViewActorFilter()->GetViewActorRelevance();
	}
}

FPrimitiveViewRelevance FASSkeletalMeshSceneProxy_Example::GetViewRelevance(const FSceneView* InSceneView) const
{
	FPrimitiveViewRelevance PrimitiveViewRelevance = FSkeletalMeshSceneProxy::GetViewRelevance(InSceneView);

	// Apply our whitelist and blacklist
	ViewActorRelevance.GetViewRelevance(PrimitiveViewRelevance, InSceneView);

	return PrimitiveViewRelevance;
}

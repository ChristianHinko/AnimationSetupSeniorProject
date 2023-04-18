// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveSceneProxies/ASPrimitiveSceneProxyData_ViewActorRelevance.h"



FASPrimitiveSceneProxyData_ViewActorRelevance::FASPrimitiveSceneProxyData_ViewActorRelevance()
	: ViewActorBlacklist(TArray<TWeakObjectPtr<const AActor>>())
	, ViewActorWhitelist(TArray<TWeakObjectPtr<const AActor>>())
{
	// Default ModifyNonRelevant implementation
	ModifyNonRelevant = TDelegate<void(FPrimitiveViewRelevance&)>::CreateLambda([](FPrimitiveViewRelevance& InOutPrimitiveViewRelevance) -> void
			{
				// Hide drawing and shadows
				InOutPrimitiveViewRelevance.bDrawRelevance = false;
				InOutPrimitiveViewRelevance.bShadowRelevance = false;
			}
		);
}

void FASPrimitiveSceneProxyData_ViewActorRelevance::GetViewRelevance(FPrimitiveViewRelevance& InOutPrimitiveViewRelevance, const FSceneView* InSceneView) const
{
	const AActor* const& ViewActor = InSceneView->ViewActor;

	// Check whitelist for non-relevant viewers
	for (const TWeakObjectPtr<const AActor>& WhitelistedViewActor : ViewActorWhitelist)
	{
		if (ViewActor != WhitelistedViewActor)
		{
			// Not relevant
			// Modify relevance and return
			ModifyNonRelevant.Execute(InOutPrimitiveViewRelevance);
			return;
		}
	}

	// Check blacklist for non-relevant viewers
	for (const TWeakObjectPtr<const AActor>& BlacklistedViewActor : ViewActorBlacklist)
	{
		if (ViewActor == BlacklistedViewActor)
		{
			// Not relevant
			// Modify relevance and return
			ModifyNonRelevant.Execute(InOutPrimitiveViewRelevance);
			return;
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ASPrimitiveSceneProxyData_ViewActorRelevance.generated.h"



/**
 * Custom scene proxy data for determining render relevance for the actors viewing the primitive.
 * 
 * This goes on your custom Primitive Scene Proxy.
 */
USTRUCT(BlueprintType)
struct ANIMATIONSETUP_API FASPrimitiveSceneProxyData_ViewActorRelevance
{
	GENERATED_BODY()

	FASPrimitiveSceneProxyData_ViewActorRelevance();

	/** Holds pointers to Actors that are blacklisted from viewing the primitive. */
	UPROPERTY(EditDefaultsOnly)
		TArray<TWeakObjectPtr<const AActor>> ViewActorBlacklist;

	/** Holds pointers to Actors that are whitelisted for viewing the primitive. */
	UPROPERTY(EditDefaultsOnly)
		TArray<TWeakObjectPtr<const AActor>> ViewActorWhitelist;

	/**
	 * Modify primitive view relevance of non-relevant scene views.
	 */
	TDelegate<void(FPrimitiveViewRelevance&)> ModifyNonRelevant;

	/**
	 * Applies this scene proxy data for custom FPrimitiveSceneProxy::GetViewRelevance() implementations.
	 */
	void GetViewRelevance(FPrimitiveViewRelevance& InOutPrimitiveViewRelevance, const FSceneView* InSceneView) const;
};

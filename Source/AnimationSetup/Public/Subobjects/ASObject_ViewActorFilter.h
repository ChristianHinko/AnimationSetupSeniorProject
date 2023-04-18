// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PrimitiveSceneProxies/ASPrimitiveSceneProxyData_ViewActorRelevance.h"

#include "ASObject_ViewActorFilter.generated.h"



/**
 * Extends the Primitive Component with ViewActorRelevance configuration.
 * 
 * This is an object instead of a struct so that we can have UFUNCTIONs.
 */
UCLASS(Within="PrimitiveComponent")
class ANIMATIONSETUP_API UASObject_ViewActorFilter : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Rendering")
		FASPrimitiveSceneProxyData_ViewActorRelevance ViewActorRelevance;

public:
	UASObject_ViewActorFilter();


	const FASPrimitiveSceneProxyData_ViewActorRelevance& GetViewActorRelevance() const { return ViewActorRelevance; }

	UFUNCTION(BlueprintCallable)
		void AddToViewActorBlacklist(const AActor* InActorToAdd);

	UFUNCTION(BlueprintCallable)
		void RemoveFromViewActorBlacklist(const AActor* InActorToRemove);

	UFUNCTION(BlueprintCallable)
		void EmptyViewActorBlacklist();

	UFUNCTION(BlueprintCallable)
		void AddToViewActorWhitelist(const AActor* InActorToAdd);

	UFUNCTION(BlueprintCallable)
		void RemoveFromViewActorWhitelist(const AActor* InActorToRemove);

	UFUNCTION(BlueprintCallable)
		void EmptyViewActorWhitelist();

protected:
	/**
	 * Statically casts our outer to a Primitive Component which is safe because we
	 * are confident that it is one.
	 */
	FORCEINLINE UPrimitiveComponent* GetOuterPrimitiveComponent() const
	{
		check(GetOuter()->IsA<UPrimitiveComponent>())
		return static_cast<UPrimitiveComponent*>(GetOuter());
	}
};

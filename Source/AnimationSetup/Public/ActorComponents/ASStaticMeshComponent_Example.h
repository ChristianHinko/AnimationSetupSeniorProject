// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/ASViewActorFilterInterface.h"

#include "ASStaticMeshComponent_Example.generated.h"


class UASObject_ViewActorFilter;



/**
 * Static Mesh Component that implements the ViewActorRelevance scene proxy.
 * 
 * You are not required to use this and can make your own if you desire.
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class ANIMATIONSETUP_API UASStaticMeshComponent_Example : public UStaticMeshComponent, public IASViewActorFilterInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<UASObject_ViewActorFilter> ViewActorFilter;

public:
	UASStaticMeshComponent_Example();

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual UASObject_ViewActorFilter* GetViewActorFilter() const override { return ViewActorFilter; }
};

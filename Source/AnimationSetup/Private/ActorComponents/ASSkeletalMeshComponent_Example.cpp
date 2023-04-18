// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ASSkeletalMeshComponent_Example.h"

#include "PrimitiveSceneProxies/ASSkeletalMeshSceneProxy_Example.h"
#include "BlueprintFunctionLibraries/ASBlueprintFunctionLibrary_CreateSceneProxyTemplates.h"
#include "Subobjects/ASObject_ViewActorFilter.h"



UASSkeletalMeshComponent_Example::UASSkeletalMeshComponent_Example()
{
	ViewActorFilter = CreateDefaultSubobject<UASObject_ViewActorFilter>(TEXT("ViewActorFilter"));
}

FPrimitiveSceneProxy* UASSkeletalMeshComponent_Example::CreateSceneProxy()
{
	return UASBlueprintFunctionLibrary_CreateSceneProxyTemplates::SkinnedMeshComponentCreateSceneProxy<FASSkeletalMeshSceneProxy_Example>(this);
}

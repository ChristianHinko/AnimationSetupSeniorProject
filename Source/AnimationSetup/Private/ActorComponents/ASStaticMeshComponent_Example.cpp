// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ASStaticMeshComponent_Example.h"

#include "PrimitiveSceneProxies/ASStaticMeshSceneProxy_Example.h"
#include "BlueprintFunctionLibraries/ASBlueprintFunctionLibrary_CreateSceneProxyTemplates.h"
#include "Subobjects/ASObject_ViewActorFilter.h"



UASStaticMeshComponent_Example::UASStaticMeshComponent_Example()
{
	ViewActorFilter = CreateDefaultSubobject<UASObject_ViewActorFilter>(TEXT("ViewActorFilter"));
}

FPrimitiveSceneProxy* UASStaticMeshComponent_Example::CreateSceneProxy()
{
	return UASBlueprintFunctionLibrary_CreateSceneProxyTemplates::StaticMeshComponentCreateSceneProxy<FASStaticMeshSceneProxy_Example>(this, &ThisClass::ShouldCreateNaniteProxy);
}

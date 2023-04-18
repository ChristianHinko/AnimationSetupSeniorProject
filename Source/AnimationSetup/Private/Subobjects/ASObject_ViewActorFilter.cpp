// Fill out your copyright notice in the Description page of Project Settings.


#include "Subobjects/ASObject_ViewActorFilter.h"

#include "PrimitiveSceneProxies/ASSkeletalMeshSceneProxy_Example.h"



UASObject_ViewActorFilter::UASObject_ViewActorFilter()
	: ViewActorRelevance(FASPrimitiveSceneProxyData_ViewActorRelevance())
{
}


void UASObject_ViewActorFilter::AddToViewActorBlacklist(const AActor* InActorToAdd)
{
	if (ViewActorRelevance.ViewActorBlacklist.Contains(InActorToAdd) == false)
	{
		ViewActorRelevance.ViewActorBlacklist.Add(InActorToAdd);
		GetOuterPrimitiveComponent()->MarkRenderStateDirty();
	}
}

void UASObject_ViewActorFilter::RemoveFromViewActorBlacklist(const AActor* InActorToRemove)
{
	if (ViewActorRelevance.ViewActorBlacklist.Contains(InActorToRemove))
	{
		ViewActorRelevance.ViewActorBlacklist.Remove(InActorToRemove);
		GetOuterPrimitiveComponent()->MarkRenderStateDirty();
	}
}

void UASObject_ViewActorFilter::EmptyViewActorBlacklist()
{
	if (ViewActorRelevance.ViewActorBlacklist.Num() > 0)
	{
		ViewActorRelevance.ViewActorBlacklist.Empty();
		GetOuterPrimitiveComponent()->MarkRenderStateDirty();
	}
}

void UASObject_ViewActorFilter::AddToViewActorWhitelist(const AActor* InActorToAdd)
{
	if (ViewActorRelevance.ViewActorWhitelist.Contains(InActorToAdd) == false)
	{
		ViewActorRelevance.ViewActorWhitelist.Add(InActorToAdd);
		GetOuterPrimitiveComponent()->MarkRenderStateDirty();
	}
}

void UASObject_ViewActorFilter::RemoveFromViewActorWhitelist(const AActor* InActorToRemove)
{
	if (ViewActorRelevance.ViewActorWhitelist.Contains(InActorToRemove))
	{
		ViewActorRelevance.ViewActorWhitelist.Remove(InActorToRemove);
		GetOuterPrimitiveComponent()->MarkRenderStateDirty();
	}
}

void UASObject_ViewActorFilter::EmptyViewActorWhitelist()
{
	if (ViewActorRelevance.ViewActorWhitelist.Num() > 0)
	{
		ViewActorRelevance.ViewActorWhitelist.Empty();
		GetOuterPrimitiveComponent()->MarkRenderStateDirty();
	}
}

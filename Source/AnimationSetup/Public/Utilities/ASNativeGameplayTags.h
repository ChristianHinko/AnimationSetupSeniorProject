// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"



/**
 * This module's Native Gameplay Tags wrapped in a namespace.
 * 
 * You should include this header in your project's PCH.
 * This is nice so that you can just start typing "ASNativeGameplayTags::" and then see the list of all of the module's Gameplay Tags.
 */
namespace ASNativeGameplayTags
{
	// Portrayals
	ANIMATIONSETUP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Portrayal_Default)
	ANIMATIONSETUP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Portrayal_OwnerViewers)
	ANIMATIONSETUP_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Portrayal_SelfViewer)
}

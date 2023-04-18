// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ASBlueprintFunctionLibrary_SkeletalMeshComponentHelpers.generated.h"


class UCapsuleComponent;



/**
 * Collection of helpful functions related to Skeletal Mesh Components
 */
UCLASS()
class ANIMATIONSETUP_API UASBlueprintFunctionLibrary_SkeletalMeshComponentHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Set up default transform for the given Skeletal Mesh Component.
	 * This is safe and intended to use in object constructors.
	 */
	UFUNCTION(BlueprintCallable, Category="Transform")
		static void ConfigureDefaultSkeletalMeshComponentTransform(USceneComponent* InSkeletalMeshComponent, const UCapsuleComponent* InCapsuleComponent);
};

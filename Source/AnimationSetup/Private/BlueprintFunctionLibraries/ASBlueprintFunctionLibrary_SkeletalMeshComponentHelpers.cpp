// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibraries/ASBlueprintFunctionLibrary_SkeletalMeshComponentHelpers.h"

#include "Components/CapsuleComponent.h"



void UASBlueprintFunctionLibrary_SkeletalMeshComponentHelpers::ConfigureDefaultSkeletalMeshComponentTransform(USceneComponent* InSkeletalMeshComponent, const UCapsuleComponent* InCapsuleComponent)
{
	if (!IsValid(InSkeletalMeshComponent))
	{
		return;
	}

	FRotator DefaultRotation;
	FVector DefaultLocation;
	FVector DefaultScale;

	// Rotate the mesh to face forward
	DefaultRotation = FRotator(0.f, -90.f, 0.f);

	// Put the mesh at the bottom of the capsule
	if (IsValid(InCapsuleComponent))
	{
		const float CapsuleHalfHeightScaled = InCapsuleComponent->GetScaledCapsuleHalfHeight();
		DefaultLocation = FVector(0.f, 0.f, -CapsuleHalfHeightScaled);
	}

	// Give default scale
	DefaultScale = FVector(1.f, 1.f, 1.f);

	InSkeletalMeshComponent->SetRelativeTransform(FTransform(DefaultRotation, DefaultLocation, DefaultScale));
}

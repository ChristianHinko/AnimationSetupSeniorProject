// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/ASSkeletalPartDefinition.h"



AActor* FASSkeletalPartDefinition::SpawnAndAttachPartActor(AActor* InOwner, USceneComponent* InComponentToAttachTo) const
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogASSkeletalPartDefinition, Error, TEXT("%s() could not spawn part Actor because the given InOwner was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	if (!IsValid(PartActorClass))
	{
		UE_LOG(LogASSkeletalPartDefinition, Error, TEXT("%s() could not spawn part Actor because the PartActorClass was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.ObjectFlags |= RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = InOwner;

	FTransform SpawnTransform = InOwner->GetActorTransform();
	if (IsValid(InComponentToAttachTo))
	{
		// Use the attachment transform
		// This doesn't really matter since we are just going to attach to there anyways
		SpawnTransform = InComponentToAttachTo->GetSocketTransform(SocketName);
	}

	// Spawn and attach the part actor
	AActor* SpawnedPartActor = InOwner->GetWorld()->SpawnActor<AActor>(PartActorClass, SpawnTransform, SpawnParameters);
	if (IsValid(SpawnedPartActor))
	{
		SpawnedPartActor->AttachToComponent(InComponentToAttachTo, AttachmentTransformRules, SocketName);
	}

	return SpawnedPartActor;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/ASAttachmentDefinition.h"



AActor* FASAttachmentDefinition::SpawnAndAttach(AActor* InOwner, USceneComponent* InAttachee) const
{
	if (!IsValid(InOwner))
	{
		UE_LOG(LogASAttachmentDefinition, Error, TEXT("%s() Could not spawn attachment because the given InOwner was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	if (!IsValid(ActorClass))
	{
		UE_LOG(LogASAttachmentDefinition, Error, TEXT("%s() Could not spawn attachment because the ActorClass was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.ObjectFlags |= RF_Transient;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = InOwner;

	FTransform SpawnTransform = InOwner->GetActorTransform();
	if (IsValid(InAttachee))
	{
		// Use the attachment transform
		// This doesn't really matter since we are just going to attach to there anyways
		SpawnTransform = InAttachee->GetSocketTransform(SocketName);
	}

	// Spawn and attach the attachment
	AActor* SpawnedAttachment = InOwner->GetWorld()->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParameters);
	if (IsValid(SpawnedAttachment))
	{
		SpawnedAttachment->AttachToComponent(InAttachee, AttachmentTransformRules, SocketName);
	}

	return SpawnedAttachment;
}

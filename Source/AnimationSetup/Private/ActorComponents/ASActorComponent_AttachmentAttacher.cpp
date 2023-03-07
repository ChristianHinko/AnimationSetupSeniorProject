// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ASActorComponent_AttachmentAttacher.h"

#include "Types/ASAttachmentDefinition.h"



UASActorComponent_AttachmentAttacher::UASActorComponent_AttachmentAttacher(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}


#if WITH_EDITOR
void UASActorComponent_AttachmentAttacher::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, AttacheeReference))
	{
		// NOTE: This change should already be covered by OnRegister() for the Blueprint editor preview but we will update the reference here too just in case for other possible scenerios.
		Attachee = Cast<USceneComponent>(AttacheeReference.GetComponent(GetOwner()));
	}
}
#endif // WITH_EDITOR

void UASActorComponent_AttachmentAttacher::OnRegister()
{
	Super::OnRegister();

	Attachee = Cast<USceneComponent>(AttacheeReference.GetComponent(GetOwner()));
}

void UASActorComponent_AttachmentAttacher::SpawnAttachments()
{
	// Spawn the attachments
	for (const FASAttachmentDefinition& AttachmentDefinition : AttachmentDefinitions)
	{
		AActor* Attachment = AttachmentDefinition.SpawnAndAttach(GetOwner(), Attachee.Get());
		Attachments.Add(Attachment);
	}
}

void UASActorComponent_AttachmentAttacher::DestroyAttachments()
{
	// Destroy all attachments
	for (TObjectPtr<AActor>& Attachment : Attachments)
	{
		if (IsValid(Attachment))
		{
			const bool bDestroyed = Attachment->Destroy();
			if (bDestroyed)
			{
				Attachment = nullptr;
			}
			else
			{
				UE_LOG(LogASAttachmentAttacherComponent, Warning, TEXT("%s() Attachment [%s] was not destroyed"), ANSI_TO_TCHAR(__FUNCTION__), *(Attachment->GetName()));
			}
		}
	}

	Attachments.Empty();
}

void UASActorComponent_AttachmentAttacher::OnUnregister()
{
	Super::OnUnregister();

	DestroyAttachments();
}

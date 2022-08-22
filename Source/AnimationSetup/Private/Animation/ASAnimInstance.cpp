// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ASAnimInstance.h"



UASAnimInstance::UASAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	const UAnimBlueprintGeneratedClass* AnimBlueprintGeneratedClass = Cast<UAnimBlueprintGeneratedClass>(IAnimClassInterface::GetFromClass(GetClass()));
	if (IsValid(AnimBlueprintGeneratedClass))
	{
		UAnimBlueprint* AnimBlueprint = Cast<UAnimBlueprint>(AnimBlueprintGeneratedClass->ClassGeneratedBy); // get our corresponding Anim Blueprint
		if (IsValid(AnimBlueprint))
		{
			AnimBlueprint->bWarnAboutBlueprintUsage = true;
		}
	}
#endif // WITH_EDITOR
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

//#include "AbilitySystemSetup.generated.h" // if we ever need reflection in here



/**
 * 
 */
class FAnimationSetupModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#include "StatOSC.h"
#include "ISettingsModule.h"

#include "StatOSCSubsystem.h"

#define LOCTEXT_NAMESPACE "FStatOSCModule"

void FStatOSCModule::StartupModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"StatOSC",
			FText::FromString(TEXT("StatOSC")),
			FText::FromString(TEXT("")),
			GetMutableDefault<UStatOSCSubsystemSettings>()
		);
	}
}

void FStatOSCModule::ShutdownModule()
{
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings(
			"Project",
			"Plugins",
			"StatOSC"
		);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStatOSCModule, StatOSC)

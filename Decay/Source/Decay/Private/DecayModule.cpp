// Copyright Epic Games, Inc. All Rights Reserved.

#include "DecayModule.h"

#include "DecaySettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FDecayModule"

DEFINE_LOG_CATEGORY(LogDecay);


void FDecayModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Decay",
			LOCTEXT("RuntimeSettingsName", "Decay"), LOCTEXT("RuntimeSettingsDescription", "Configure Decay"),
			GetMutableDefault<UDecaySettings>());
	}
}

void FDecayModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Decay");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDecayModule, Decay)
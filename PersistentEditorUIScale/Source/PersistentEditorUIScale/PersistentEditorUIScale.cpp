#include "PersistentEditorUIScale.h"
#include "PersistentEditorUIScaleSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FPersistentEditorUIScaleModule"

void FPersistentEditorUIScaleModule::StartupModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Editor", "Plugins", "Persistent Editor UI Scale Settings",
			LOCTEXT("Persistent Editor UI Scale Settings", "Persistent Editor UI Scale"), LOCTEXT("Persistent Editor UI Scale Settings Description", "Configure Persistent Editor UI Scale"),
			GetMutableDefault<UPersistentEditorUIScaleSettings>());
	}

	FSlateApplication& SlateApp = FSlateApplication::Get();
    SlateApp.SetApplicationScale(GetDefault<UPersistentEditorUIScaleSettings>()->UIScale);
}

void FPersistentEditorUIScaleModule::ShutdownModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Editor", "Plugins", "Persistent Editor UI Scale Settings");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPersistentEditorUIScaleModule, PersistentEditorUIScale)

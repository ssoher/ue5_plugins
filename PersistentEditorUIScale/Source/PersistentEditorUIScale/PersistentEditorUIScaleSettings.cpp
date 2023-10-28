#include "PersistentEditorUIScaleSettings.h"

UPersistentEditorUIScaleSettings::UPersistentEditorUIScaleSettings() {
    UIScale = 1.0f;
}

void UPersistentEditorUIScaleSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (!PropertyChangedEvent.MemberProperty)
    {
        return;
    }

    const FName PropertyChangedName = PropertyChangedEvent.MemberProperty->GetFName();

    if (PropertyChangedName == GET_MEMBER_NAME_CHECKED(UPersistentEditorUIScaleSettings, UIScale))
    {
        FSlateApplication& SlateApp = FSlateApplication::Get();
        SlateApp.SetApplicationScale(UIScale);
    }
}
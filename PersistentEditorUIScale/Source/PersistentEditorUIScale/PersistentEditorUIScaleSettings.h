#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PersistentEditorUIScaleSettings.generated.h"

UCLASS(config = PersistentEditorUIScale)
class PERSISTENTEDITORUISCALE_API UPersistentEditorUIScaleSettings : public UObject
{
    GENERATED_BODY()

public:
    UPersistentEditorUIScaleSettings();

    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

    UPROPERTY(config, EditAnywhere, Category = "Persistent Editor UI Scale")
    float UIScale;
};

#pragma once

#include "Modules/ModuleInterface.h"
#include "SublimeTextSourceCodeAccessor.h"

class FSublimeTextSourceCodeAccessModule : public IModuleInterface {
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	FSublimeTextSourceCodeAccessor& GetAccessor();

private:
	FSublimeTextSourceCodeAccessor SublimeTextSourceCodeAccessor;

};
#include "SublimeTextSourceCodeAccessModule.h"
#include "Modules/ModuleManager.h"
#include "Features/IModularFeatures.h"

IMPLEMENT_MODULE( FSublimeTextSourceCodeAccessModule, SublimeTextSourceCodeAccess );

void FSublimeTextSourceCodeAccessModule::StartupModule() {
	IModularFeatures::Get().RegisterModularFeature(TEXT("SourceCodeAccessor"), &SublimeTextSourceCodeAccessor );
}

void FSublimeTextSourceCodeAccessModule::ShutdownModule() {
	IModularFeatures::Get().UnregisterModularFeature(TEXT("SourceCodeAccessor"), &SublimeTextSourceCodeAccessor);
}

FSublimeTextSourceCodeAccessor& FSublimeTextSourceCodeAccessModule::GetAccessor() {
	return SublimeTextSourceCodeAccessor;
}
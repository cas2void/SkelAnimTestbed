#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHandEditSkinnedMeshComponent, All, All)

class FHandEditSkinnedMeshComponentModule : public IModuleInterface
{
	//
	// IModuleInterface Interfaces
	//
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
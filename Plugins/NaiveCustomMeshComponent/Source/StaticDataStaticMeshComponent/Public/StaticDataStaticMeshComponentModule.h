#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogStaticDataStaticMeshComponent, All, All)

class FStaticDataStaticMeshComponentModule : public IModuleInterface
{
	//
	// IModuleInterface Interfaces
	//
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FThirdPersonModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

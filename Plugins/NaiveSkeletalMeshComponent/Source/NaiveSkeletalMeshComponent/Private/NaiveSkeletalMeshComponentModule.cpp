#include "NaiveSkeletalMeshComponentModule.h"

#define LOCTEXT_NAMESPACE "FNaiveSkeletalMeshComponentModule"

DEFINE_LOG_CATEGORY(LogNaiveSkeletalMeshComponent)

void FNaiveSkeletalMeshComponentModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogNaiveSkeletalMeshComponent, Log, TEXT("StartupModule"));
}

void FNaiveSkeletalMeshComponentModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNaiveSkeletalMeshComponentModule, NaiveSkeletalMeshComponent)
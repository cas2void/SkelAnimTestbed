#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"
#include "StaticMeshResources.h"
#include "RawIndexBuffer.h"
#include "LocalVertexFactory.h"

#include "StaticDataStaticMeshComponent.h"

/**
 * 
 */
class STATICDATASTATICMESHCOMPONENT_API FStaticDataStaticMeshSceneProxy : public FPrimitiveSceneProxy
{
public:
	FStaticDataStaticMeshSceneProxy(const UStaticDataStaticMeshComponent* Component);
	virtual ~FStaticDataStaticMeshSceneProxy();

	//
	// FPrimitiveSceneProxy Interfaces
	//
public:
	virtual SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override;

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	virtual uint32 GetMemoryFootprint(void) const override
	{
		return(sizeof(*this) + GetAllocatedSize());
	}

private:
	FStaticMeshVertexBuffers StaticVertexBuffers;

	FRawStaticIndexBuffer IndexBuffer;

	FLocalVertexFactory VertexFactory;

	class UMaterialInterface* Material;
};

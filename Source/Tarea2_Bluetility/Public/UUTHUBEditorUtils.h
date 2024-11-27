#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UUTHUBEditorUtils.generated.h"


UCLASS()
class TAREA2_BLUETILITY_API UUUTHUBEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(InNumTriangles = 10000.f, InNumMaterials = 5.f, InNumMeshSize = 6000.f), Category = "StaticMeshUtility")
	static void ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utils")
	static FString GetExportDirRework();
	
	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportsActors(const TArray<UObject*>& ActorsToExports);
};

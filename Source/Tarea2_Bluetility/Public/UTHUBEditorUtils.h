#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UTHUBEditorUtils.generated.h"


UCLASS()
class TAREA2_BLUETILITY_API UUTHUBEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utils")
	static FString GetExportDirRework();

	UFUNCTION(BlueprintCallable, meta=(InNumTriangles = 10000.f, InNumMaterials = 5.f, InNumMeshSize = 6000.f), Category = "StaticMeshUtility")
	static void ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize);

	UFUNCTION()
	static bool CheckValidations(float InNumTriangles, float InNumMaterials, float InNumMeshSize, const UStaticMeshComponent* MeshComp);
	
	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportsActors(const TArray<FString>& ActorsToExports);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static void LogAssetDependencies();
	
};

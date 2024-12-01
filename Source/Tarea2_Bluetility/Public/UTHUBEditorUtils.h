#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UTHUBEditorUtils.generated.h"


class IAssetRegistry;

UCLASS()
class TAREA2_BLUETILITY_API UUTHUBEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/***** START Export Static Mesh Actors of Level *****/
	UFUNCTION()
	static bool CheckValidations(float InNumTriangles, float InNumMaterials, float InNumMeshSize, const UStaticMeshComponent* MeshComp);
	/***** END Export Static Mesh Actors of Level *****/

	/***** START Get Assets Not referenced and theirs dependencies *****/
	// UFUNCTION()
	// static void GetDependenciesRecursively(const FName& AssetName, IAssetRegistry& AssetRegistry, TSet<FName>& OutDependencies, TSet<FName>& VisitedAssets);
	/***** END Get Assets Not referenced and theirs dependencies *****/

	
public:

	/***** START Export Static Mesh Actors of Level *****/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utils")
	static FString GetExportDirRework();

	UFUNCTION(BlueprintCallable, meta=(InNumTriangles = 10000.f, InNumMaterials = 5.f, InNumMeshSize = 6000.f), Category = "StaticMeshUtility")
	static void ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize);
	
	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportsActors(const TArray<FString>& ActorsToExports);
	/***** END Export Static Mesh Actors of Level *****/
	
	/***** START Get Assets Not referenced and theirs dependencies *****/
	UFUNCTION(BlueprintCallable, Category = "Utilities")
	static void ListAssetsWithDependencies();

	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportListAsLogFile(const TArray<FString>& InList);
	/***** END Get Assets Not referenced and theirs dependencies *****/
};

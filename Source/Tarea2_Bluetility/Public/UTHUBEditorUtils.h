#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UTHUBEditorUtils.generated.h"

class IAssetRegistry;
class FJsonObject;

USTRUCT(BlueprintType, Category = "Utils | Json")
struct FMeshValidationJson
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utils | Json") bool bHasManyTris;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utils | Json") bool bHasManyMats;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utils | Json") bool bIsMassive;
};

UCLASS()
class TAREA2_BLUETILITY_API UUTHUBEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/***** START Export Static Mesh Actors of Level *****/
	static void CheckValidations(float InNumTriangles, float InNumMaterials, float InNumMeshSize, const UStaticMeshComponent* MeshComp, FMeshValidationJson& ValidationStruct);
	/***** END Export Static Mesh Actors of Level *****/

	/***** START Get Assets Not referenced and theirs dependencies *****/
	static void GetAllDependenciesRecursive(const IAssetRegistry& AssetRegistry, const FName& PackageName, TSet<FName>& AllDependencies);
	/***** END Get Assets Not referenced and theirs dependencies *****/

	
public:

	/***** START Export Static Mesh Actors of Level *****/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utils")
	static FString GetExportDirRework();

	UFUNCTION(BlueprintCallable, meta=(InNumTriangles = 10000.f, InNumMaterials = 5.f, InNumMeshSize = 6000.f), Category = "Utils | Ejercicio2")
	static void ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize);
	
	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportActors(const TArray<FString>& ActorsToExports);
	/***** END Export Static Mesh Actors of Level *****/

	static void GetDependenciesRecursively(const IAssetRegistry& AssetRegistry, const FAssetData& Asset, TArray<FString> Array);
	/***** START Get Assets Not referenced and theirs dependencies *****/
	UFUNCTION(BlueprintCallable, Category = "Utils | Ejercicio1")
	static void ListAssetsWithDependencies();

	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportListAsLogFile(const FString& LogContent);
	/***** END Get Assets Not referenced and theirs dependencies *****/

};

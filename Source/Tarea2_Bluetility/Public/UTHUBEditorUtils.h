﻿#pragma once

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
	UFUNCTION()
	static void CheckValidations(float InNumTriangles, float InNumMaterials, float InNumMeshSize, const UStaticMeshComponent* MeshComp, FMeshValidationJson& ValidationStruct);
	/***** END Export Static Mesh Actors of Level *****/

	/***** START Get Assets Not referenced and theirs dependencies *****/
	// UFUNCTION()
	// static void GetDependenciesRecursively(const FName& AssetName, IAssetRegistry& AssetRegistry, TSet<FName>& OutDependencies, TSet<FName>& VisitedAssets);
	/***** END Get Assets Not referenced and theirs dependencies *****/

	
public:

	/***** START Export Static Mesh Actors of Level *****/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Utils")
	static FString GetExportDirRework();

	UFUNCTION(BlueprintCallable, meta=(InNumTriangles = 10000.f, InNumMaterials = 5.f, InNumMeshSize = 6000.f), Category = "Utils | Ejercicio2")
	static void ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize);
	
	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportActors(const TArray<FString>& ActorsToExports);
	
	// UFUNCTION(BlueprintCallable, Category="Utils")
	// static void ExportJson(TArray<TSharedPtr<FJsonObject>> Array);
	
	/***** END Export Static Mesh Actors of Level *****/
	
	/***** START Get Assets Not referenced and theirs dependencies *****/
	UFUNCTION(BlueprintCallable, Category = "Utils | Ejercicio1")
	static void ListAssetsWithDependencies();

	UFUNCTION(BlueprintCallable, Category="Utils")
	static void ExportListAsLogFile(const TArray<FString>& InList);
	/***** END Get Assets Not referenced and theirs dependencies *****/

};

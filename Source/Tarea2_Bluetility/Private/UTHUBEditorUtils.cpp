#include "Tarea2_Bluetility/Public/UTHUBEditorUtils.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

void UUTHUBEditorUtils::ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize)
{
    const UWorld* World = GEditor->GetEditorWorldContext().World();
    if(!World) return;

    TArray<FString> ActorsPathsToExports;
	TArray<TSharedPtr<FJsonObject>> ValidationMeshJson;

    
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

    for (const AActor* Actor : FoundActors)
    {
        const UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
        if (!MeshComp) continue;

        const UStaticMesh* Mesh = MeshComp->GetStaticMesh();
        if (!Mesh) continue;

        FMeshValidationJson ValidationStruct;
    	CheckValidations(InNumTriangles, InNumMaterials, InNumMeshSize, MeshComp, ValidationStruct);
    	
        if(ValidationStruct.bIsMassive || ValidationStruct.bHasManyMats || ValidationStruct.bHasManyTris)
        {
            FString PathString;
            UKismetSystemLibrary::BreakSoftObjectPath(UKismetSystemLibrary::GetSoftObjectPath(Mesh), PathString);
            //FTopLevelAssetPath TopLevelAssetPath = SoftClassPath.GetAssetPath();
            
            //UE_LOG(LogTemp, Display, TEXT("%s"), *TopLevelAssetPath.GetPackageName().ToString());
            //ActorsPathsToExports.Add(TopLevelAssetPath.GetPackageName().ToString());
        	ActorsPathsToExports.AddUnique(PathString);
        	
        	TSharedPtr<FJsonObject> MeshEntry = MakeShareable(new FJsonObject());
        	MeshEntry->SetBoolField("bHasManyTris", ValidationStruct.bHasManyTris);
        	MeshEntry->SetBoolField("bHasManyMats", ValidationStruct.bHasManyMats);
        	MeshEntry->SetBoolField("bIsMassive", ValidationStruct.bIsMassive);
            MeshEntry->SetStringField("MeshName", Mesh->GetName());
        	ValidationMeshJson.AddUnique(MeshEntry);
        }
    }
    ExportsActors(ActorsPathsToExports);

	// Exportar a JSON
	FString JSONOutput;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONOutput);
	Writer->WriteObjectStart();
	Writer->WriteArrayStart(TEXT("Meshes"));

	for (const TSharedPtr<FJsonObject>& Entry : ValidationMeshJson)
	{
		FJsonSerializer::Serialize(Entry.ToSharedRef(), Writer);
	}

	Writer->WriteArrayEnd();
	Writer->WriteObjectEnd();
	Writer->Close();

    const FString JSONPath = FPaths::Combine(GetExportDirRework(), TEXT("StaticMeshRework_report.json"));
	FFileHelper::SaveStringToFile(JSONOutput, *JSONPath);
}

void UUTHUBEditorUtils::CheckValidations(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize,
                                          const UStaticMeshComponent* MeshComp, FMeshValidationJson& ValidationStruct)
{
    // Verificar condiciones
    const bool bHasManyTris = MeshComp->GetStaticMesh()->GetRenderData()->LODResources[0].GetNumTriangles() > InNumTriangles;

    const int32 MaterialCount = MeshComp->GetNumMaterials();
    const bool bHasManyMats = MaterialCount > InNumMaterials;

    const FVector MeshSize = MeshComp->GetStaticMesh()->GetBoundingBox().GetSize();
    const bool bIsMassive = MeshSize.Size() > InNumMeshSize;

	ValidationStruct.bHasManyTris = bHasManyTris;
	ValidationStruct.bHasManyMats = bHasManyMats;
	ValidationStruct.bIsMassive = bIsMassive;
}

FString UUTHUBEditorUtils::GetExportDirRework()
{
    return FPaths::ProjectSavedDir() / TEXT("Rework");
}

void UUTHUBEditorUtils::ExportsActors(const TArray<FString>& ActorsToExports)
{
    const FAssetToolsModule& Module = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
    
    Module.Get().ExportAssets(ActorsToExports, GetExportDirRework());
}

void UUTHUBEditorUtils::ListAssetsWithDependencies()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> OutputText;
	
	TArray<FAssetData> AllAssets;
	AssetRegistry.GetAllAssets(AllAssets);

	for (const FAssetData& Asset : AllAssets)
	{
		
		TArray<FName> References;
		AssetRegistry.GetReferencers(Asset.AssetName,References);
		if (References.Num() > 0)
		{
			continue;
		}
		
		OutputText.Add(FString::Printf(TEXT("%s [ROOT]\n"), *Asset.GetSoftObjectPath().ToString()));
		
		// TSet<FName> Dependencies;
		// TSet<FName> VisitedAssets; // Evitar ciclos infinitos
		// GetDependenciesRecursively(Asset.PackageName, AssetRegistry, Dependencies, VisitedAssets);
		//
		// // Si el asset tiene dependencias, añadir al mapa
		// if (Dependencies.Num() > 0)
		// {
		// 	AssetDependencies.Add(Asset.PackageName, Dependencies);
		// }
	}
	ExportListAsLogFile(OutputText);	
}

// void UUTHUBEditorUtils::GetDependenciesRecursively(const FName& AssetName, IAssetRegistry& AssetRegistry, TSet<FName>& OutDependencies, TSet<FName>& VisitedAssets)
// {
// 	// Evitar procesar el mismo asset más de una vez
// 	if (VisitedAssets.Contains(AssetName))
// 	{
// 		return;
// 	}
// 	VisitedAssets.Add(AssetName);
//
// 	TArray<FName> DirectDependencies;
// 	// Si falla la consulta, loguear y continuar
// 	if (!AssetRegistry.GetDependencies(AssetName, DirectDependencies))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("No se pudieron obtener las dependencias para el asset %s"), *AssetName.ToString());
// 		return;
// 	}
//
// 	for (const FName& Dependency : DirectDependencies)
// 	{
// 		if (!OutDependencies.Contains(Dependency))
// 		{
// 			OutDependencies.Add(Dependency);
// 			// Llamada recursiva para explorar subdependencias
// 			GetDependenciesRecursively(Dependency, AssetRegistry, OutDependencies, VisitedAssets);
// 		}
// 	}
// }

void UUTHUBEditorUtils::ExportListAsLogFile(const TArray<FString>& InList)
{
	if(InList.IsEmpty()) return;
	
	const FString FileName = FPaths::ProjectSavedDir() /
							TEXT("Logs/RefLogOut_") +
							FDateTime::Now().ToString() +
							TEXT(".log");

	if(FFileHelper::SaveStringArrayToFile(InList, *FileName))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("Log de dependencias guardado en: %s"), *FileName)));
	}
}
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
            MeshEntry->SetStringField("MeshName", Mesh->GetName());
        	MeshEntry->SetBoolField("bHasManyTris", ValidationStruct.bHasManyTris);
        	MeshEntry->SetBoolField("bHasManyMats", ValidationStruct.bHasManyMats);
        	MeshEntry->SetBoolField("bIsMassive", ValidationStruct.bIsMassive);
        	ValidationMeshJson.AddUnique(MeshEntry);
        }
    }
    ExportActors(ActorsPathsToExports);
	//ExportJson(ValidationMeshJson); Me da error en el TSharedRef y no veo porqueee

	// Exportar a JSON
	FString JSONOutput;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONOutput);
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

void UUTHUBEditorUtils::ExportActors(const TArray<FString>& ActorsToExports)
{
    const FAssetToolsModule& Module = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
    Module.Get().ExportAssets(ActorsToExports, GetExportDirRework());
}

void UUTHUBEditorUtils::ListAssetsWithDependencies()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AllAssets;
	AssetRegistry.GetAllAssets(AllAssets);
	
	FStringBuilderBase OutputBuilder;

	for (const FAssetData& Asset : AllAssets)
	{
		TArray<FName> References;
		if (!AssetRegistry.GetReferencers(Asset.AssetName, References))
		{
			OutputBuilder.Appendf(TEXT("%s [ROOT]\n"), *Asset.GetSoftObjectPath().ToString());

			TSet<FName> AllDependencies;
			GetAllDependenciesRecursive(AssetRegistry, Asset.PackageName, AllDependencies);

			for (const FName& Dependency : AllDependencies)
			{
				OutputBuilder.Appendf(TEXT("   | - %s\n"), *Dependency.ToString());
			}
		}
	}
	ExportListAsLogFile(OutputBuilder.ToString());
}

void UUTHUBEditorUtils::GetAllDependenciesRecursive(const IAssetRegistry& AssetRegistry, const FName& PackageName, TSet<FName>& AllDependencies)
{
	TArray<FName> DirectDependencies;
	if (AssetRegistry.GetDependencies(PackageName, DirectDependencies))
	{
		for (const FName& Dependency : DirectDependencies)
		{
			// Solo procesar si aún no hemos agregado esta dependencia
			if (!AllDependencies.Contains(Dependency))
			{
				AllDependencies.Add(Dependency);
				// Llamada recursiva para procesar las dependencias de este elemento
				GetAllDependenciesRecursive(AssetRegistry, Dependency, AllDependencies);
			}
		}
	}
}

void UUTHUBEditorUtils::ExportListAsLogFile(const FString& LogContent)
{
	if(LogContent.IsEmpty()) return;
	
	const FString FilePath = FPaths::ProjectSavedDir() /
							TEXT("Logs/RefLogOut_") +
							FDateTime::Now().ToString() +
							TEXT(".log");
	
	if(FFileHelper::SaveStringToFile(LogContent, *FilePath))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("Log de dependencias guardado en: %s"), *FilePath)));
	}
}

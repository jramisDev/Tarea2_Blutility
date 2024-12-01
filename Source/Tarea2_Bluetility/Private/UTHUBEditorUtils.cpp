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
    
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

    for (const AActor* Actor : FoundActors)
    {
        const UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
        if (!MeshComp) continue;

        const UStaticMesh* Mesh = MeshComp->GetStaticMesh();
        if (!Mesh) continue;

        if(CheckValidations(InNumTriangles, InNumMaterials, InNumMeshSize, MeshComp))
        {
            FString PathString;
            UKismetSystemLibrary::BreakSoftObjectPath(UKismetSystemLibrary::GetSoftObjectPath(Mesh), PathString);
            //FTopLevelAssetPath TopLevelAssetPath = SoftClassPath.GetAssetPath();
            
            //UE_LOG(LogTemp, Display, TEXT("%s"), *TopLevelAssetPath.GetPackageName().ToString());
            //ActorsPathsToExports.Add(TopLevelAssetPath.GetPackageName().ToString());
            ActorsPathsToExports.Add(PathString);
        }
    }
    ExportsActors(ActorsPathsToExports);    
}

bool UUTHUBEditorUtils::CheckValidations(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize,
                                          const UStaticMeshComponent* MeshComp)
{
    // Verificar condiciones
    const bool bHasManyTris = MeshComp->GetStaticMesh()->GetRenderData()->LODResources[0].GetNumTriangles() > InNumTriangles;

    const int32 MaterialCount = MeshComp->GetNumMaterials();
    const bool bHasManyMats = MaterialCount > InNumMaterials;

    const FVector MeshSize = MeshComp->GetStaticMesh()->GetBoundingBox().GetSize();
    const bool bIsMassive = MeshSize.Size() > InNumMeshSize;

    return (bHasManyTris || bHasManyMats || bIsMassive);
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

void UUTHUBEditorUtils::LogAssetDependencies()
{

    TArray<FString> OutputLines;

    // Obtener el módulo del Asset Registry
    const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    // Obtener todos los assets en el proyecto
    TArray<FAssetData> AllAssets;
    AssetRegistry.GetAllAssets(AllAssets);

    for (const FAssetData& Asset : AllAssets)
    {
        TArray<FName> Dependencies;
        AssetRegistry.GetDependencies(Asset.PackageName, Dependencies);

        if (Dependencies.Num() > 0)
        {
            FString AssetRoot = FString::Printf(TEXT("%s [ROOT]"), *Asset.AssetName.ToString());
            OutputLines.Add(AssetRoot);

            for (const FName& Dependency : Dependencies)
            {
                FString DependencyLine = FString::Printf(TEXT("|- %s"), *Dependency.ToString());
                OutputLines.Add(DependencyLine);
            }
        }
    }

    // Escribir la salida a un archivo
    const FString FileName = FString::Printf(TEXT("RefLogOut_%s.log"), *FDateTime::Now().ToString());
    const FString FilePath = FPaths::ProjectSavedDir() / TEXT("Logs") / FileName;

    FFileHelper::SaveStringArrayToFile(OutputLines, *FilePath);

    // Mostrar un mensaje de confirmación
    FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("Log de dependencias guardado en: %s"), *FilePath)));
}

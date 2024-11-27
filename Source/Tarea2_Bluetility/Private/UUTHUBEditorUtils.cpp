#include "Tarea2_Bluetility/Public/UUTHUBEditorUtils.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

void UUUTHUBEditorUtils::ListAndExportStaticMeshes(const float InNumTriangles, const float InNumMaterials, const float InNumMeshSize)
{
    const UWorld* World = GEditor->GetEditorWorldContext().World();
    if(!World) return;

    TArray<UObject*> ActorsPathsToExports;
    
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

    for (const AActor* Actor : FoundActors)
    {
        const AStaticMeshActor* SMActor = Cast<AStaticMeshActor>(Actor);
        if (!SMActor) continue;

        const UStaticMeshComponent* MeshComp = SMActor->GetStaticMeshComponent();
        UStaticMesh* Mesh = MeshComp->GetStaticMesh();
        if (!Mesh) continue;

        // Verificar condiciones
        const bool bHasManyTris = Mesh->GetRenderData()->LODResources[0].GetNumTriangles() > InNumTriangles;

        const int32 MaterialCount = MeshComp->GetNumMaterials();
        const bool bHasManyMats = MaterialCount > InNumMaterials;

        FVector MeshSize = Mesh->GetBoundingBox().GetSize();
        const bool bIsMassive = MeshSize.Size() > InNumMeshSize;

        if (bHasManyTris || bHasManyMats || bIsMassive)
        {            
            UE_LOG(LogTemp, Display, TEXT("%s"), *SMActor->GetFName().ToString());
            ActorsPathsToExports.Add(Mesh);
        }
    }

    ExportsActors(ActorsPathsToExports);
    
}

FString UUUTHUBEditorUtils::GetExportDirRework()
{
    return FPaths::ProjectSavedDir() / TEXT("Rework");
}

void UUUTHUBEditorUtils::ExportsActors(const TArray<UObject*>& ActorsToExports)
{
    const FAssetToolsModule& Module = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");    
    Module.Get().ExportAssets(ActorsToExports, GetExportDirRework());
}

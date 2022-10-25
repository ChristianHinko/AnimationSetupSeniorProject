// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibraries/ASBlueprintFunctionLibrary_SkeletalMeshHelpers.h"

#include "Engine/SkeletalMeshSocket.h"



USkeletalMesh* UASBlueprintFunctionLibrary_SkeletalMeshHelpers::CreateSkinlessSkeletalMeshFromSkeleton(USkeleton* InSkeleton)
{
	if (!IsValid(InSkeleton))
	{
		UE_LOG(LogASSkinlessSkeletalMesh, Error, TEXT("%s() could not create skinless Skeletal Mesh because the passed in InSkeleton was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return nullptr;
	}

	UObject* const Outer = GetTransientPackage();
	const FName ObjectName = MakeUniqueObjectName(Outer, USkeletalMesh::StaticClass(), TEXT("SkinlessSkeletalMesh"));
	const EObjectFlags ObjectObjectFlags = EObjectFlags::RF_Transient | EObjectFlags::RF_Public; // use public because intend to use this outside of the transient package - also because we don't references to get nulled by FArchiveReplaceOrClearExternalReferences::operator<<()

	USkeletalMesh* SkinlessSkeletalMesh = NewObject<USkeletalMesh>(Outer, ObjectName, ObjectObjectFlags);
	InitializeSkinlessSkeletalMeshFromSkeleton(SkinlessSkeletalMesh, InSkeleton);

	if (!GIsEditor)
	{
		// Transient has no files to stream from
		SkinlessSkeletalMesh->NeverStream = true;
	}

	return SkinlessSkeletalMesh;
}

void UASBlueprintFunctionLibrary_SkeletalMeshHelpers::InitializeSkinlessSkeletalMeshFromSkeleton(USkeletalMesh* InOutSkeletalMesh, USkeleton* InSkeleton)
{
	if (!IsValid(InOutSkeletalMesh))
	{
		UE_LOG(LogASSkinlessSkeletalMesh, Error, TEXT("%s() could not initialize because the passed in InOutSkeletalMesh was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}
	if (!IsValid(InSkeleton))
	{
		UE_LOG(LogASSkinlessSkeletalMesh, Error, TEXT("%s() could not initialize because the passed in InSkeleton was NULL"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	// Copy Reference Skeleton
	InOutSkeletalMesh->SetRefSkeleton(InSkeleton->GetReferenceSkeleton());

	// Copy Sockets
	for (const USkeletalMeshSocket* SkeletonSocket : InSkeleton->Sockets)
	{
		USkeletalMeshSocket* SkeletonSocketCopy = Cast<USkeletalMeshSocket>(StaticDuplicateObject(SkeletonSocket, InOutSkeletalMesh));
		InOutSkeletalMesh->GetMeshOnlySocketList().Add(SkeletonSocketCopy);
	}
	InOutSkeletalMesh->RebuildSocketMap();

	// Add LOD info - use values from UnFbx::FFbxImporter::ImportSkeletalMesh()
	{
		InOutSkeletalMesh->ResetLODInfo();
		FSkeletalMeshLODInfo& NewLODInfo = InOutSkeletalMesh->AddLODInfo();
		NewLODInfo.ReductionSettings.NumOfTrianglesPercentage = 1.f;
		NewLODInfo.ReductionSettings.NumOfVertPercentage = 1.f;
		NewLODInfo.ReductionSettings.MaxDeviationPercentage = 0.f;
		NewLODInfo.LODHysteresis = .02f;
	}

	// Calculate and set bounds
	{
		TArray<FVector> BoneLocations;

		for (const FTransform& BoneTransform : InSkeleton->GetReferenceSkeleton().GetRefBonePose())
		{
			BoneLocations.Add(BoneTransform.GetLocation());
		}

		FBox3d BoundingBox = FBox3d(BoneLocations);
		InOutSkeletalMesh->SetImportedBounds(FBoxSphereBounds(BoundingBox));
	}

	// Calculate inverse reference pose matrices
	InOutSkeletalMesh->CalculateInvRefMatrices();

	// Build render data
	{
		InOutSkeletalMesh->AllocateResourceForRendering();
		FSkeletalMeshLODRenderData* SkeletalMeshLODRenderData = new FSkeletalMeshLODRenderData();


		// Add all Skeleton bones as required bones for animation
		const TArray<FMeshBoneInfo>& RefBoneInfo = InSkeleton->GetReferenceSkeleton().GetRefBoneInfo();
		for (FBoneIndexType i = 0; i < RefBoneInfo.Num(); ++i)
		{
			SkeletalMeshLODRenderData->RequiredBones.Add(i);
		}

		// Give us a vertex in order to be able to pass FSkeletalMeshRenderData::GetFirstValidLODIdx()
		SkeletalMeshLODRenderData->StaticVertexBuffers.PositionVertexBuffer.Init(TArray<FVector3f>({ FVector3f(0.f, 0.f, 0.f) }));


		InOutSkeletalMesh->GetResourceForRendering()->LODRenderData.Add(SkeletalMeshLODRenderData);
	}

	// Set Skeleton
	InOutSkeletalMesh->SetSkeleton(InSkeleton);

	// Initialize for rendering
	InOutSkeletalMesh->InitResources();
}

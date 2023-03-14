// Copyright Augmenta 2023, All Rights Reserved.


#include "LiveLinkAugmentaClusterManager.h"

#include "LiveLinkAugmentaManager.h"

#include "DisplayCluster/Public/Blueprints/DisplayClusterBlueprintLib.h"

// Sets default values
ALiveLinkAugmentaClusterManager::ALiveLinkAugmentaClusterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AugmentaManager = nullptr;

	bInitialized = false;
}

// Called when the game starts or when spawned
void ALiveLinkAugmentaClusterManager::BeginPlay()
{
	Super::BeginPlay();

	//Get DisplayClusterAPI
	UDisplayClusterBlueprintLib::GetAPI(DisplayClusterAPI);

	//Bind to cluster event listener
	DisplayClusterAPI->AddClusterEventListener(this);

	//Bind to Augmenta Manager events
	if(ensure(AugmentaManager))
	{
		AugmentaManager->OnAugmentaSceneUpdated.AddDynamic(this, &ALiveLinkAugmentaClusterManager::SendSceneUpdatedClusterEvent);
		AugmentaManager->OnAugmentaVideoOutputUpdated.AddDynamic(this, &ALiveLinkAugmentaClusterManager::SendVideoOutputUpdatedClusterEvent);
		AugmentaManager->OnAugmentaObjectEntered.AddDynamic(this, &ALiveLinkAugmentaClusterManager::SendObjectEnteredClusterEvent);
		AugmentaManager->OnAugmentaObjectUpdated.AddDynamic(this, &ALiveLinkAugmentaClusterManager::SendObjectUpdatedClusterEvent);
		AugmentaManager->OnAugmentaObjectLeft.AddDynamic(this, &ALiveLinkAugmentaClusterManager::SendObjectLeftClusterEvent);
		AugmentaManager->OnAugmentaSourceDestroyed.AddDynamic(this, &ALiveLinkAugmentaClusterManager::SendSourceDestroyedClusterEvent);

		bInitialized = true;
		UE_LOG(LogTemp, Log, TEXT("Augmenta Cluster Manager bound to Augmenta Manager successfully."));
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Augmenta Manager reference is invalid in Augmenta Cluster Manager !"));
	}

}

void ALiveLinkAugmentaClusterManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//Unbind from cluster event listener
	DisplayClusterAPI->RemoveClusterEventListener(this);

	//Unbind from Augmenta Manager
	if (AugmentaManager && bInitialized)
	{
		AugmentaManager->OnAugmentaSceneUpdated.RemoveDynamic(this, &ALiveLinkAugmentaClusterManager::SendSceneUpdatedClusterEvent);
		AugmentaManager->OnAugmentaVideoOutputUpdated.RemoveDynamic(this, &ALiveLinkAugmentaClusterManager::SendVideoOutputUpdatedClusterEvent);
		AugmentaManager->OnAugmentaObjectEntered.RemoveDynamic(this, &ALiveLinkAugmentaClusterManager::SendObjectEnteredClusterEvent);
		AugmentaManager->OnAugmentaObjectUpdated.RemoveDynamic(this, &ALiveLinkAugmentaClusterManager::SendObjectUpdatedClusterEvent);
		AugmentaManager->OnAugmentaObjectLeft.RemoveDynamic(this, &ALiveLinkAugmentaClusterManager::SendObjectLeftClusterEvent);
		AugmentaManager->OnAugmentaSourceDestroyed.RemoveDynamic(this, &ALiveLinkAugmentaClusterManager::SendSourceDestroyedClusterEvent);
	}

}

void ALiveLinkAugmentaClusterManager::SendSceneUpdatedClusterEvent(const FLiveLinkAugmentaScene AugmentaScene)
{
	FDisplayClusterClusterEventJson Event;

	Event.Name = "SceneUpdated";
	Event.Type = "-1";
	Event.Category = "Augmenta";
	Event.Parameters = SerializeAugmentaScene(AugmentaScene);

	DisplayClusterAPI->EmitClusterEventJson(Event, true);
}

void ALiveLinkAugmentaClusterManager::SendVideoOutputUpdatedClusterEvent(const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput)
{
	UE_LOG(LogTemp, Log, TEXT("Video Output Updated"));
}

void ALiveLinkAugmentaClusterManager::SendObjectEnteredClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	UE_LOG(LogTemp, Log, TEXT("Object Entered"));
}

void ALiveLinkAugmentaClusterManager::SendObjectUpdatedClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	UE_LOG(LogTemp, Log, TEXT("Object Updated"));
}

void ALiveLinkAugmentaClusterManager::SendObjectLeftClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	UE_LOG(LogTemp, Log, TEXT("Object Left"));
}

void ALiveLinkAugmentaClusterManager::SendSourceDestroyedClusterEvent()
{
	UE_LOG(LogTemp, Log, TEXT("Source Destroyed"));
}

TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeAugmentaScene(const FLiveLinkAugmentaScene AugmentaScene)
{
	TMap<FString, FString> JSonMap;

	JSonMap.Add("Frame", FString::FromInt(AugmentaScene.Frame));
	JSonMap.Add("ObjectCount", FString::FromInt(AugmentaScene.ObjectCount));
	JSonMap.Add("SizeX", FString::SanitizeFloat(AugmentaScene.Size.X));
	JSonMap.Add("SizeY", FString::SanitizeFloat(AugmentaScene.Size.Y));
	JSonMap.Add("PositionX", FString::SanitizeFloat(AugmentaScene.Position.X));
	JSonMap.Add("PositionY", FString::SanitizeFloat(AugmentaScene.Position.Y));
	JSonMap.Add("PositionZ", FString::SanitizeFloat(AugmentaScene.Position.Z));
	JSonMap.Add("RotationX", FString::SanitizeFloat(AugmentaScene.Rotation.X));
	JSonMap.Add("RotationY", FString::SanitizeFloat(AugmentaScene.Rotation.Y));
	JSonMap.Add("RotationZ", FString::SanitizeFloat(AugmentaScene.Rotation.Z));
	JSonMap.Add("RotationW", FString::SanitizeFloat(AugmentaScene.Rotation.W));
	JSonMap.Add("ScaleX", FString::SanitizeFloat(AugmentaScene.Scale.X));
	JSonMap.Add("ScaleY", FString::SanitizeFloat(AugmentaScene.Scale.Y));
	JSonMap.Add("ScaleZ", FString::SanitizeFloat(AugmentaScene.Scale.Z));

	return JSonMap;
}

FLiveLinkAugmentaScene ALiveLinkAugmentaClusterManager::DeserializeAugmentaScene(const TMap<FString, FString> JSonMap)
{
	FLiveLinkAugmentaScene AugmentaScene;

	FString TmpString = *JSonMap.Find("Frame");	AugmentaScene.Frame = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("ObjectCount");	AugmentaScene.ObjectCount = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("SizeX");			AugmentaScene.Size.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("SizeY");			AugmentaScene.Size.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionX");		AugmentaScene.Position.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionY");		AugmentaScene.Position.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionZ");		AugmentaScene.Position.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationX");		AugmentaScene.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationY");		AugmentaScene.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationZ");		AugmentaScene.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationW");		AugmentaScene.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleX");			AugmentaScene.Scale.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleY");			AugmentaScene.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleZ");			AugmentaScene.Scale.Z = FCString::Atod(*TmpString);

	return AugmentaScene;
}

void ALiveLinkAugmentaClusterManager::OnClusterEventJson_Implementation(const FDisplayClusterClusterEventJson& Event)
{
	if(Event.Name == "SceneUpdated")
	{
		OnAugmentaSceneUpdatedCluster.Broadcast(DeserializeAugmentaScene(Event.Parameters));
	}
}


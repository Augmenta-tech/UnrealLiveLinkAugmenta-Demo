// Copyright Augmenta 2023, All Rights Reserved.


#include "LiveLinkAugmentaClusterManager.h"

#include "LiveLinkAugmentaManager.h"

#include "Cluster/IDisplayClusterClusterManager.h"
#include "DisplayCluster/Public/IDisplayCluster.h"

// Sets default values
ALiveLinkAugmentaClusterManager::ALiveLinkAugmentaClusterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AugmentaManager = nullptr;
	ClusterManager = nullptr;

	bInitialized = false;
}

// Called when the game starts or when spawned
void ALiveLinkAugmentaClusterManager::BeginPlay()
{
	Super::BeginPlay();

	//Get DisplayClusterManager
	ClusterManager = IDisplayCluster::Get().GetClusterMgr();

	//Bind to cluster event listener
	if (ensure(ClusterManager)) {
		ClusterManager->AddClusterEventListener(this);
	}

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
	if (ClusterManager) {
		ClusterManager->RemoveClusterEventListener(this);
	}

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
	Event.Parameters = SerializeJsonAugmentaScene(AugmentaScene);
	Event.bIsSystemEvent = false;
	Event.bShouldDiscardOnRepeat = true;

	ClusterManager->EmitClusterEventJson(Event, true);

	//FString message = "SendingClusterMessage : " + Event.Name;
	//GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Blue, message);
}

void ALiveLinkAugmentaClusterManager::SendVideoOutputUpdatedClusterEvent(const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput)
{
	FDisplayClusterClusterEventJson Event;

	Event.Name = "VideoOutputUpdated";
	Event.Type = "-2";
	Event.Category = "Augmenta";
	Event.Parameters = SerializeJsonAugmentaVideoOutput(AugmentaVideoOutput);
	Event.bIsSystemEvent = false;
	Event.bShouldDiscardOnRepeat = true;

	ClusterManager->EmitClusterEventJson(Event, true);
}

void ALiveLinkAugmentaClusterManager::SendObjectEnteredClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	FDisplayClusterClusterEventJson Event;

	Event.Name = "ObjectEntered";
	Event.Type = FString::FromInt(AugmentaObject.Id);
	Event.Category = "Augmenta";
	Event.Parameters = SerializeJsonAugmentaObject(AugmentaObject);
	Event.bIsSystemEvent = false;
	Event.bShouldDiscardOnRepeat = true;

	ClusterManager->EmitClusterEventJson(Event, true);
}

void ALiveLinkAugmentaClusterManager::SendObjectUpdatedClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	FDisplayClusterClusterEventJson Event;

	Event.Name = "ObjectUpdated";
	Event.Type = FString::FromInt(AugmentaObject.Id);
	Event.Category = "Augmenta";
	Event.Parameters = SerializeJsonAugmentaObject(AugmentaObject);
	Event.bIsSystemEvent = false;
	Event.bShouldDiscardOnRepeat = true;

	ClusterManager->EmitClusterEventJson(Event, true);
}

void ALiveLinkAugmentaClusterManager::SendObjectLeftClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	FDisplayClusterClusterEventJson Event;

	Event.Name = "ObjectLeft";
	Event.Type = FString::FromInt(AugmentaObject.Id);
	Event.Category = "Augmenta";
	Event.Parameters = SerializeJsonAugmentaObject(AugmentaObject);
	Event.bIsSystemEvent = false;
	Event.bShouldDiscardOnRepeat = true;

	ClusterManager->EmitClusterEventJson(Event, true);
}

void ALiveLinkAugmentaClusterManager::SendSourceDestroyedClusterEvent()
{
	FDisplayClusterClusterEventJson Event;

	Event.Name = "SourceDestroyed";
	Event.Type = "";
	Event.Category = "Augmenta";
	Event.bIsSystemEvent = false;
	Event.bShouldDiscardOnRepeat = true;

	ClusterManager->EmitClusterEventJson(Event, true);
}

void ALiveLinkAugmentaClusterManager::OnClusterEventJson_Implementation(const FDisplayClusterClusterEventJson& Event)
{

	//FString message = "ClusterMessageReceived : " + Event.Name;
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::White, message);

	if (Event.Name == "SceneUpdated")
	{
		OnAugmentaSceneUpdatedCluster.Broadcast(DeserializeJsonAugmentaScene(Event.Parameters));
	} else if(Event.Name == "VideoOutputUpdated")
	{
		OnAugmentaVideoOutputUpdatedCluster.Broadcast(DeserializeJsonAugmentaVideoOutput(Event.Parameters));
	}
	else if (Event.Name == "ObjectEntered")
	{
		OnAugmentaObjectEnteredCluster.Broadcast(DeserializeJsonAugmentaObject(Event.Parameters));
	}
	else if (Event.Name == "ObjectUpdated")
	{
		OnAugmentaObjectUpdatedCluster.Broadcast(DeserializeJsonAugmentaObject(Event.Parameters));
	}
	else if (Event.Name == "ObjectLeft")
	{
		OnAugmentaObjectLeftCluster.Broadcast(DeserializeJsonAugmentaObject(Event.Parameters));
	}
	else if (Event.Name == "SourceDestroyed")
	{
		OnAugmentaSourceDestroyedCluster.Broadcast();
	}
}



TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeJsonAugmentaScene(const FLiveLinkAugmentaScene AugmentaScene)
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

FLiveLinkAugmentaScene ALiveLinkAugmentaClusterManager::DeserializeJsonAugmentaScene(const TMap<FString, FString> JSonMap)
{
	FLiveLinkAugmentaScene AugmentaScene;

	FString TmpString = *JSonMap.Find("Frame");	AugmentaScene.Frame = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("ObjectCount"); AugmentaScene.ObjectCount = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("SizeX");	AugmentaScene.Size.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("SizeY");	AugmentaScene.Size.Y = FCString::Atod(*TmpString);

	TmpString = *JSonMap.Find("PositionX");	AugmentaScene.Position.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionY");	AugmentaScene.Position.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionZ");	AugmentaScene.Position.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationX");	AugmentaScene.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationY");	AugmentaScene.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationZ");	AugmentaScene.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationW");	AugmentaScene.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleX"); AugmentaScene.Scale.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleY");	 AugmentaScene.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleZ");	 AugmentaScene.Scale.Z = FCString::Atod(*TmpString);

	return AugmentaScene;
}

TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeJsonAugmentaVideoOutput(
	const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput)
{
	TMap<FString, FString> JSonMap;

	JSonMap.Add("OffsetX", FString::SanitizeFloat(AugmentaVideoOutput.Offset.X));
	JSonMap.Add("OffsetY", FString::SanitizeFloat(AugmentaVideoOutput.Offset.Y));
	JSonMap.Add("SizeX", FString::SanitizeFloat(AugmentaVideoOutput.Size.X));
	JSonMap.Add("SizeY", FString::SanitizeFloat(AugmentaVideoOutput.Size.Y));
	JSonMap.Add("ResolutionX", FString::FromInt(AugmentaVideoOutput.Resolution.X));
	JSonMap.Add("ResolutionY", FString::FromInt(AugmentaVideoOutput.Resolution.Y));

	JSonMap.Add("PositionX", FString::SanitizeFloat(AugmentaVideoOutput.Position.X));
	JSonMap.Add("PositionY", FString::SanitizeFloat(AugmentaVideoOutput.Position.Y));
	JSonMap.Add("PositionZ", FString::SanitizeFloat(AugmentaVideoOutput.Position.Z));
	JSonMap.Add("RotationX", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.X));
	JSonMap.Add("RotationY", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.Y));
	JSonMap.Add("RotationZ", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.Z));
	JSonMap.Add("RotationW", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.W));
	JSonMap.Add("ScaleX", FString::SanitizeFloat(AugmentaVideoOutput.Scale.X));
	JSonMap.Add("ScaleY", FString::SanitizeFloat(AugmentaVideoOutput.Scale.Y));
	JSonMap.Add("ScaleZ", FString::SanitizeFloat(AugmentaVideoOutput.Scale.Z));

	return JSonMap;
}

FLiveLinkAugmentaVideoOutput ALiveLinkAugmentaClusterManager::DeserializeJsonAugmentaVideoOutput(
	const TMap<FString, FString> JSonMap)
{
	FLiveLinkAugmentaVideoOutput AugmentaVideoOutput;

	FString TmpString = *JSonMap.Find("OffsetX"); AugmentaVideoOutput.Offset.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("OffsetY"); AugmentaVideoOutput.Offset.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("SizeX");	AugmentaVideoOutput.Size.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("SizeY");	AugmentaVideoOutput.Size.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ResolutionX"); AugmentaVideoOutput.Resolution.X = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("ResolutionY"); AugmentaVideoOutput.Resolution.Y = FCString::Atoi(*TmpString);

	TmpString = *JSonMap.Find("PositionX");	AugmentaVideoOutput.Position.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionY");	AugmentaVideoOutput.Position.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionZ");	AugmentaVideoOutput.Position.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationX");	AugmentaVideoOutput.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationY");	AugmentaVideoOutput.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationZ");	AugmentaVideoOutput.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationW");	AugmentaVideoOutput.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleX"); AugmentaVideoOutput.Scale.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleY");	 AugmentaVideoOutput.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleZ");	 AugmentaVideoOutput.Scale.Z = FCString::Atod(*TmpString);

	return AugmentaVideoOutput;
}

TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeJsonAugmentaObject(
	const FLiveLinkAugmentaObject AugmentaObject)
{
	TMap<FString, FString> JSonMap;

	JSonMap.Add("Frame", FString::FromInt(AugmentaObject.Frame));
	JSonMap.Add("Id", FString::FromInt(AugmentaObject.Id));
	JSonMap.Add("Oid", FString::FromInt(AugmentaObject.Oid));
	JSonMap.Add("Age", FString::SanitizeFloat(AugmentaObject.Age));
	JSonMap.Add("CentroidX", FString::SanitizeFloat(AugmentaObject.Centroid.X));
	JSonMap.Add("CentroidY", FString::SanitizeFloat(AugmentaObject.Centroid.Y));
	JSonMap.Add("VelocityX", FString::SanitizeFloat(AugmentaObject.Velocity.X));
	JSonMap.Add("VelocityY", FString::SanitizeFloat(AugmentaObject.Velocity.Y));
	JSonMap.Add("Orientation", FString::SanitizeFloat(AugmentaObject.Orientation));
	JSonMap.Add("BoundingRectPosX", FString::SanitizeFloat(AugmentaObject.BoundingRectPos.X));
	JSonMap.Add("BoundingRectPosY", FString::SanitizeFloat(AugmentaObject.BoundingRectPos.Y));
	JSonMap.Add("BoundingRectSizeX", FString::SanitizeFloat(AugmentaObject.BoundingRectSize.X));
	JSonMap.Add("BoundingRectSizeY", FString::SanitizeFloat(AugmentaObject.BoundingRectSize.Y));
	JSonMap.Add("BoundingRectRotation", FString::SanitizeFloat(AugmentaObject.BoundingRectRotation));
	JSonMap.Add("Height", FString::SanitizeFloat(AugmentaObject.Height));
	JSonMap.Add("HighestX", FString::SanitizeFloat(AugmentaObject.Highest.X));
	JSonMap.Add("HighestY", FString::SanitizeFloat(AugmentaObject.Highest.Y));
	JSonMap.Add("Distance", FString::SanitizeFloat(AugmentaObject.Distance));
	JSonMap.Add("Reflectivity", FString::SanitizeFloat(AugmentaObject.Reflectivity));
	JSonMap.Add("LastUpdateTimeYear", FString::FromInt(AugmentaObject.LastUpdateTime.GetYear()));
	JSonMap.Add("LastUpdateTimeMonth", FString::FromInt(AugmentaObject.LastUpdateTime.GetMonth()));
	JSonMap.Add("LastUpdateTimeDay", FString::FromInt(AugmentaObject.LastUpdateTime.GetDay()));
	JSonMap.Add("LastUpdateTimeHour", FString::FromInt(AugmentaObject.LastUpdateTime.GetHour()));
	JSonMap.Add("LastUpdateTimeMinute", FString::FromInt(AugmentaObject.LastUpdateTime.GetMinute()));
	JSonMap.Add("LastUpdateTimeSecond", FString::FromInt(AugmentaObject.LastUpdateTime.GetSecond()));
	JSonMap.Add("LastUpdateTimeMillisecond", FString::FromInt(AugmentaObject.LastUpdateTime.GetMillisecond()));

	JSonMap.Add("PositionX", FString::SanitizeFloat(AugmentaObject.Position.X));
	JSonMap.Add("PositionY", FString::SanitizeFloat(AugmentaObject.Position.Y));
	JSonMap.Add("PositionZ", FString::SanitizeFloat(AugmentaObject.Position.Z));
	JSonMap.Add("RotationX", FString::SanitizeFloat(AugmentaObject.Rotation.X));
	JSonMap.Add("RotationY", FString::SanitizeFloat(AugmentaObject.Rotation.Y));
	JSonMap.Add("RotationZ", FString::SanitizeFloat(AugmentaObject.Rotation.Z));
	JSonMap.Add("RotationW", FString::SanitizeFloat(AugmentaObject.Rotation.W));
	JSonMap.Add("ScaleX", FString::SanitizeFloat(AugmentaObject.Scale.X));
	JSonMap.Add("ScaleY", FString::SanitizeFloat(AugmentaObject.Scale.Y));
	JSonMap.Add("ScaleZ", FString::SanitizeFloat(AugmentaObject.Scale.Z));

	return JSonMap;
}

FLiveLinkAugmentaObject ALiveLinkAugmentaClusterManager::DeserializeJsonAugmentaObject(
	const TMap<FString, FString> JSonMap)
{
	FLiveLinkAugmentaObject AugmentaObject;

	FString TmpString = *JSonMap.Find("Frame");	AugmentaObject.Frame = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("Id");	 AugmentaObject.Id = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("Oid"); AugmentaObject.Oid = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("Age"); AugmentaObject.Age = FCString::Atof(*TmpString);
	TmpString = *JSonMap.Find("CentroidX");	AugmentaObject.Centroid.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("CentroidY");	AugmentaObject.Centroid.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("VelocityX");	AugmentaObject.Velocity.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("VelocityY");	AugmentaObject.Velocity.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("Orientation"); AugmentaObject.Orientation = FCString::Atof(*TmpString);
	TmpString = *JSonMap.Find("BoundingRectPosX"); AugmentaObject.BoundingRectPos.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("BoundingRectPosY"); AugmentaObject.BoundingRectPos.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("BoundingRectSizeX"); AugmentaObject.BoundingRectSize.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("BoundingRectSizeY");	AugmentaObject.BoundingRectSize.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("BoundingRectRotation"); AugmentaObject.BoundingRectRotation = FCString::Atof(*TmpString);
	TmpString = *JSonMap.Find("Height"); AugmentaObject.Height = FCString::Atof(*TmpString);
	TmpString = *JSonMap.Find("HighestX"); AugmentaObject.Highest.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("HighestY"); AugmentaObject.Highest.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("Distance"); AugmentaObject.Distance = FCString::Atof(*TmpString);
	TmpString = *JSonMap.Find("Reflectivity"); AugmentaObject.Reflectivity = FCString::Atof(*TmpString);

	TmpString = *JSonMap.Find("LastUpdateTimeYear"); int Year = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("LastUpdateTimeMonth"); int Month = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("LastUpdateTimeDay"); int Day = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("LastUpdateTimeHour"); int Hour = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("LastUpdateTimeMinute"); int Minute = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("LastUpdateTimeSecond"); int Second = FCString::Atoi(*TmpString);
	TmpString = *JSonMap.Find("LastUpdateTimeMillisecond"); int Millisecond = FCString::Atoi(*TmpString);
	FDateTime LastUpdateTime{ Year, Month, Day, Hour, Minute, Second, Millisecond };
	AugmentaObject.LastUpdateTime = LastUpdateTime;

	TmpString = *JSonMap.Find("PositionX");		AugmentaObject.Position.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionY");		AugmentaObject.Position.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("PositionZ");		AugmentaObject.Position.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationX");		AugmentaObject.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationY");		AugmentaObject.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationZ");		AugmentaObject.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("RotationW");		AugmentaObject.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleX");			AugmentaObject.Scale.X = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleY");			AugmentaObject.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *JSonMap.Find("ScaleZ");			AugmentaObject.Scale.Z = FCString::Atod(*TmpString);

	return AugmentaObject;
}


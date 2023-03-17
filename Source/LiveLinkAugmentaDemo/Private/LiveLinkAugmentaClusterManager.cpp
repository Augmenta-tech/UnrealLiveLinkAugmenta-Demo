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
	bUseBinaryClusterEvents = true;
	BinaryEventIdOffset = 0;
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
	if(bUseBinaryClusterEvents)
	{
		FDisplayClusterClusterEventBinary Event;

		Event.EventId = 1 + BinaryEventIdOffset;
		Event.EventData = SerializeBinaryAugmentaScene(AugmentaScene);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventBinary(Event, true);
	}
	else {
		FDisplayClusterClusterEventJson Event;

		Event.Name = "SceneUpdated";
		Event.Type = "-1";
		Event.Category = "Augmenta";
		Event.Parameters = SerializeJsonAugmentaScene(AugmentaScene);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventJson(Event, true);
	}

	//FString message = "SendingClusterMessage : " + Event.Name;
	//GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Blue, message);
}

void ALiveLinkAugmentaClusterManager::SendVideoOutputUpdatedClusterEvent(const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput)
{
	if (bUseBinaryClusterEvents)
	{
		FDisplayClusterClusterEventBinary Event;

		Event.EventId = 2 + BinaryEventIdOffset;
		Event.EventData = SerializeBinaryAugmentaVideoOutput(AugmentaVideoOutput);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventBinary(Event, true);
	}
	else {
		FDisplayClusterClusterEventJson Event;

		Event.Name = "VideoOutputUpdated";
		Event.Type = "-2";
		Event.Category = "Augmenta";
		Event.Parameters = SerializeJsonAugmentaVideoOutput(AugmentaVideoOutput);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventJson(Event, true);
	}
}

void ALiveLinkAugmentaClusterManager::SendObjectEnteredClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	if (bUseBinaryClusterEvents)
	{
		FDisplayClusterClusterEventBinary Event;

		Event.EventId = 3 + BinaryEventIdOffset;
		Event.EventData = SerializeBinaryAugmentaObject(AugmentaObject);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = false;

		ClusterManager->EmitClusterEventBinary(Event, true);
	}
	else {
		FDisplayClusterClusterEventJson Event;

		Event.Name = "ObjectEntered";
		Event.Type = FString::FromInt(AugmentaObject.Id);
		Event.Category = "Augmenta";
		Event.Parameters = SerializeJsonAugmentaObject(AugmentaObject);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventJson(Event, true);
	}
}

void ALiveLinkAugmentaClusterManager::SendObjectUpdatedClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	if(bUseBinaryClusterEvents)
	{
		FDisplayClusterClusterEventBinary Event;

		Event.EventId = 4 + BinaryEventIdOffset;
		Event.EventData = SerializeBinaryAugmentaObject(AugmentaObject);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = false;

		ClusterManager->EmitClusterEventBinary(Event, true);
	}
	else {
		FDisplayClusterClusterEventJson Event;

		Event.Name = "ObjectUpdated";
		Event.Type = FString::FromInt(AugmentaObject.Id);
		Event.Category = "Augmenta";
		Event.Parameters = SerializeJsonAugmentaObject(AugmentaObject);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventJson(Event, true);
	}
}

void ALiveLinkAugmentaClusterManager::SendObjectLeftClusterEvent(const FLiveLinkAugmentaObject AugmentaObject)
{
	if(bUseBinaryClusterEvents)
	{
		FDisplayClusterClusterEventBinary Event;

		Event.EventId = 5 + BinaryEventIdOffset;
		Event.EventData = SerializeBinaryAugmentaObject(AugmentaObject);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = false;

		ClusterManager->EmitClusterEventBinary(Event, true);
	}
	else {
		FDisplayClusterClusterEventJson Event;

		Event.Name = "ObjectLeft";
		Event.Type = FString::FromInt(AugmentaObject.Id);
		Event.Category = "Augmenta";
		Event.Parameters = SerializeJsonAugmentaObject(AugmentaObject);
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventJson(Event, true);
	}
}

void ALiveLinkAugmentaClusterManager::SendSourceDestroyedClusterEvent()
{
	if (bUseBinaryClusterEvents)
	{
		FDisplayClusterClusterEventBinary Event;

		Event.EventId = BinaryEventIdOffset;
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventBinary(Event, true);
	}
	else {
		FDisplayClusterClusterEventJson Event;

		Event.Name = "SourceDestroyed";
		Event.Type = "";
		Event.Category = "Augmenta";
		Event.bIsSystemEvent = false;
		Event.bShouldDiscardOnRepeat = true;

		ClusterManager->EmitClusterEventJson(Event, true);
	}
}

void ALiveLinkAugmentaClusterManager::OnClusterEventJson_Implementation(const FDisplayClusterClusterEventJson& Event)
{

	//FString message = "ClusterMessageReceived : " + Event.Name;
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::White, message);

	//TRACE_CPUPROFILER_EVENT_SCOPE_STR("AugmentaClusterManager OnClusterEventJson");

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

void ALiveLinkAugmentaClusterManager::OnClusterEventBinary_Implementation(
	const FDisplayClusterClusterEventBinary& Event)
{
	//FString message = "BinaryClusterMessageReceived : " + FString::FromInt(Event.EventId);
	//GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::White, message);

	//TRACE_CPUPROFILER_EVENT_SCOPE_STR("AugmentaClusterManager OnClusterEventBinary");

	if (Event.EventId == BinaryEventIdOffset)
	{
		OnAugmentaSourceDestroyedCluster.Broadcast();
	}
	else if(Event.EventId == BinaryEventIdOffset + 1)
	{
		OnAugmentaSceneUpdatedCluster.Broadcast(DeserializeBinaryAugmentaScene(Event.EventData));
	}
	else if (Event.EventId == BinaryEventIdOffset + 2)
	{
		OnAugmentaVideoOutputUpdatedCluster.Broadcast(DeserializeBinaryAugmentaVideoOutput(Event.EventData));
	}
	else if (Event.EventId == BinaryEventIdOffset + 3)
	{
		OnAugmentaObjectEnteredCluster.Broadcast(DeserializeBinaryAugmentaObject(Event.EventData));
	}
	else if (Event.EventId == BinaryEventIdOffset + 4)
	{
		OnAugmentaObjectUpdatedCluster.Broadcast(DeserializeBinaryAugmentaObject(Event.EventData));
	}
	else if (Event.EventId == BinaryEventIdOffset + 5)
	{
		OnAugmentaObjectLeftCluster.Broadcast(DeserializeBinaryAugmentaObject(Event.EventData));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Json Serialization
//////////////////////////////////////////////////////////////////////////////////////////////

TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeJsonAugmentaScene(const FLiveLinkAugmentaScene AugmentaScene)
{
	TMap<FString, FString> EventData;

	EventData.Add("Frame", FString::FromInt(AugmentaScene.Frame));
	EventData.Add("ObjectCount", FString::FromInt(AugmentaScene.ObjectCount));
	EventData.Add("SizeX", FString::SanitizeFloat(AugmentaScene.Size.X));
	EventData.Add("SizeY", FString::SanitizeFloat(AugmentaScene.Size.Y));

	EventData.Add("PositionX", FString::SanitizeFloat(AugmentaScene.Position.X));
	EventData.Add("PositionY", FString::SanitizeFloat(AugmentaScene.Position.Y));
	EventData.Add("PositionZ", FString::SanitizeFloat(AugmentaScene.Position.Z));
	EventData.Add("RotationX", FString::SanitizeFloat(AugmentaScene.Rotation.X));
	EventData.Add("RotationY", FString::SanitizeFloat(AugmentaScene.Rotation.Y));
	EventData.Add("RotationZ", FString::SanitizeFloat(AugmentaScene.Rotation.Z));
	EventData.Add("RotationW", FString::SanitizeFloat(AugmentaScene.Rotation.W));
	EventData.Add("ScaleX", FString::SanitizeFloat(AugmentaScene.Scale.X));
	EventData.Add("ScaleY", FString::SanitizeFloat(AugmentaScene.Scale.Y));
	EventData.Add("ScaleZ", FString::SanitizeFloat(AugmentaScene.Scale.Z));

	return EventData;
}

FLiveLinkAugmentaScene ALiveLinkAugmentaClusterManager::DeserializeJsonAugmentaScene(const TMap<FString, FString> EventData)
{
	FLiveLinkAugmentaScene AugmentaScene;

	FString TmpString = *EventData.Find("Frame");	AugmentaScene.Frame = FCString::Atoi(*TmpString);
	TmpString = *EventData.Find("ObjectCount"); AugmentaScene.ObjectCount = FCString::Atoi(*TmpString);
	TmpString = *EventData.Find("SizeX");	AugmentaScene.Size.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("SizeY");	AugmentaScene.Size.Y = FCString::Atod(*TmpString);

	TmpString = *EventData.Find("PositionX");	AugmentaScene.Position.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("PositionY");	AugmentaScene.Position.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("PositionZ");	AugmentaScene.Position.Z = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationX");	AugmentaScene.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationY");	AugmentaScene.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationZ");	AugmentaScene.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationW");	AugmentaScene.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleX"); AugmentaScene.Scale.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleY");	 AugmentaScene.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleZ");	 AugmentaScene.Scale.Z = FCString::Atod(*TmpString);

	return AugmentaScene;
}

TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeJsonAugmentaVideoOutput(
	const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput)
{
	TMap<FString, FString> EventData;

	EventData.Add("OffsetX", FString::SanitizeFloat(AugmentaVideoOutput.Offset.X));
	EventData.Add("OffsetY", FString::SanitizeFloat(AugmentaVideoOutput.Offset.Y));
	EventData.Add("SizeX", FString::SanitizeFloat(AugmentaVideoOutput.Size.X));
	EventData.Add("SizeY", FString::SanitizeFloat(AugmentaVideoOutput.Size.Y));
	EventData.Add("ResolutionX", FString::FromInt(AugmentaVideoOutput.Resolution.X));
	EventData.Add("ResolutionY", FString::FromInt(AugmentaVideoOutput.Resolution.Y));

	EventData.Add("PositionX", FString::SanitizeFloat(AugmentaVideoOutput.Position.X));
	EventData.Add("PositionY", FString::SanitizeFloat(AugmentaVideoOutput.Position.Y));
	EventData.Add("PositionZ", FString::SanitizeFloat(AugmentaVideoOutput.Position.Z));
	EventData.Add("RotationX", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.X));
	EventData.Add("RotationY", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.Y));
	EventData.Add("RotationZ", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.Z));
	EventData.Add("RotationW", FString::SanitizeFloat(AugmentaVideoOutput.Rotation.W));
	EventData.Add("ScaleX", FString::SanitizeFloat(AugmentaVideoOutput.Scale.X));
	EventData.Add("ScaleY", FString::SanitizeFloat(AugmentaVideoOutput.Scale.Y));
	EventData.Add("ScaleZ", FString::SanitizeFloat(AugmentaVideoOutput.Scale.Z));

	return EventData;
}

FLiveLinkAugmentaVideoOutput ALiveLinkAugmentaClusterManager::DeserializeJsonAugmentaVideoOutput(
	const TMap<FString, FString> EventData)
{
	FLiveLinkAugmentaVideoOutput AugmentaVideoOutput;

	FString TmpString = *EventData.Find("OffsetX"); AugmentaVideoOutput.Offset.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("OffsetY"); AugmentaVideoOutput.Offset.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("SizeX");	AugmentaVideoOutput.Size.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("SizeY");	AugmentaVideoOutput.Size.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ResolutionX"); AugmentaVideoOutput.Resolution.X = FCString::Atoi(*TmpString);
	TmpString = *EventData.Find("ResolutionY"); AugmentaVideoOutput.Resolution.Y = FCString::Atoi(*TmpString);

	TmpString = *EventData.Find("PositionX");	AugmentaVideoOutput.Position.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("PositionY");	AugmentaVideoOutput.Position.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("PositionZ");	AugmentaVideoOutput.Position.Z = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationX");	AugmentaVideoOutput.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationY");	AugmentaVideoOutput.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationZ");	AugmentaVideoOutput.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationW");	AugmentaVideoOutput.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleX"); AugmentaVideoOutput.Scale.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleY");	 AugmentaVideoOutput.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleZ");	 AugmentaVideoOutput.Scale.Z = FCString::Atod(*TmpString);

	return AugmentaVideoOutput;
}

TMap<FString, FString> ALiveLinkAugmentaClusterManager::SerializeJsonAugmentaObject(
	const FLiveLinkAugmentaObject AugmentaObject)
{
	TMap<FString, FString> EventData;

	EventData.Add("Id", FString::FromInt(AugmentaObject.Id));
	EventData.Add("Age", FString::SanitizeFloat(AugmentaObject.Age));
	EventData.Add("PositionX", FString::SanitizeFloat(AugmentaObject.Position.X));
	EventData.Add("PositionY", FString::SanitizeFloat(AugmentaObject.Position.Y));
	EventData.Add("PositionZ", FString::SanitizeFloat(AugmentaObject.Position.Z));
	EventData.Add("RotationX", FString::SanitizeFloat(AugmentaObject.Rotation.X));
	EventData.Add("RotationY", FString::SanitizeFloat(AugmentaObject.Rotation.Y));
	EventData.Add("RotationZ", FString::SanitizeFloat(AugmentaObject.Rotation.Z));
	EventData.Add("RotationW", FString::SanitizeFloat(AugmentaObject.Rotation.W));
	EventData.Add("ScaleX", FString::SanitizeFloat(AugmentaObject.Scale.X));
	EventData.Add("ScaleY", FString::SanitizeFloat(AugmentaObject.Scale.Y));
	EventData.Add("ScaleZ", FString::SanitizeFloat(AugmentaObject.Scale.Z));

	if (!bSendReducedObjectData) {
		EventData.Add("Frame", FString::FromInt(AugmentaObject.Frame));
		EventData.Add("Oid", FString::FromInt(AugmentaObject.Oid));
		EventData.Add("CentroidX", FString::SanitizeFloat(AugmentaObject.Centroid.X));
		EventData.Add("CentroidY", FString::SanitizeFloat(AugmentaObject.Centroid.Y));
		EventData.Add("VelocityX", FString::SanitizeFloat(AugmentaObject.Velocity.X));
		EventData.Add("VelocityY", FString::SanitizeFloat(AugmentaObject.Velocity.Y));
		EventData.Add("Orientation", FString::SanitizeFloat(AugmentaObject.Orientation));
		EventData.Add("BoundingRectPosX", FString::SanitizeFloat(AugmentaObject.BoundingRectPos.X));
		EventData.Add("BoundingRectPosY", FString::SanitizeFloat(AugmentaObject.BoundingRectPos.Y));
		EventData.Add("BoundingRectSizeX", FString::SanitizeFloat(AugmentaObject.BoundingRectSize.X));
		EventData.Add("BoundingRectSizeY", FString::SanitizeFloat(AugmentaObject.BoundingRectSize.Y));
		EventData.Add("BoundingRectRotation", FString::SanitizeFloat(AugmentaObject.BoundingRectRotation));
		EventData.Add("Height", FString::SanitizeFloat(AugmentaObject.Height));
		EventData.Add("HighestX", FString::SanitizeFloat(AugmentaObject.Highest.X));
		EventData.Add("HighestY", FString::SanitizeFloat(AugmentaObject.Highest.Y));
		EventData.Add("Distance", FString::SanitizeFloat(AugmentaObject.Distance));
		EventData.Add("Reflectivity", FString::SanitizeFloat(AugmentaObject.Reflectivity));
		EventData.Add("LastUpdateTimeYear", FString::FromInt(AugmentaObject.LastUpdateTime.GetYear()));
		EventData.Add("LastUpdateTimeMonth", FString::FromInt(AugmentaObject.LastUpdateTime.GetMonth()));
		EventData.Add("LastUpdateTimeDay", FString::FromInt(AugmentaObject.LastUpdateTime.GetDay()));
		EventData.Add("LastUpdateTimeHour", FString::FromInt(AugmentaObject.LastUpdateTime.GetHour()));
		EventData.Add("LastUpdateTimeMinute", FString::FromInt(AugmentaObject.LastUpdateTime.GetMinute()));
		EventData.Add("LastUpdateTimeSecond", FString::FromInt(AugmentaObject.LastUpdateTime.GetSecond()));
		EventData.Add("LastUpdateTimeMillisecond", FString::FromInt(AugmentaObject.LastUpdateTime.GetMillisecond()));
	}

	return EventData;
}

FLiveLinkAugmentaObject ALiveLinkAugmentaClusterManager::DeserializeJsonAugmentaObject(
	const TMap<FString, FString> EventData)
{
	FLiveLinkAugmentaObject AugmentaObject;

	FString TmpString = *EventData.Find("Id");	 AugmentaObject.Id = FCString::Atoi(*TmpString);
	TmpString = *EventData.Find("Age"); AugmentaObject.Age = FCString::Atof(*TmpString);
	TmpString = *EventData.Find("PositionX");		AugmentaObject.Position.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("PositionY");		AugmentaObject.Position.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("PositionZ");		AugmentaObject.Position.Z = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationX");		AugmentaObject.Rotation.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationY");		AugmentaObject.Rotation.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationZ");		AugmentaObject.Rotation.Z = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("RotationW");		AugmentaObject.Rotation.W = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleX");			AugmentaObject.Scale.X = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleY");			AugmentaObject.Scale.Y = FCString::Atod(*TmpString);
	TmpString = *EventData.Find("ScaleZ");			AugmentaObject.Scale.Z = FCString::Atod(*TmpString);

	if (!bSendReducedObjectData) {
		TmpString = *EventData.Find("Frame");	AugmentaObject.Frame = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("Oid"); AugmentaObject.Oid = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("CentroidX");	AugmentaObject.Centroid.X = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("CentroidY");	AugmentaObject.Centroid.Y = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("VelocityX");	AugmentaObject.Velocity.X = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("VelocityY");	AugmentaObject.Velocity.Y = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("Orientation"); AugmentaObject.Orientation = FCString::Atof(*TmpString);
		TmpString = *EventData.Find("BoundingRectPosX"); AugmentaObject.BoundingRectPos.X = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("BoundingRectPosY"); AugmentaObject.BoundingRectPos.Y = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("BoundingRectSizeX"); AugmentaObject.BoundingRectSize.X = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("BoundingRectSizeY");	AugmentaObject.BoundingRectSize.Y = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("BoundingRectRotation"); AugmentaObject.BoundingRectRotation = FCString::Atof(*TmpString);
		TmpString = *EventData.Find("Height"); AugmentaObject.Height = FCString::Atof(*TmpString);
		TmpString = *EventData.Find("HighestX"); AugmentaObject.Highest.X = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("HighestY"); AugmentaObject.Highest.Y = FCString::Atod(*TmpString);
		TmpString = *EventData.Find("Distance"); AugmentaObject.Distance = FCString::Atof(*TmpString);
		TmpString = *EventData.Find("Reflectivity"); AugmentaObject.Reflectivity = FCString::Atof(*TmpString);

		TmpString = *EventData.Find("LastUpdateTimeYear"); int Year = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("LastUpdateTimeMonth"); int Month = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("LastUpdateTimeDay"); int Day = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("LastUpdateTimeHour"); int Hour = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("LastUpdateTimeMinute"); int Minute = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("LastUpdateTimeSecond"); int Second = FCString::Atoi(*TmpString);
		TmpString = *EventData.Find("LastUpdateTimeMillisecond"); int Millisecond = FCString::Atoi(*TmpString);
		FDateTime LastUpdateTime{ Year, Month, Day, Hour, Minute, Second, Millisecond };
		AugmentaObject.LastUpdateTime = LastUpdateTime;
	}

	return AugmentaObject;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// Binary Serialization
//////////////////////////////////////////////////////////////////////////////////////////////

TArray<uint8> ALiveLinkAugmentaClusterManager::SerializeBinaryAugmentaScene(const FLiveLinkAugmentaScene AugmentaScene)
{
	TArray<uint8> EventData;

	// Allocate buffer memory
	const uint32 BufferSize = sizeof(AugmentaScene);
	EventData.SetNumUninitialized(BufferSize);

	FMemory::Memcpy(EventData.GetData(), &AugmentaScene, BufferSize);

	return EventData;
}

FLiveLinkAugmentaScene ALiveLinkAugmentaClusterManager::DeserializeBinaryAugmentaScene(const TArray<uint8> EventData)
{
	FLiveLinkAugmentaScene AugmentaScene;

	FMemory::Memcpy(&AugmentaScene, EventData.GetData(), sizeof(AugmentaScene));

	return AugmentaScene;
}

TArray<uint8> ALiveLinkAugmentaClusterManager::SerializeBinaryAugmentaVideoOutput(
	const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput)
{
	TArray<uint8> EventData;

	// Allocate buffer memory
	const uint32 BufferSize = sizeof(AugmentaVideoOutput);
	EventData.SetNumUninitialized(BufferSize);

	FMemory::Memcpy(EventData.GetData(), &AugmentaVideoOutput, BufferSize);

	return EventData;
}

FLiveLinkAugmentaVideoOutput ALiveLinkAugmentaClusterManager::DeserializeBinaryAugmentaVideoOutput(
	const TArray<uint8> EventData)
{
	FLiveLinkAugmentaVideoOutput AugmentaVideoOutput;

	FMemory::Memcpy(&AugmentaVideoOutput, EventData.GetData(), sizeof(AugmentaVideoOutput));

	return AugmentaVideoOutput;
}

TArray<uint8> ALiveLinkAugmentaClusterManager::SerializeBinaryAugmentaObject(
	const FLiveLinkAugmentaObject AugmentaObject)
{
	TArray<uint8> EventData;

	// Allocate buffer memory
	const uint32 BufferSize = sizeof(AugmentaObject);
	EventData.SetNumUninitialized(BufferSize);

	FMemory::Memcpy(EventData.GetData(), &AugmentaObject, BufferSize);

	return EventData;
}

FLiveLinkAugmentaObject ALiveLinkAugmentaClusterManager::DeserializeBinaryAugmentaObject(const TArray<uint8> EventData)
{
	FLiveLinkAugmentaObject AugmentaObject;

	FMemory::Memcpy(&AugmentaObject, EventData.GetData(), sizeof(AugmentaObject));

	return AugmentaObject;
}




// Copyright Augmenta 2023, All Rights Reserved.

#pragma once

#include "LiveLinkAugmentaData.h"
#include "Cluster/IDisplayClusterClusterEventListener.h"

#include "CoreMinimal.h"
#include "Cluster/IDisplayClusterClusterManager.h"
#include "GameFramework/Actor.h"
#include "LiveLinkAugmentaClusterManager.generated.h"

/** Forward Declarations */
class ALiveLinkAugmentaManager;
class IDisplayClusterClusterManager;

/** Delegates */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAugmentaSceneUpdatedClusterEvent, const FLiveLinkAugmentaScene, AugmentaScene);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAugmentaObjectUpdatedClusterEvent, const FLiveLinkAugmentaObject, AugmentaObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAugmentaVideoOutputUpdatedClusterEvent, const FLiveLinkAugmentaVideoOutput, AugmentaVideoOutput);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAugmentaSourceDestroyedClusterEvent);

UCLASS()
class LIVELINKAUGMENTADEMO_API ALiveLinkAugmentaClusterManager : public AActor, public IDisplayClusterClusterEventListener
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALiveLinkAugmentaClusterManager();

	//Augmenta Manager to attach to.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Augmenta")
	ALiveLinkAugmentaManager* AugmentaManager;

	//Whether to use binary or json cluster events.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Augmenta|Cluster Events")
	bool bUseBinaryClusterEvents;

	//Offset binary cluster events id by this value. Useful to avoid overlapping with other binary events.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Augmenta|Cluster Events")
	int BinaryEventIdOffset;

	//Send only the transform, id and age data of the Augmenta objects to improve performance. Only works with json cluster events.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Augmenta|Cluster Events")
	bool bSendReducedObjectData;

protected:

	bool bInitialized;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void SendSceneUpdatedClusterEvent(const FLiveLinkAugmentaScene AugmentaScene);

	UFUNCTION(BlueprintCallable)
	void SendVideoOutputUpdatedClusterEvent(const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput);

	UFUNCTION(BlueprintCallable)
	void SendObjectEnteredClusterEvent(const FLiveLinkAugmentaObject AugmentaObject);

	UFUNCTION(BlueprintCallable)
	void SendObjectUpdatedClusterEvent(const FLiveLinkAugmentaObject AugmentaObject);

	UFUNCTION(BlueprintCallable)
	void SendObjectLeftClusterEvent(const FLiveLinkAugmentaObject AugmentaObject);

	UFUNCTION(BlueprintCallable)
	void SendSourceDestroyedClusterEvent();

	TMap<FString, FString> SerializeJsonAugmentaScene(const FLiveLinkAugmentaScene AugmentaScene);
	FLiveLinkAugmentaScene DeserializeJsonAugmentaScene(const TMap<FString, FString> EventData);

	TMap<FString, FString> SerializeJsonAugmentaVideoOutput(const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput);
	FLiveLinkAugmentaVideoOutput DeserializeJsonAugmentaVideoOutput(const TMap<FString, FString> EventData);

	TMap<FString, FString> SerializeJsonAugmentaObject(const FLiveLinkAugmentaObject AugmentaObject);
	FLiveLinkAugmentaObject DeserializeJsonAugmentaObject(const TMap<FString, FString> EventData);

	TArray<uint8> SerializeBinaryAugmentaScene(const FLiveLinkAugmentaScene AugmentaScene);
	FLiveLinkAugmentaScene DeserializeBinaryAugmentaScene(const TArray<uint8> EventData);

	TArray<uint8> SerializeBinaryAugmentaVideoOutput(const FLiveLinkAugmentaVideoOutput AugmentaVideoOutput);
	FLiveLinkAugmentaVideoOutput DeserializeBinaryAugmentaVideoOutput(const TArray<uint8> EventData);

	TArray<uint8> SerializeBinaryAugmentaObject(const FLiveLinkAugmentaObject AugmentaObject);
	FLiveLinkAugmentaObject DeserializeBinaryAugmentaObject(const TArray<uint8> EventData);

	IDisplayClusterClusterManager* ClusterManager;

public:

	// A delegate that is fired when an Augmenta scene message is received.
	UPROPERTY(BlueprintAssignable, Category = "Augmenta|Events")
	FAugmentaSceneUpdatedClusterEvent OnAugmentaSceneUpdatedCluster;

	// A delegate that is fired when an Augmenta video output (fusion) message is received.
	UPROPERTY(BlueprintAssignable, Category = "Augmenta|Events")
	FAugmentaVideoOutputUpdatedClusterEvent OnAugmentaVideoOutputUpdatedCluster;

	// A delegate that is fired when a new Augmenta object entered the scene.
	UPROPERTY(BlueprintAssignable, Category = "Augmenta|Events")
	FAugmentaObjectUpdatedClusterEvent OnAugmentaObjectEnteredCluster;

	// A delegate that is fired when an Augmenta object has been updated.
	UPROPERTY(BlueprintAssignable, Category = "Augmenta|Events")
	FAugmentaObjectUpdatedClusterEvent OnAugmentaObjectUpdatedCluster;

	// A delegate that is fired when an Augmenta object has left the scene.
	UPROPERTY(BlueprintAssignable, Category = "Augmenta|Events")
	FAugmentaObjectUpdatedClusterEvent OnAugmentaObjectLeftCluster;

	// A delegate that is fired when the Augmenta Live Link source is destroyed.
	UPROPERTY(BlueprintAssignable, Category = "Augmenta|Events")
	FAugmentaSourceDestroyedClusterEvent OnAugmentaSourceDestroyedCluster;

	UFUNCTION(BlueprintNativeEvent)
	void OnClusterEventJson(const FDisplayClusterClusterEventJson& Event);

	UFUNCTION(BlueprintNativeEvent)
	void OnClusterEventBinary(const FDisplayClusterClusterEventBinary& Event);

};

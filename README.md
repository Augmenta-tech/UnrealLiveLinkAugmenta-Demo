# [Augmenta](https://www.augmenta.tech) [Unreal](https://www.unrealengine.com) [Live Link](https://docs.unrealengine.com/5.1/en-US/live-link-in-unreal-engine/) Demo

Example Unreal 5.1 project using the [UnrealLiveLinkAugmenta](https://www.unrealengine.com/marketplace/en-US/product/live-link-augmenta) plugin.

For an example project including the Live Link Augmenta plugin sources directly, please check the [master](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/tree/master) branch.

## Quick Start

1. Download and install the plugin for Unreal 5.1 from the [marketplace](https://www.unrealengine.com/marketplace/en-US/product/live-link-augmenta).

2. Clone or download this repository.

3. Launch this project, it will open the L_LiveLinkAugmentaDemo_AugmentaManager level by default.

4. Set up an Augmenta input either :
    1. Using the [Augmenta Simulator](https://github.com/Augmenta-tech/Augmenta-Simulator/releases) : Start the Augmenta Simulator and set its output ip address to your computer ip address (127.0.0.1 if you are running the simulator and Unreal on the same computer). Set the output port to 12000 (this is the default value).
    2. Using Augmenta Fusion : Add an output in Fusion with the ip address set to your computer ip address (127.0.0.1 if you are running Fusion and Unreal on the same computer). Set the output port to 12000 (this is the default value).

5. Play the level in Unreal, you should see the Augmenta scene and objects from the simulator or Fusion appear in the scene after a few seconds.

## Setup

For more information on the Live Link Augmenta setup, please refer to the plugin [documentation](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/README.md).

# Documentation

## Using Live Link subjects

The LiveLinkAugmentaDemo_LiveLinkSubjects level shows an example of controlling scene actors transforms with the Live Link subjects.

In this level, the AugmentaScene, AugmentaVideoOutput and the 3 AugmentaObjects actors were placed beforehand, each with a LiveLinkController component. Their transforms are controlled by the Live Link data, through these components. Note that in the following video, only the actors previously placed in the scene are updated when the corresponding Augmenta object is received in the Live Link window.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/marketplace-demo/Resources/Documentation/Images/LiveLinkSubjectsAnimation.gif)

## Using Augmenta Manager

The LiveLinkAugmentaDemo_AugmentaManager level shows an example of using the AugmentaManager class to receive the Augmenta events in Blueprint and spawn custom objects at runtime.

The blueprint BP_AugmentaVisualizer shows an example of how to use the Augmenta manager events to instantiate, update and destroy custom actors according to the Augmenta events. In this visualizer example, the entire process is done in blueprint but you could also connect to the Augmenta manager via a C++ class.

In this level, only the blueprints BP_AugmentaManager and BP_AugmentaVisualizer were placed beforehand. All the other actors are created at runtime by the AugmentaVisualizer, listening to the AugmentaManager events.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/marketplace-demo/Resources/Documentation/Images/AugmentaManagerAnimation.gif)

## Using Live Link Augmenta with nDisplay

When using nDisplay and Augmenta, you have several options to synchronize the Augmenta data accross your nDisplay cluster detailed below.

>Please note that nDisplay is only supported on Windows and Linux currently.

### Synchronizing Live Link subjects

If you are using Live Link subjects, you can synchronize them easily, simply enabling the Live Link over nDisplay plugin.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/marketplace-demo/Resources/Documentation/Images/LiveLinkOverNDisplayPlugin.jpg)

### Synchronizing Augmenta Manager events directly in Blueprint

If you are using the Augmenta manager, you can synchronize the Augmenta events by sending cluster events from the Augmenta manager events.

The LiveLinkAugmentaDemo_AugmentaManager_nDisplay shows an example of this approach. The blueprint BP_AugmentaVizualiser_nDisplay shows an example of this implementation, adapting the logic of the BP_AugmentaVisualizer to work with an nDisplay cluster.

The main idea behind this implementation is to bind functions sending cluster events to the AugmentaManager events. Then listening to the incoming cluster events to perform the Augmenta visualization logic.

While this approach is simple to implement, it is not well suited to handle a lot of Augmenta objects due to the added performance cost of blueprint logic. If you have to deal with more than 10 or 20 Augmenta objects at once, we recommend you use the AugmentaClusterManager instead.

### Synchronizing Augmenta Manager events using the Augmenta Cluster Manager

> To keep this demo project C++ free, the AugmentaClusterManager class has only been added in the [master](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/tree/master) branch of this repository. Please refer to it if you wish to use the AugmentaClusterManager.

The AugmentaClusterManager is a C++ class that binds to an existing AugmentaManager to send the event from the AugmentaManager through the cluster via cluster events. This allows to propagate the Augmenta events through the cluster in a synchronized manner. This framework is described in the diagram below.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/marketplace-demo/Resources/Documentation/Images/AugmentaClusterManagerDiagram.jpg)

An example of this setup is shown in the level LiveLinkAugmentaDemo_AugmentaClusterManager_nDisplay. Notice in the level there is an AugmentaManager, an AugmentaClusterManager, linked to the AugmentaManager, and an AugmentaClusterVisualizer, linked to the AugmentaClusterManager.

The AugmentaClusterManager propagate cluster events from the incoming Augmenta events from the AugmentaManager, while the AugmentaClusterVisualizer listen to the AugmentaClusterManager events for the instantiation and update of the visualization objects.

Augmenta Documentation
-------------

https://github.com/Augmenta-tech/Augmenta/wiki

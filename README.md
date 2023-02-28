# [Augmenta](https://www.augmenta-tech.com) [Unreal](https://www.unrealengine.com) [Live Link](https://docs.unrealengine.com/5.1/en-US/live-link-in-unreal-engine/) Demo

Example Unreal 5.1 project using the [UnrealLiveLinkAugmenta](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta) plugin.

## Installation

This project uses the plugin sources directly. For an example project using the plugin from the Unreal marketplace, please check the [marketplace-demo](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/tree/marketplace-demo) branch.

If you are not familiar with Git or C++ compilation in Unreal, we highly suggest you use the [marketplace demo](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/tree/marketplace-demo) instead.

### Git User 

1. Clone this repository.

2. Pull the UnrealLiveLinkAugmenta submodule using `$git submodule update --init --recursive`.

### Non-Git User

1. Download this repository and unzip it.

2. Download the [UnrealLiveLinkAugmenta](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta) repository and unzip it in the `Plugins/UnrealLiveLinkAugmenta` folder.

## Setup

### Creating the Live Link Augmenta Source

1. Open the Live Link window by clicking Window -> Virtual Production -> Live Link.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/master/Resources/Documentation/Images/LiveLinkSourceCreation_1.jpg)

2. In the Live Link window click on Source, LiveLinkAugmenta Source, enter your IP address, port and scene name, then click Add.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/master/Resources/Documentation/Images/LiveLinkSourceCreation_2.jpg)

3. If you are receiving Augmenta data (from a node, Fusion, or the simulator), you should see the received Augmenta elements in the subjects list.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/master/Resources/Documentation/Images/LiveLinkSourceCreation_3.jpg)

## Content

### Using Live Link Subjects

The L_LiveLinkAugmentaDemo_LiveLinkSubjects level shows an example of controlling scene actors transforms with the Live Link subjects.

In this level, the AugmentaScene, AugmentaVideoOutput and the 3 AugmentaObjects actors were placed beforehand, each with a LiveLinkController component. Their transforms are controlled by the Live Link data, through these components. Note that in the following video, only the actors previously placed in the scene are updated when the corresponding Augmenta object is received in the Live Link window.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/master/Resources/Documentation/Images/LiveLinkSubjectsAnimation.gif)

### Using Augmenta Manager

The L_LiveLinkAugmentaDemo_AugmentaManager level shows an example of using the AugmentaManager class to receive the Augmenta events in Blueprint and spawn custom objects at runtime.

The blueprint BP_AugmentaVisualizer shows an example of how to use the Augmenta manager events to instantiate, update and destroy custom actors according to the Augmenta events. In this visualizer example, the entire process is done in blueprint but you could also connect to the Augmenta manager via a C++ class.

In this level, only the blueprints BP_AugmentaManager and BP_AugmentaVisualizer were placed beforehand. All the other actors are created at runtime by the AugmentaVisualizer, listening to the AugmentaManager events.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta-Demo/blob/master/Resources/Documentation/Images/AugmentaManagerAnimation.gif)

Augmenta Documentation
-------------

https://github.com/Augmenta-tech/Augmenta/wiki

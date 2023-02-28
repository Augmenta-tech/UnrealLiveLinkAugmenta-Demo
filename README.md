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

For more information on the plugin setup, please refer to the plugin [documentation](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/README.md).

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

# [Augmenta](https://www.augmenta-tech.com) [Unreal](https://www.unrealengine.com) [Live Link](https://docs.unrealengine.com/5.1/en-US/live-link-in-unreal-engine/) Demo

Example Unreal 5.1 project using the [UnrealLiveLinkAugmenta](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta) plugin.

## Installation

### Git User

Once you clone this repository, don't forget to pull the UnrealLiveLinkAugmenta submodule using `$git submodule update --init --recursive`.

### Non-Git User

1. Download this repository and unzip it.

2. Download the [UnrealLiveLinkAugmenta](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta) repository and unzip it in the `Plugins/UnrealLiveLinkAugmenta` folder.

## Setup

### Creating the Live Link Augmenta Source

1. Open the Live Link window by clicking Window -> Virtual Production -> Live Link.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Documentation/Images/LiveLinkSourceCreation_1.jpg)

2. In the Live Link window click on Source, LiveLinkAugmenta Source, enter your IP address, port and scene name, then click Add.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Documentation/Images/LiveLinkSourceCreation_2.jpg)

3. If you are receiving Augmenta data (from a node, Fusion, or the simulator), you should see the received Augmenta elements in the subjects list.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Documentation/Images/LiveLinkSourceCreation_3.jpg)

## Content

### Using Live Link Subjects

The L_LiveLinkAugmentaDemo_LiveLinkSubjects level shows an example of controlling scene actors transforms with the Live Link subjects.

In this level, the Augmenta Scene, Augmenta Video Output and the 3 Augmenta Objects actors were placed beforehand with the LiveLinkController components. Their transforms are controlled at runtime by these components.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Documentation/Images/AugmentaLiveLinkSubjects.jpg)

### Using Augmenta Manager

The L_LiveLinkAugmentaDemo_AugmentaManager level shows an example of using the AugmentaManager script to get the Augmenta Event in Blueprint and spawn a custom object at runtime.

The blueprint BP_AugmentaVisualizer shows an example of how to use the Augmenta manager events to instantiate, update and destroy custom actors according to the incoming Augmenta stream. In this visualizer example, the entire process is done in blueprint but you could also connect to the Augmenta manager via a C++ class.

In this level, only the blueprints BP_AugmentaManager and BP_AugmentaVisualizer were placed beforehand. All the other actors are created at runtime by the AugmentaVisualizer, listening to the AugmentaManager events.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Documentation/Images/AugmentaManagerAndVisualizer.jpg)

Augmenta Documentation
-------------

https://github.com/Augmenta-tech/Augmenta/wiki

# [Augmenta](https://www.augmenta-tech.com) [Unreal](https://www.unrealengine.com) [Live Link](https://docs.unrealengine.com/5.0/en-US/live-link-in-unreal-engine/) Demo

Example Unreal 5.0 project using the [UnrealLiveLinkAugmenta](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta) plugin.

## Installation

### Git User

Once you clone this repository, don't forget to pull the UnrealLiveLinkAugmenta submodule using `$git submodule update --init --recursive`.

### Non-Git User

1. Download this repository and unzip it.

2. Download the [UnrealLiveLinkAugmenta](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta) repository and unzip it in the `Plugins/UnrealLiveLinkAugmenta` folder.

## Setup

### Creating the Live Link Augmenta Source

1. Open the Live Link window by clicking Window -> Virtual Production -> Live Link.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Images/LiveLinkSourceCreation_1.jpg)

2. In the Live Link window click on Source, LiveLinkAugmenta Source, enter your IP address, port and scene name, then click Add.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Images/LiveLinkSourceCreation_2.jpg)

3. If you are receiving Augmenta data (from a node, Fusion, or the simulator), you should see the received Augmenta elements in the subjects list.

![](https://github.com/Augmenta-tech/UnrealLiveLinkAugmenta/blob/main/Resources/Images/LiveLinkSourceCreation_3.jpg)

## Content

### Using Live Link Subjects

The L_LiveLinkAugmentaDemo_LiveLinkSubjects level shows an example of controlling scene actors transforms with the Live Link subjects.

Augmenta Documentation
-------------

https://github.com/Augmenta-tech/Augmenta/wiki

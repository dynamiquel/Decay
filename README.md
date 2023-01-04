# Decay
Unreal Engine system for handling the eventual destruction of inactive Actors and ActorComponents, such as corpses and bullet impacts. Forked from an early build of [Abstruse](https://vixentail.com/abstruse).

**C++ and Blueprint**

## How-to-use
### 1. Create a Decay Instance
A **Decay Instance** is a group of decayable objects in your game. We recommend making a Decay Instance for every type of decayable object in your game, for example, one for Corpses, another for Bullet Impacts and another for Foliage. This allows Corpses and Bullet Impacts to act independently from each other. Alternatively, you could assign a Decay Instance in a priority fashion instead, such as VeryHigh, High, Normal and Low.

Decay Instances can be easily created by going to **Project Settings > Plugins > Decay** and then adding a **CustomDecayInstance**. The Name you provide for the Decay Instance is the ID you will use to reference this Decay Instance later.

The **Default Decay Instance** is the Decay Instance where objects will be automatically assigned to if no Decay Instance Name is given and can be used as an 'other' decay group.

![image](https://user-images.githubusercontent.com/50085636/210535976-aae916db-3040-4f2b-853d-37dd1ae4061b.png)

### 2. Adding an Actor or ActorComponent to a Decay Instance
Instead of destroying an Actor or ActorComponent when it enters an inactive state (immediately or after time has passed), as you typically would, register the object with a Decay Instance by either:

#### Default Decay Instance
- `DECAY_DEFAULT_ADD(ObjectPtr)` (preferred)
- `GetWorld()->GetSubsystem<UDecaySubsystem>()->GetDefaultDecayInstance()->AddObject(ObjectPtr)` (same for Blueprint)

#### Custom Decay Instance
- `DECAY_ADD(DecayInstanceName, ObjectPtr)` (preferred)
- `GetWorld()->GetSubsystem<UDecaySubsystem>()->GetDecayInstance(DecayInstanceName)->AddObject(ObjectPtr)` (same for Blueprint)

### 3. And more
That's all you need to know to use Decay. For anything else, such as disabling Decay Instances or manually destroying objects, feel free to read the documentation within the code. 

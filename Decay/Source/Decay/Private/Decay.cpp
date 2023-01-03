// © 2021 Vixentail, Liam Hall and Savir Sohal


#include "Decay.h"

#include "DecayModule.h"

void UDecay::Init(FName InInstanceName, int32 InMaxObjects, float InMaxObjectLifetime, bool bInDestroyIfCannotBeAdded)
{
	InstanceName = InInstanceName;
	// TODO: Ceil to closest power of 2.
	MaxObjects = InMaxObjects;
	MaxObjectLifetime = InMaxObjectLifetime;
	bDestroyIfCannotBeAdded = bInDestroyIfCannotBeAdded;
	bUsingTimeExpiration = MaxObjectLifetime > 0.f;

	if (MaxObjects > 0)
		ReferencedObjects = new TCircularQueue<TObjectPtr<UObject>>(GetMaxObjects());

	// Only create the Referenced Timers queue if needed.
	if (bUsingTimeExpiration)
		ReferencedTimers = new TCircularQueue<FTimerHandle>(GetMaxObjects());
}

void UDecay::Register()
{
	if (bRegistered)
		return;

	bRegistered = true;
	bEnabled = true;
}

void UDecay::FinishDestroy()
{
	/*bEnabled = false;
	DestroyAllObjects();
	ReferencedObjects->Empty();
	ReferencedObjects = nullptr;*/

	Super::FinishDestroy();
}

void UDecay::AddObject(UObject* NewObject)
{
	if (!IsRegistered() || !IsEnabled())
	{
		UE_LOG(LogDecay, Warning, TEXT("Attempted to add an object but the Decay Instance is not registered or enabled."));
		
		if (bDestroyIfCannotBeAdded)
			DestroyObject(NewObject);
		return;
	}

	// This instance cannot store anything, instantly destroy.
	if (GetMaxObjects() <= 0)
	{
		DestroyObject(NewObject);
		return;
	}
	
	if (ReferencedObjects->IsFull())
		DestroyOldestObject();

	ReferencedObjects->Enqueue(NewObject);
	CreateNewTimer();
}

void UDecay::DestroyOldestObject()
{
	DestroyOldestObjects(1);
}

void UDecay::DestroyOldestObjects(int32 PopTimes)
{
	// This is an empty decay instance; nothing to destroy.
	if (GetMaxObjects() <= 0)
		return;
	
	while (PopTimes > 0 && !GetReferencedObjects().IsEmpty())
	{
		PopTimes--;
		TObjectPtr<UObject> Object;
		if (ReferencedObjects->Dequeue(OUT Object))
		{
			DestroyOldestTimer();
			DestroyObject(Object);
		}
	}
}

void UDecay::DestroyAllObjects()
{
	// This is an empty decay instance; nothing to destroy.
	if (GetMaxObjects() <= 0)
		return;
	
	// Alternate destroy for increased efficiency.
	while (!GetReferencedObjects().IsEmpty())
	{
		TObjectPtr<UObject> Object;
		if (ReferencedObjects->Dequeue(OUT Object))
			DestroyObject(Object);
	}
	
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UDecay::DestroyObject(const TObjectPtr<UObject> Object) const
{
	if (!IsValid(Object))
		return;
	
	if (Object.IsA(AActor::StaticClass()))
		Cast<AActor>(Object)->Destroy();
	else if (Object.IsA(UActorComponent::StaticClass()))
		Cast<UActorComponent>(Object)->DestroyComponent();
}

void UDecay::DestroyOldestTimer()
{
	FTimerHandle Timer;
	if (ReferencedTimers->Dequeue(OUT Timer))
		GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void UDecay::CreateNewTimer()
{
	checkf(!ReferencedTimers->IsFull(), TEXT("UDecay::ReferencedTimers is full. This shouldn't happen. Fix your code."))
	checkf(GetWorld(), TEXT("UDecay cannot find World. Was it not created by a subobject of the World?"))
	
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UDecay::DestroyOldestObject, GetMaxObjectLifetime());
	ReferencedTimers->Enqueue(Timer);
}

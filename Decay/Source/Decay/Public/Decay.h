// © 2021 Vixentail, Liam Hall and Savir Sohal

#pragma once

#include "CoreMinimal.h"
#include "Containers/CircularQueue.h"
#include "UObject/Object.h"

#include "Decay.generated.h"

/**
 * Handles an instance of objects you wish to be destroyed after some time or to enforce a maximum amount.
 *
 * Do not manually create or destroy this object. Lifetime is managed exclusively by DecaySubsystem.
 */
UCLASS()
class DECAY_API UDecay : public UObject
{
	GENERATED_BODY()

private:
	bool bEnabled = false;
	bool bRegistered = false;

	FName InstanceName;
	bool bUsingTimeExpiration = false;
	bool bDestroyIfCannotBeAdded = false;
	int32 MaxObjects = 0;
	float MaxObjectLifetime = 15.f;
	
	TCircularQueue<TObjectPtr<UObject>>* ReferencedObjects = nullptr;
	TCircularQueue<FTimerHandle>* ReferencedTimers = nullptr;
	
public:
	/* Set ups the Decay instance with all the necessary data. */
	void Init(FName InInstanceName, int32 InMaxObjects, float InMaxObjectLifetime, bool bInDestroyIfCannotBeAdded);

	/* Marks the Decay instance as registered and ready to use. */
	void Register();

	virtual void FinishDestroy() override;

public:
	/* The Decay instance is registered and ready to use. */
	UFUNCTION(BlueprintCallable, Category="Decay")
	bool IsRegistered() const { return bRegistered; }

	/* The Decay instance is currently enabled. */
	UFUNCTION(BlueprintCallable, Category="Decay")
	bool IsEnabled() const { return bEnabled;}

	const TCircularQueue<TObjectPtr<UObject>>& GetReferencedObjects() const { return *ReferencedObjects; }

	UFUNCTION(BlueprintCallable, Category="Decay")
	int32 GetMaxObjects() const { return MaxObjects; }

	UFUNCTION(BlueprintCallable, Category="Decay")
	float GetMaxObjectLifetime() const { return MaxObjectLifetime; }

	UFUNCTION(BlueprintCallable, Category="Decay")
	FName GetInstanceName() const { return InstanceName; }

public:
	UFUNCTION(BlueprintCallable, Category="Decay")
	void SetActive(bool bNewEnabled)
	{
		if (bEnabled == bNewEnabled)
			return;

		bEnabled = bNewEnabled;
	}

	UFUNCTION(BlueprintCallable, Category="Decay")
	void AddObject(UObject* NewObject);

	UFUNCTION()
	void DestroyOldestObject();
	
	UFUNCTION(BlueprintCallable, Category="Decay")
	void DestroyOldestObjects(int32 PopTimes);

	UFUNCTION(BlueprintCallable, Category="Decay")
	void DestroyAllObjects();

protected:
	void DestroyObject(TObjectPtr<UObject> Object) const;
	void DestroyOldestTimer();
	void CreateNewTimer();
};

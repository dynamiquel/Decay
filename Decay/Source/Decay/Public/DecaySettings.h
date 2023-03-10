// © 2021 Vixentail, Liam Hall and Savir Sohal

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DecaySettings.generated.h"

USTRUCT()
struct DECAY_API FDecayInstanceSettings
{
	GENERATED_BODY()

	/** The maximum number of UObjects this Decay Instance can store before it starts destroying.
	 *
	 * Will round up to the closest power of 2.
	 *
	 * 0: The UObject is destroyed instantly.
	 */
	UPROPERTY(Config, EditAnywhere, meta = (ClampMin = 0, ClampMax = 1024))
	int32 MaxObjects = 32;

	/** The time in seconds until the UObject added to this Decay Instance is destroyed.
	 *
	 * -1: The UObject is destroyed instantly.
	 */
	UPROPERTY(Config, EditAnywhere, meta = (ClampMin = -1, ClampMax = 1200, Delta = 0.1f))
	float MaxObjectLifetime = 30.f;

	/** Whether an unsuccessful addition of a UObject should instantly destroy the object or should the Decay Instance
	 * ignore the operation (will not be destroyed or pending to destroy).
	 */
	UPROPERTY(Config, EditAnywhere)
	bool bDestroyObjectIfCannotBeAdded = true;

	/** Should this Decal Instance be created? */
	UPROPERTY(Config, EditAnywhere)
	bool bEnabled = true;
};

/**
 * 
 */
UCLASS(Config=DecaySetting)
class DECAY_API UDecaySettings : public UObject
{
	GENERATED_BODY()

public:
	/** Should the Decal Subsystem be loaded? */
	UPROPERTY(Config, EditAnywhere)
	bool bEnabled = true;
	
	UPROPERTY(Config, EditAnywhere)
	FDecayInstanceSettings DefaultDecayInstance;
	
	UPROPERTY(Config, EditAnywhere)
	TMap<FName, FDecayInstanceSettings> CustomDecayInstances;
};
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DecaySubsystem.generated.h"

#define DECAY_CORPSE FName(TEXT("CORPSE"))
#define DECAY_DECAL FName(TEXT("DECAL"));
#define DECAY_ENVIRONMENT FName(TEXT("ENVIRONMENT"));

#define DECAY_ADD(DecayInstanceName, UObjectPtr) GetWorld()->GetSubsystem<UDecaySubsystem>()->GetDecayInstance(DecayInstanceName)->AddObject(UObjectPtr);
#define DECAY_DEFAULT_ADD(UObjectPtr) GetWorld()->GetSubsystem<UDecaySubsystem>()->GetDefaultDecayInstance()->AddObject(UObjectPtr);


class UDecay;

UCLASS()
class DECAY_API UDecaySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UDecay>> DecayInstances;
	
	const FName DefaultInstanceName = FName(TEXT("Default"));

	void RegisterDecayInstances();

public:
	UFUNCTION(BlueprintCallable, Category="Decay")
	UDecay* GetDecayInstance(FName InstanceName);

	UFUNCTION(BlueprintCallable, Category="Decay")
	UDecay* GetDefaultDecayInstance();
	
	UFUNCTION(BlueprintCallable, Category="Decay")
	bool DoesDecayInstanceExist(FName InstanceName);

	UFUNCTION(BlueprintCallable, Category="Decay")
	void DestroyDecayInstance(FName InstanceName);

	UFUNCTION(BlueprintCallable, Category="Decay")
	FName GetDefaultInstanceName() const { return DefaultInstanceName; }
};

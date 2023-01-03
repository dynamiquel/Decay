// Copyright Epic Games, Inc. All Rights Reserved.

#include "DecaySubsystem.h"
#include "Decay.h"
#include "DecaySettings.h"

void UDecaySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Create the Decal Instances if this Subsystem is wanted.
	const UDecaySettings* Settings = GetMutableDefault<UDecaySettings>();
	if (Settings->bEnabled)
		RegisterDecayInstances();
}

void UDecaySubsystem::Deinitialize()
{
	for (TMap<FName, TObjectPtr<UDecay>>::TIterator It(DecayInstances); It; ++It)
	{
		It.Value()->ConditionalBeginDestroy();
	}
	DecayInstances.Empty();
}

void UDecaySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	
}

bool UDecaySubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UDecaySubsystem::RegisterDecayInstances()
{
	const UDecaySettings* Settings = GetMutableDefault<UDecaySettings>();
	
	// Default Decay Instances
	TObjectPtr<UDecay> DefaultDecay = NewObject<UDecay>(this);
	DefaultDecay->Init(
		DefaultInstanceName,
		Settings->DefaultDecayInstance.MaxObjects,
		Settings->DefaultDecayInstance.MaxObjectLifetime,
		Settings->DefaultDecayInstance.bDestroyObjectIfCannotBeAdded);

	DecayInstances.Add(DefaultInstanceName, DefaultDecay);
	DefaultDecay->Register();

	// Custom Decal Instances.
	for (auto DecayInstance : Settings->CustomDecayInstances)
	{
		// Don't create if a Decay Instance already exists with this name.
		if (DoesDecayInstanceExist(DecayInstance.Key) || !DecayInstance.Value.bEnabled)
			continue;
		
		TObjectPtr<UDecay> NewDecay = NewObject<UDecay>(this);
		NewDecay->Init(
			DecayInstance.Key,
			DecayInstance.Value.MaxObjects,
			DecayInstance.Value.MaxObjectLifetime,
			DecayInstance.Value.bDestroyObjectIfCannotBeAdded);

		DecayInstances.Add(DecayInstance.Key, NewDecay);
		NewDecay->Register();
	}	
}

UDecay* UDecaySubsystem::GetDecayInstance(FName InstanceName)
{
	if (InstanceName.IsNone())
		return GetDefaultDecayInstance();
	
	if (DoesDecayInstanceExist(InstanceName))
		return DecayInstances[InstanceName];

	return nullptr;
}

UDecay* UDecaySubsystem::GetDefaultDecayInstance()
{
	if (DoesDecayInstanceExist(DefaultInstanceName))
		return DecayInstances[DefaultInstanceName];

	return nullptr;
}

bool UDecaySubsystem::DoesDecayInstanceExist(FName InstanceName)
{
	if (InstanceName.IsNone())
		InstanceName = DefaultInstanceName;
	
	if (const TObjectPtr<UDecay>* DecayInstance = DecayInstances.Find(InstanceName))
	{
		// If contains a null reference, remove it.
		if (DecayInstance->IsNull())
			DecayInstances.Remove(InstanceName);
		else
			return true;
	}

	return false;
}

void UDecaySubsystem::DestroyDecayInstance(FName InstanceName)
{
	if (InstanceName.IsNone())
		InstanceName = DefaultInstanceName;
	
	if (const TObjectPtr<UDecay>* DecayInstance = DecayInstances.Find(InstanceName))
	{
		if (IsValid(*DecayInstance))
			DecayInstance->Get()->ConditionalBeginDestroy();
		
		DecayInstances.Remove(InstanceName);
	}
}
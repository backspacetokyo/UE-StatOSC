// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Subsystems/EngineSubsystem.h"
#include "OSCClient.h"
#include "StatOSCSubsystem.generated.h"

UCLASS(config=Project, defaultconfig)
class UStatOSCSubsystemSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, config, Category = OSCStat)
	FString OSCTargetHost = "127.0.0.1";

	UPROPERTY(EditAnywhere, config, Category = OSCStat)
	int OSCTargetPort = 20000;
};

UCLASS()
class STATOSC_API UStatOSCSubsystem : public UEngineSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:

	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override { return true; }
	virtual bool IsTickableInEditor() const override { return true; }
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class UOSCClient* OSCClient;

private:

	void AddStatMessage(const FString ComputerName, const FString StatName, float Value, FOSCBundle& Bundle);
};

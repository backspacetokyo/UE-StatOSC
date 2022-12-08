#include "StatOSCSubsystem.h"

#include "OSCManager.h"

UStatOSCSubsystemSettings::UStatOSCSubsystemSettings(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UStatOSCSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UStatOSCSubsystemSettings* Settings = GetDefault<UStatOSCSubsystemSettings>();
	
	OSCClient = UOSCManager::CreateOSCClient(Settings->OSCTargetHost, Settings->OSCTargetPort, TEXT("StatOSC"), GEngine);
}

void UStatOSCSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

TStatId UStatOSCSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMyGameInstanceSubsystem, STATGROUP_Tickables);
}

void UStatOSCSubsystem::Tick(float DeltaTime)
{
	if (!IsValid(OSCClient))
		return;

	auto ComputerName = FPlatformProcess::ComputerName();
	
	FOSCBundle Bundle;
	UOSCManager::ClearBundle(Bundle);

	float DiffTime = FApp::GetCurrentTime() - FApp::GetLastTime();
	AddStatMessage(ComputerName, TEXT("FrameTime"), DiffTime * 1000, Bundle);

	AddStatMessage(ComputerName, TEXT("GameThreadTime"), FPlatformTime::ToMilliseconds(GGameThreadTime), Bundle);
	AddStatMessage(ComputerName, TEXT("RenderThreadTime"), FPlatformTime::ToMilliseconds(GRenderThreadTime), Bundle);
	AddStatMessage(ComputerName, TEXT("InputLatencyTime"), FPlatformTime::ToMilliseconds64(GInputLatencyTime), Bundle);
	AddStatMessage(ComputerName, TEXT("RHIThreadTime"), FPlatformTime::ToMilliseconds(GRHIThreadTime), Bundle);

	for (uint32 GPUIndex : FRHIGPUMask::All())
	{
		const uint32 GPUCycles = RHIGetGPUFrameCycles(GPUIndex);
		float GPUFrameTime = FPlatformTime::ToMilliseconds(GPUCycles);

		AddStatMessage(ComputerName, FString::Format(TEXT("GPUFrameTime{0}"), {GPUIndex}), GPUFrameTime, Bundle);
	}
	
	AddStatMessage(ComputerName, TEXT("ScreenPercentage"), Scalability::GetResolutionScreenPercentage(), Bundle);
	AddStatMessage(ComputerName, TEXT("QualityLevel"), Scalability::GetQualityLevels().GetSingleQualityLevel(), Bundle);

	{
		static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));
		float vsync = CVar->GetFloat();
		AddStatMessage(ComputerName, TEXT("VSync"), vsync, Bundle);
	}
	
	AddStatMessage(ComputerName, TEXT("SystemResolutionX"), GSystemResolution.ResX, Bundle);
	AddStatMessage(ComputerName, TEXT("SystemResolutionY"), GSystemResolution.ResY, Bundle);

	OSCClient->SendOSCBundle(Bundle);
}

void UStatOSCSubsystem::AddStatMessage(const FString ComputerName, const FString StatName, float Value, FOSCBundle& Bundle)
{
	FOSCMessage Message;
	UOSCManager::ClearMessage(Message);
	
	UOSCManager::SetOSCMessageAddress(Message,
		UOSCManager::ConvertStringToOSCAddress(
			TEXT("/") + ComputerName + TEXT("/") +  StatName
		));
	;
	UOSCManager::AddFloat(Message, Value);
	UOSCManager::AddMessageToBundle(Message, Bundle);
}

#include "DaytimeManager.h"

#include "Curves/CurveLinearColor.h"
#include "Engine/DirectionalLight.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void ADaytimeManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ADaytimeManager::ADaytimeManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.0f;

	SkyboxComponent = CreateDefaultSubobject<UStaticMeshComponent>("Skybox");
	RootComponent = SkyboxComponent;
}

void ADaytimeManager::BeginPlay()
{
	Super::BeginPlay();

	UMaterialParameterCollection* ParameterCollectionFinder = LoadObject<UMaterialParameterCollection>(
		nullptr, TEXT("/Game/Visuals/Materials/MPC_GlobalParams"));
	if (ParameterCollectionFinder)
	{
		ParameterCollection = GetWorld()->GetParameterCollectionInstance(ParameterCollectionFinder);
	}

	if (!SkyboxMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("DaytimeManager missing Static Mesh"))
		return;
	}
	if (!SkyboxMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("DaytimeManager missing Material"))
		return;
	}
	SkyboxComponent->SetStaticMesh(SkyboxMesh);
	SkyboxComponent->SetRelativeScale3D(FVector(400, 400, 400));
	DynamicMaterial = SkyboxComponent->CreateDynamicMaterialInstance(0, SkyboxMaterial);
}


void ADaytimeManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	SetTime(GetTime() + (DayTimeSpeed * DeltaSeconds) / 60);

	SunHeight = FMath::GetMappedRangeValueUnclamped(
		FVector2D(0, -90),
		FVector2D(0, 1),
		SunActor->GetActorRotation().Pitch);
	RefreshMaterial();
}

void ADaytimeManager::SetTime(const float NewTime)
{
	CurrentTime = NewTime < 0 ? 0 : FMath::Fmod(NewTime, GetFullDayLength());
	if (CurrentTime <= DayLength)
	{
		// It should be Day
		if (!bIsDay) StartDay();
		const float NewSunPitch = FMath::GetMappedRangeValueUnclamped(
			FVector2D(0, DayLength),
			FVector2D(-180, 0),
			CurrentTime);

		SunActor->SetActorRotation(FRotator(NewSunPitch, 0, 0));
		MoonActor->SetActorRotation(FRotator(NewSunPitch + 180.0, 0, 0));
	}
	else
	{
		// It should be Night
		if (bIsDay) StartNight();

		const float NewMoonPitch = FMath::GetMappedRangeValueUnclamped(
			FVector2D(0, NightLength),
			FVector2D(-180, 0),
			CurrentTime);

		SunActor->SetActorRotation(FRotator(NewMoonPitch + 180.0, 0, 0));
		MoonActor->SetActorRotation(FRotator(NewMoonPitch, 0, 0));
	}
}

void ADaytimeManager::StartDay()
{
	bIsDay = true;
	SunActor->SetEnabled(true);
	MoonActor->SetEnabled(false);

	if (ParameterCollection)
		ParameterCollection->SetScalarParameterValue(FName("IsNight"), 0.0f);
}

void ADaytimeManager::StartNight()
{
	bIsDay = false;
	SunActor->SetEnabled(false);
	MoonActor->SetEnabled(true);

	if (ParameterCollection)
		ParameterCollection->SetScalarParameterValue(FName("IsNight"), 1.0f);
}

void ADaytimeManager::RefreshMaterial()
{
	if (!DynamicMaterial) return;
	DynamicMaterial->SetVectorParameterValue(FName("HorizonColor"),
	                                         HorizonColorCurve->GetClampedLinearColorValue(SunHeight));
	DynamicMaterial->SetVectorParameterValue(FName("ZenithColor"),
	                                         ZenithColorCurve->GetClampedLinearColorValue(SunHeight));
	DynamicMaterial->SetVectorParameterValue(FName("CloudsColor"),
	                                         CloudsColorCurve->GetClampedLinearColorValue(SunHeight));

	DynamicMaterial->SetScalarParameterValue(FName("HorizonFalloff"),
	                                         HorizonFalloffCurve.GetRichCurveConst()->Eval(SunHeight));

	const FRotator SunRotator = SunActor->GetActorRotation();
	DynamicMaterial->SetVectorParameterValue(FName("SunlightDirection"), SunRotator.Vector());
	DynamicMaterial->SetVectorParameterValue(FName("SunColor"), SunColor);
	DynamicMaterial->SetScalarParameterValue(FName("SunBrightness"), SunBrightness);

	const FRotator MoonRotator = MoonActor->GetActorRotation();
	DynamicMaterial->SetVectorParameterValue(FName("MoonlightDirection"), MoonRotator.Vector());
	DynamicMaterial->SetVectorParameterValue(FName("MoonColor"), MoonColor);
	DynamicMaterial->SetScalarParameterValue(FName("MoonBrightness"), MoonBrightness);

	DynamicMaterial->SetScalarParameterValue(FName("CloudSpeed"), CloudSpeed);
	DynamicMaterial->SetScalarParameterValue(FName("CloudOpacity"), CloudOpacity);

	const float StarOpacity = SunHeight < 0 ? FMath::Abs(SunHeight) : 0;
	DynamicMaterial->SetScalarParameterValue(FName("StarOpacity"), StarOpacity);
	DynamicMaterial->SetScalarParameterValue(FName("StarBrightness"), StarBrightness);
}

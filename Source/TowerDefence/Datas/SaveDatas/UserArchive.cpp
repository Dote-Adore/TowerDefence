#include "UserArchive.h"

#include "TowerDefence/GlobalConfig.h"

UUserArchive::UUserArchive(const FObjectInitializer& ObjectInitializer)
	:USaveGame(ObjectInitializer)
{
	LastAddPhysicalPointTime = FDateTime::MinValue();
}

int32 UUserArchive::GetCurrentPhysicalStrength()
{
	const UGlobalConfig* UGC = GetDefault<UGlobalConfig>();

	// 如果是首次的话
	if(LastAddPhysicalPointTime == FDateTime::MinValue())
	{
		LastAddPhysicalPointTime = FDateTime::Now();
		PhysicalStrength = UGC->MaxPhysicalStrength;
	}
	FTimespan Offset = FDateTime::Now() - LastAddPhysicalPointTime;
	int32 AddPoints = Offset.GetSeconds()/UGC->PhysicalStrengthRecoverySeconds;
	AddPoints = FMath::Clamp(AddPoints, 0, UGC->MaxPhysicalStrength);
	FTimespan NewPhysicalPointTime = FTimespan::FromSeconds(AddPoints*UGC->PhysicalStrengthRecoverySeconds);
	PhysicalStrength = FMath::Clamp(PhysicalStrength+AddPoints, 0, UGC->MaxPhysicalStrength);
	LastAddPhysicalPointTime = LastAddPhysicalPointTime + NewPhysicalPointTime;
	return PhysicalStrength;
}

int32 UUserArchive::GetMaxPhysicalStrength()
{
	const UGlobalConfig* UGC = GetDefault<UGlobalConfig>();
	return UGC->MaxPhysicalStrength;
}

bool UUserArchive::UsePhysicalStrength(int32 InPoints)
{
	GetCurrentPhysicalStrength();
	if(PhysicalStrength <InPoints)
		return false;

	PhysicalStrength -= InPoints;
	return true;
}

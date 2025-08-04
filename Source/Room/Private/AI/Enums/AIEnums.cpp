// AIEnums.cpp
#include "AI/Enums/AIEnums.h"

//AIEnums::AIEnums()
//{
//}
//
//AIEnums::~AIEnums()
//{
//}
void ForceUHTToInclude()
{
	// UHT가 모든 enum을 인식하게끔 강제 사용
	ECharacterMovementSpeedType Dummy1 = ECharacterMovementSpeedType::Walk;
	EAICombatType Dummy2 = EAICombatType::Melee;
	EAIStateType Dummy3 = EAIStateType::Idle;
	EAIAwarenessLevel Dummy4 = EAIAwarenessLevel::Unaware;
}
// Shoot Them Up game


#include "Player/STUPlayerState.h"

#include "GenericTeamAgentInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

void ASTUPlayerState::Loginfo()
{
	UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Death: %i "),TeamID, KillsNum, DeathNum );
}

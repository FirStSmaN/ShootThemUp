// Shoot Them Up game


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) 
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
	
}
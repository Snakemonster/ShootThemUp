// Shoot Them Up Game, All Rights Reserved.


#include "Components/STURespawnActorComponent.h"

#include "STUGameModeBase.h"

USTURespawnActorComponent::USTURespawnActorComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnActorComponent::Respawn(int32 RespawnTime) {
	if(!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnActorComponent::RespawnTimerUpdate, 1.f, true);
}

bool USTURespawnActorComponent::IsRespawnInProgress() const {
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USTURespawnActorComponent::RespawnTimerUpdate() {
	if(--RespawnCountDown == 0) {
		if(!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if(!GameMode) return;
		
		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}


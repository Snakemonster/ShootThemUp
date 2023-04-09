// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnActorComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const {
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnActorComponent>(GetOwningPlayer());
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}

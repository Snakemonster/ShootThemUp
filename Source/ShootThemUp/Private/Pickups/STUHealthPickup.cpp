// Shoot Them Up Game, All Rights Reserved.


#include "Pickups/STUHealthPickup.h"
#include "Components/STUHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickUpTo(APawn* PlayerPawn) {
	const auto HealthComponent = PlayerPawn->FindComponentByClass<USTUHealthComponent>();
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	
	return HealthComponent->TryToAddHealth(HealthAmount);
}

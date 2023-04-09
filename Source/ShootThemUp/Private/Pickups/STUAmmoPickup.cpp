// Shoot Them Up Game, All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickUpTo(APawn* PlayerPawn) {
	const auto HealthComponent = PlayerPawn->FindComponentByClass<USTUHealthComponent>();
	if(!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = PlayerPawn->FindComponentByClass<USTUWeaponComponent>();
	if(!WeaponComponent) return false;
	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}

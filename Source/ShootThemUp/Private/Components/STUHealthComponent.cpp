// Shoot Them Up Game, All Rights Reserved.


#include "Components/STUHealthComponent.h"
#include "STUGameModeBase.h"
#include "GameFramework/Character.h"
#include "Perception/AISense_Damage.h"


DEFINE_LOG_CATEGORY_STATIC(LogSTUHealthComponent, All, All)

USTUHealthComponent::USTUHealthComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount) {
	if (IsDead() || IsHealthFull()) return false;
	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull() const {
	return FMath::IsNearlyEqual(Health, MaxHealth);
}


// Called when the game starts
void USTUHealthComponent::BeginPlay() {
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner) {
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* InstigatedBy, AActor* DamageCauser) {
	UE_LOG(LogSTUHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                            FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
                                            FVector ShotFromDirection,
                                            const UDamageType* DamageType, AActor* DamageCauser) {
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	UE_LOG(LogSTUHealthComponent, Display, TEXT("On point damage: %f, final damage: %f, bone: %s"), Damage, FinalDamage,
	       *BoneName.ToString());
	ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                             FVector Origin, FHitResult HitInfo, AController* InstigatedBy,
                                             AActor* DamageCauser) {
	UE_LOG(LogSTUHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
	ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy) {
	if (Damage <= 0.f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead()) {
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}

	else if (AutoHeal) {
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime,
		                                       true, HealDelay);
	}
	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamageActor, const FName& BoneName) {
	const auto Character = Cast<ACharacter>(DamageActor);
	if (!Character ||
		!Character->GetMesh() ||
		!Character->GetMesh()->GetBodyInstance(BoneName))
		return 1.f;

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.f;

	return DamageModifiers[PhysMaterial];
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy) {
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
	
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage,
	                                   InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}

void USTUHealthComponent::HealUpdate() {
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld()) {
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth) {
	const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::PlayCameraShake() {
	if (IsDead())return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController) {
	if (!GetWorld()) return;

	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

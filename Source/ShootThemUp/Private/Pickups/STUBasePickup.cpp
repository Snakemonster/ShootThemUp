// Shoot Them Up Game, All Rights Reserved.


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasicPickup, All, All);

ASTUBasePickup::ASTUBasePickup() {
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);

}

void ASTUBasePickup::BeginPlay() {
	Super::BeginPlay();
	
	check(CollisionComponent);
	
	GenerateRotationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0.f, RotationYaw, 0.f));
}

bool ASTUBasePickup::CouldBeTaken() const {
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Pawn = Cast<APawn>(OtherActor);
	if (GivePickUpTo(Pawn)) {
		PickupWasTaken();
	}
}

bool ASTUBasePickup::GivePickUpTo(APawn* PlayerPawn) {
	return false;
}

void ASTUBasePickup::PickupWasTaken() {
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent()) {
		GetRootComponent()->SetVisibility(false, true);
	}
	
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpTakenSound, GetActorLocation());
}

void ASTUBasePickup::Respawn() {
	GenerateRotationYaw();
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	if (GetRootComponent()) {
		GetRootComponent()->SetVisibility(true, true);
	}
}

void ASTUBasePickup::GenerateRotationYaw() {
	const auto Direction = FMath::RandBool() ? 1.f : -1.f;
	RotationYaw = FMath::RandRange(1.f, 2.f) * Direction;
}

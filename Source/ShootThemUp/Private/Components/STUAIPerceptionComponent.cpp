// Shoot Them Up Game, All Rights Reserved.


#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const {
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	
	if (PerceiveActors.Num() == 0) {
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceiveActors);
		if (PerceiveActors.Num() == 0) return nullptr;
	}

	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for (const auto PerceiveActor : PerceiveActors) {
		const auto HealthComponent = PerceiveActor->FindComponentByClass<USTUHealthComponent>();

		const auto PerceivePawn = Cast<APawn>(PerceiveActor);
		if (!PerceivePawn) UE_LOG(LogClass, Error, TEXT("PerceivePawn is nullptr"));
		
		const auto AreEnemies = PerceivePawn && STUUtils::AreEnemies(Controller, PerceivePawn->Controller);
		
		if (HealthComponent && !HealthComponent->IsDead() && AreEnemies) {
			const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance) {
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}
	return BestPawn;
}

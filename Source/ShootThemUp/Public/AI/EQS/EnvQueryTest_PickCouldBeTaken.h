// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickCouldBeTaken.generated.h"

UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_PickCouldBeTaken : public UEnvQueryTest {
	GENERATED_BODY()
public:
	UEnvQueryTest_PickCouldBeTaken(const FObjectInitializer& ObjectInitializer);
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};

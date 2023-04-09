// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUGoToMenuWidget.h"
#include "STUGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMenuWidget, All, All);

void USTUGoToMenuWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (MainMenuButton) {
		MainMenuButton->OnClicked.AddDynamic(this, &USTUGoToMenuWidget::OnGoToMenu);
	}
}

void USTUGoToMenuWidget::OnGoToMenu() {
	if (!GetWorld()) return;	

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if (!STUGameInstance) return;

	if (STUGameInstance->GetMenuLevelName().IsNone()) {
		UE_LOG(LogSTUGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
		return;
	}
	
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}
#include "DamageTextWidget.h"
#include "Components/TextBlock.h"  // UTextBlock 사용

void UDamageTextWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 위젯이 생성될 때 자동으로 DamageText를 찾음
}

void UDamageTextWidget::SetDamageText(int32 Damage)
{
    if (DamageText)  // TextBlock이 존재하는지 확인
    {
        DamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Damage)));
    }
}

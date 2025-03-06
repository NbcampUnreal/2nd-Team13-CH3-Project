#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

class UTextBlock;  // 전방 선언

UCLASS()
class SHADOW_OF_THE_DESERT_API UDamageTextWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    void SetDamageText(int32 Damage);

private:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DamageText;  // 블루프린트에서 "DamageText"라는 이름의 TextBlock을 가져옴
};

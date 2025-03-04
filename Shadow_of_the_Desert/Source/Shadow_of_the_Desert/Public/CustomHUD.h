#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API ACustomHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void DrawHUD() override; // 조준점 그리기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
    float LineSize = 30.0f; // 기본 조준선 길이

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
    float Thickness = 2.0f; // 기본 조준선 두께

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair")
    FLinearColor CrosshairColor = FLinearColor::White; // 기본 색상
};

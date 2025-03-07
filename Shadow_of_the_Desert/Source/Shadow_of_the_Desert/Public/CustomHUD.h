#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CustomHUD.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API ACustomHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void DrawHUD() override; // 조준점 및 히트마커 그리기

public:
    // 히트마커 및 킬마커 활성화 함수
    void ShowHitmarker();
    void ShowKillmarker();  // 적 처치 시 호출

private:
    // 히트마커 관련 변수
    bool bShowHitmarker = false;
    float HitmarkerDuration = 0.2f; // 히트마커 유지 시간
    FTimerHandle HitmarkerTimerHandle; // 히트마커 제거용 타이머
    FLinearColor CurrentHitmarkerColor; // 현재 히트마커 색상

    // 킬마커 관련 변수
    bool bIsKillConfirmed = false;  // 적 처치 여부 확인
    float KillmarkerDuration = 0.3f; // 킬마커 유지 시간
    FTimerHandle KillmarkerTimerHandle; // 킬마커 제거용 타이머

    // 조준선 및 히트마커 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
    float LineSize = 25.0f; // 조준선 길이

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
    float Thickness = 2.0f; // 조준선 두께

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
    FLinearColor CrosshairColor = FLinearColor::Green; // 기본 조준선 색상

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitmarker", meta = (AllowPrivateAccess = "true"))
    FLinearColor HitmarkerColor = FColor::FromHex(TEXT("#00ffff")); // 히트마커 색상

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Killmarker", meta = (AllowPrivateAccess = "true"))
    FLinearColor KillmarkerColor = FLinearColor::Red; // 적 처치 시 조준선 색상
};

#include "CustomHUD.h"
#include "Engine/Canvas.h"
#include "TimerManager.h"

void ACustomHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas) return;

    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;

    // 히트마커 오프셋 설정
    const float HitmarkerOffset = 15.0f;  // 중앙에서 떨어지는 거리
    const float HitSize = LineSize * 0.6f; // 히트마커 크기 조절
    const float Offset = HitSize * 0.5f;

    // 킬마커가 활성화되었으면 빨간색, 아니면 기본 색상 사용
    FLinearColor CurrentCrosshairColor = bIsKillConfirmed ? KillmarkerColor : CrosshairColor;

    // 기본 조준선 그리기
    DrawRect(CurrentCrosshairColor, CenterX - Thickness * 0.5f, CenterY - LineSize * 0.5f, Thickness, LineSize); // 수직선
    DrawRect(CurrentCrosshairColor, CenterX - LineSize * 0.5f, CenterY - Thickness * 0.5f, LineSize, Thickness); // 수평선

     //히트마커 그리기
    if (bShowHitmarker)
    {
        DrawLine(CenterX - Offset - HitmarkerOffset, CenterY - Offset - HitmarkerOffset, CenterX - Offset / 2 - HitmarkerOffset, CenterY - Offset / 2 - HitmarkerOffset, HitmarkerColor, Thickness);
        DrawLine(CenterX + Offset + HitmarkerOffset, CenterY - Offset - HitmarkerOffset, CenterX + Offset / 2 + HitmarkerOffset, CenterY - Offset / 2 - HitmarkerOffset, HitmarkerColor, Thickness);
        DrawLine(CenterX - Offset - HitmarkerOffset, CenterY + Offset + HitmarkerOffset, CenterX - Offset / 2 - HitmarkerOffset, CenterY + Offset / 2 + HitmarkerOffset, HitmarkerColor, Thickness);
        DrawLine(CenterX + Offset + HitmarkerOffset, CenterY + Offset + HitmarkerOffset, CenterX + Offset / 2 + HitmarkerOffset, CenterY + Offset / 2 + HitmarkerOffset, HitmarkerColor, Thickness);
    }
}

void ACustomHUD::ShowHitmarker()
{
    bShowHitmarker = true;

    // 일정 시간 후 히트마커 숨기기
    GetWorldTimerManager().SetTimer(HitmarkerTimerHandle, [this]()
        {
            bShowHitmarker = false;
        }, HitmarkerDuration, false);
}

void ACustomHUD::ShowKillmarker()
{
    bIsKillConfirmed = true;

    // 일정 시간 후 원래 색상으로 복구
    GetWorldTimerManager().SetTimer(KillmarkerTimerHandle, [this]()
        {
            bIsKillConfirmed = false;
        }, KillmarkerDuration, false);
}

#include "CustomHUD.h"
#include "Engine/Canvas.h"
#include "TimerManager.h"

void ACustomHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas) return;

    const float CenterX = Canvas->ClipX * 0.5f;
    const float CenterY = Canvas->ClipY * 0.5f;

    // 기본 조준선 그리기
    DrawRect(CrosshairColor, CenterX - Thickness * 0.5f, CenterY - LineSize * 0.5f, Thickness, LineSize); // 수직선
    DrawRect(CrosshairColor, CenterX - LineSize * 0.5f, CenterY - Thickness * 0.5f, LineSize, Thickness); // 수평선

    // 히트마커 그리기
    if (bShowHitmarker)
    {
        const float HitSize = LineSize * 0.6f; // 히트마커 크기 조절
        const float Offset = HitSize * 0.5f;

        DrawLine(CenterX - Offset, CenterY - Offset, CenterX - Offset / 2, CenterY - Offset / 2, HitmarkerColor, Thickness);
        DrawLine(CenterX + Offset, CenterY - Offset, CenterX + Offset / 2, CenterY - Offset / 2, HitmarkerColor, Thickness);
        DrawLine(CenterX - Offset, CenterY + Offset, CenterX - Offset / 2, CenterY + Offset / 2, HitmarkerColor, Thickness);
        DrawLine(CenterX + Offset, CenterY + Offset, CenterX + Offset / 2, CenterY + Offset / 2, HitmarkerColor, Thickness);
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

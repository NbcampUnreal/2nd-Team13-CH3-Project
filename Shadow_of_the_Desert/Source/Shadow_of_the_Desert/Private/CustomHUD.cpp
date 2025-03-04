#include "CustomHUD.h"
#include "Engine/Canvas.h"  // Canvas 사용
#include "UObject/ConstructorHelpers.h"

void ACustomHUD::DrawHUD()
{
    Super::DrawHUD();

    if (Canvas)
    {
        const float CenterX = Canvas->ClipX * 0.5f;
        const float CenterY = Canvas->ClipY * 0.5f;

        // 수직선 (Vertical Line)
        DrawRect(CrosshairColor, CenterX - Thickness * 0.5f, CenterY - LineSize * 0.5f, Thickness, LineSize);

        // 수평선 (Horizontal Line)
        DrawRect(CrosshairColor, CenterX - LineSize * 0.5f, CenterY - Thickness * 0.5f, LineSize, Thickness);
    }
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceMatchingHUD.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

void ADistanceMatchingHUD::ShowDebugInfo(float& YL, float& YPos)
{
    Super::ShowDebugInfo(YL, YPos);

}

void ADistanceMatchingHUD::DrawHUD()
{
    Super::DrawHUD();

    if (bShowMovementPanel)
    {
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerCharacter)
        {
            UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();
            if (CharacterMovement)
            {
                FVector2D Offset(2.0f, 2.0f);
                FVector2D CenterPoint(PanelRadius, PanelRadius);
                CenterPoint += Offset;

                DrawRadialPanel(CharacterMovement->GetCurrentAcceleration(), CharacterMovement->GetMaxAcceleration(), CenterPoint, PanelRadius, "a");
                DrawRadialPanel(PlayerCharacter->GetVelocity(), CharacterMovement->GetMaxSpeed(), CenterPoint + FVector2D(PanelRadius * 2 + 10, 0), PanelRadius, "v");
            }
        }
    }
}

void ADistanceMatchingHUD::ShowMovementPanel()
{
    bShowMovementPanel = !bShowMovementPanel;
}

void ADistanceMatchingHUD::DrawRadialPanel(const FVector& VectorValue, float MaxMag, const FVector2D Center, float Radius, const FString& Caption, const FVector& North, const FVector& East)
{
    DrawDebugCanvas2DCircle(Canvas, Center, Radius, 32, FLinearColor::Gray);
    
    float CaptionScale = 3.0f;
    float CaptionWidth, CaptionHeight;
    GetTextSize(Caption, CaptionWidth, CaptionHeight, nullptr, CaptionScale);
    DrawText(Caption, FLinearColor::Gray, Center.X - CaptionWidth * 0.5f, Center.Y - Radius * 0.8f, nullptr, CaptionScale);

    float VectorMag = VectorValue.Size();
    FString VectorMagString = FString::Printf(TEXT("%.1f"), VectorMag);
    float VectorMagStringScale = 1.5f;
    float VectorMagStringWidth, VectorMagStringHeight;
    GetTextSize(VectorMagString, VectorMagStringWidth, VectorMagStringHeight, nullptr, VectorMagStringScale);
    DrawText(VectorMagString, VectorMag > 0 ? FLinearColor::Yellow : FLinearColor::Yellow * 0.5f, Center.X - VectorMagStringWidth * 0.5f, Center.Y + Radius * 0.2f, nullptr, VectorMagStringScale);

    if (VectorMag > 0)
    {
        FVector NormalizedVector = VectorValue / VectorMag;
        float Angle = FMath::Acos(FVector::DotProduct(NormalizedVector, North));
        if (FVector::DotProduct(NormalizedVector, East) < 0)
        {
            Angle = PI * 2 - Angle;
        }

        float LengthScale = VectorMag / MaxMag;
        FVector2D EndPoint = Center + FVector2D(FMath::Sin(Angle), -FMath::Cos(Angle)) * Radius * LengthScale;
        DrawDebugCanvas2DLine(Canvas, Center, EndPoint, FLinearColor::White, 3.0f);
    }
}
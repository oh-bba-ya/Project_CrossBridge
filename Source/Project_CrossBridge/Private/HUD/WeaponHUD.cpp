// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WeaponHUD.h"

void AWeaponHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;

	if(GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter = ViewportSize * 0.5f;

		if(HUDStruct.CrosshairCenter)
		{
			DrawCrossHair(HUDStruct.CrosshairCenter, ViewportCenter);
		}
		if(HUDStruct.CrosshairLeft)
		{
			DrawCrossHair(HUDStruct.CrosshairLeft, ViewportCenter);
		}
		if(HUDStruct.CrosshairRight)
		{
			DrawCrossHair(HUDStruct.CrosshairRight, ViewportCenter);
		}
		if(HUDStruct.CrosshairBottom)
		{
			DrawCrossHair(HUDStruct.CrosshairBottom, ViewportCenter);
		}
		if(HUDStruct.CrosshairTop)
		{
			DrawCrossHair(HUDStruct.CrosshairTop, ViewportCenter);
		}
	}
}

void AWeaponHUD::DrawCrossHair(UTexture2D* texture, FVector2D ViewportCenter)
{
	const float TextureWidth = texture->GetSizeX();
	const float TextureHeight = texture->GetSizeY();

	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth * 0.5f),
		ViewportCenter.Y - (TextureHeight* 0.5f)
		);

	DrawTexture(
		texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		FLinearColor::White
		);
}

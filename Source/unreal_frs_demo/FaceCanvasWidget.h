// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Face.h"
#include "Blueprint/UserWidget.h"

#include "FaceCanvasWidget.generated.h"

UCLASS()
class UNREAL_FRS_DEMO_API UFaceCanvasWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ImageTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FFace> Faces;

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};

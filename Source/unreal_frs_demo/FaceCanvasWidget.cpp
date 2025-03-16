// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceCanvasWidget.h"

#include "Slate/SlateBrushAsset.h"
#include "Engine/Texture2D.h"
#include "Rendering/DrawElements.h"
#include "Util/ColorConstants.h"

int32 UFaceCanvasWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                     const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                     int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FVector2D WidgetSize = AllottedGeometry.GetLocalSize();

	if (ImageTexture)
	{
		FVector2D TextureSize = FVector2D(ImageTexture->GetSizeX(), ImageTexture->GetSizeY());

		float Scale = FMath::Min(WidgetSize.X / TextureSize.X, WidgetSize.Y / TextureSize.Y);
		FVector2D ScaledSize = TextureSize * Scale;

		FVector2D Offset = (WidgetSize - ScaledSize) * 0.5f;

		FSlateBrush Brush;
		Brush.SetResourceObject(ImageTexture);
		Brush.ImageSize = ScaledSize;

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(Offset, ScaledSize),
			&Brush
		);

		LayerId++;

		for (const FFace &Face : Faces)
		{
			FVector2D RectPos = FVector2D(Face.X, Face.Y) * Scale + Offset;
			FVector2D RectSize = FVector2D(Face.Width, Face.Height) * Scale;

			FVector2D TopLeft = RectPos;
			FVector2D TopRight = FVector2D(RectPos.X + RectSize.X, RectPos.Y);
			FVector2D BottomRight = FVector2D(RectPos.X + RectSize.X, RectPos.Y + RectSize.Y);
			FVector2D BottomLeft = FVector2D(RectPos.X, RectPos.Y + RectSize.Y);

			TArray<FVector2D> LinePoints;
			LinePoints.Add(TopLeft);
			LinePoints.Add(TopRight);
			LinePoints.Add(BottomRight);
			LinePoints.Add(BottomLeft);
			LinePoints.Add(TopLeft);

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				LinePoints,
				ESlateDrawEffect::None,
				UE::Geometry::LinearColors::Green3b(),
				true,
				2.0f
			);			
		}


	}

	return LayerId + 1;
}

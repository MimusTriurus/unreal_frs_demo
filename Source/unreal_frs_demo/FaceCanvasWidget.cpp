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
			// draw faces rects
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
			// draw mesh keypoints
			if (bShowLandmarkDetectionInfo)
			{
				for (const auto& Point : Face.MeshKeypoints)
				{
					auto DrawingPoint = FVector2D(
						Point.X,
						Point.Y
						) * Scale + Offset;
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						LayerId,
						AllottedGeometry.ToPaintGeometry(DrawingPoint, FVector2D(4.0f, 4.0f)),
						FCoreStyle::Get().GetBrush("WhiteBrush"),
						ESlateDrawEffect::None,
						FLinearColor::Red
					);
				}
			}
			FString Sex = TEXT("Unknown");
			FString Emotion = TEXT("Unknown");
			if (bShowEmotionsRecognitionInfo)
			{
				Sex = Face.GenderIdx == 2 ? TEXT("Female") : TEXT("Male");
				switch (Face.EmotionIndex)
				{
				case 0:
					Emotion = TEXT("Angry");
					break;
				case 1:
					Emotion = TEXT("Disgusted");
					break;
				case 2:
					Emotion = TEXT("Fearful");
					break;
				case 3:
					Emotion = TEXT("Happy");
					break;
				case 4:
					Emotion = TEXT("Neutral");
					break;
				case 5:
					Emotion = TEXT("Sad");
					break;
				case 6:
					Emotion = TEXT("Surprised");
					break;
				}				
			}
			int32 Age = bShowAgeGenderInfo ? Face.Age : -1; 
			// draw meta
			const FString TextToDraw = FString::Printf(TEXT("%s. %s. %d years old"), *Sex, *Emotion, Age);
			FSlateFontInfo Font = FCoreStyle::Get().GetFontStyle("NormalFont");
			Font.Size = 18;
			FLinearColor TextColor = FLinearColor::White;

			FSlateDrawElement::MakeText(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(BottomLeft,  FVector2D(1, 1)),
				TextToDraw,
				Font,
				ESlateDrawEffect::None,
				TextColor
			);
			if (bShowHeadPoseEstimationInfo)
			{
				FVector2D HeadCenter = FVector2D(RectPos.X + RectSize.X / 2.0, RectPos.Y + RectSize.Y / 2.0f);

				// Rotation
				FRotator HeadRotation(Face.Pitch, Face.Yaw, Face.Roll);
				FMatrix RotationMatrix = FRotationMatrix(HeadRotation);
				float AxisLength = 50.0f;

				FVector XAxis = RotationMatrix.GetScaledAxis(EAxis::X) * AxisLength;
				FVector YAxis = RotationMatrix.GetScaledAxis(EAxis::Y) * AxisLength;
				FVector ZAxis = RotationMatrix.GetScaledAxis(EAxis::Z) * AxisLength;

				FVector2D XEnd = HeadCenter + FVector2D(XAxis.X, XAxis.Y);
				FVector2D YEnd = HeadCenter + FVector2D(YAxis.X, YAxis.Y);
				FVector2D ZEnd = HeadCenter + FVector2D(ZAxis.X, ZAxis.Y);

				FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
					{ HeadCenter, XEnd }, ESlateDrawEffect::None, FLinearColor::Red, true, 2.0f);
				FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
					{ HeadCenter, YEnd }, ESlateDrawEffect::None, FLinearColor::Green, true, 2.0f);
				FSlateDrawElement::MakeLines(OutDrawElements, LayerId, AllottedGeometry.ToPaintGeometry(),
					{ HeadCenter, ZEnd }, ESlateDrawEffect::None, FLinearColor::Blue, true, 2.0f);				
			}
		}
	}

	return LayerId + 1;
}

#pragma once
#include "Brushes/SlateColorBrush.h"

class ULevelInfomation;
class SMapCreatorPanel;
class SLevelCreatorPanel:public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLevelCreatorPanel){}
	SLATE_END_ARGS()

	void Construct(const SLevelCreatorPanel::FArguments& InArgs);
private:
	FSlateColorBrush MapCreatorBGBrush = FSlateColorBrush(FLinearColor::Black);
	FSlateColorBrush LevelPaddingBrush = FSlateColorBrush(FLinearColor(0,0,0,0));
	TSharedPtr<SMapCreatorPanel> MapCreatorPanel;
	

	void OnSelectionChangedFunc(ULevelInfomation* LevelInfomation, ESelectInfo::Type Type);
};
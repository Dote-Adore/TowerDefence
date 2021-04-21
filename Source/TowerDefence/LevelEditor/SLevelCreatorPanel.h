#pragma once
#include "Brushes/SlateColorBrush.h"

class ULevelInfomation;
class SMapCreatorPanel;
class SLevelList;
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
	TSharedPtr<SHorizontalBox> MainHorizontalBox;
	TSharedPtr<SWidget> InfoParamContainterWidget;
	TSharedPtr<SLevelList> LevelListWidget;
	
	// 新建一个LevelInfo的数据
	FString NewLevelName;
	int32 NewWidth;
	int32 NewHeight;
	
	void OnSelectionChangedFunc(ULevelInfomation* LevelInfomation, ESelectInfo::Type Type);
	FReply OnCreateNewLevelBtnClicked();


};
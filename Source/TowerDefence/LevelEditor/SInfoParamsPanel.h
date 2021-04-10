#pragma once

class ULevelInfomation;
//  每一个关卡文件属性的编辑
class SInfoParamsPanel:public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SInfoParamsPanel){}
	SLATE_END_ARGS()
public:
	void Construct(const SInfoParamsPanel::FArguments& InArgs, ULevelInfomation*LevelInfomation);

private:
	ULevelInfomation* CurrentLevelInfomation;
	TSharedPtr<SComboButton> BGSelectorSelctorButton;

	void OnBackgroundLevelSelected(const FAssetData& AssetData);
	bool OnBackgroundIsAssetAcceptableForDrop(const UObject* InObject);
	TSharedRef<SWidget> OnGetBGSelectorMenuContent();
	void OnBGSelectorMenuOpenChanged(bool bOpen);
};

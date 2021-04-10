#pragma once

class SEditableTextBox;
struct FEnemyGenerationInfo;
class ULevelInfomation;
class SWaveSettingsPanel:public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SWaveSettingsPanel){}
	SLATE_END_ARGS()
public:
	void Construct(const SWaveSettingsPanel::FArguments& InArgs,
		FEnemyGenerationInfo* EnemyGenerationInfo, ULevelInfomation* ParentLevelInfomation);
private:
	FEnemyGenerationInfo* CurrentWaveInfo;
	ULevelInfomation* CurrentLevelInfomation;
	TSharedPtr<SEditableTextBox> GrapTimeInputWidget;


	void OnGrapTimeInputChanged(const FText& InText);
};

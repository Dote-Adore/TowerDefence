#include "SWaveSettingsPanel.h"
#include "TowerDefence/Level/LevelInfomation.h"
#include "Widgets/Input/SEditableTextBox.h"
#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SWaveSettingsPanel::Construct(const SWaveSettingsPanel::FArguments& InArgs,
	FEnemyGenerationInfo* EnemyGenerationInfo, ULevelInfomation* ParentLevelInfomation)
{
	CurrentWaveInfo = EnemyGenerationInfo;
	CurrentLevelInfomation = ParentLevelInfomation;
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Grap Time", "Grap Time:"))
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		[
		    SAssignNew(GrapTimeInputWidget, SEditableTextBox)
		    .Text(FText::Format(LOCTEXT("Grap Input Text","{0}"), EnemyGenerationInfo->GrapTime))
		    .OnTextChanged(this, &SWaveSettingsPanel::OnGrapTimeInputChanged)
		]
	];
}

void SWaveSettingsPanel::OnGrapTimeInputChanged(const FText& InText)
{
	if(InText.IsNumeric())
	{
		float TargetInputFloat = FCString::Atof(*InText.ToString());
		CurrentWaveInfo->GrapTime = TargetInputFloat;
		CurrentLevelInfomation->MarkPackageDirty();
	}
	else
	{
		GrapTimeInputWidget->SetText(FText::Format(LOCTEXT("Grap Input Text","{0}"), CurrentWaveInfo->GrapTime));
	}
}

#undef LOCTEXT_NAMESPACE

#include "SInfoParamsPanel.h"

#include "PropertyCustomizationHelpers.h"
#include "TowerDefence/Level/LevelInfomation.h"
#include "EditorWidgets/Public/SAssetDropTarget.h"

#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SInfoParamsPanel::Construct(const SInfoParamsPanel::FArguments& InArgs, ULevelInfomation*LevelInfomation)
{
	this->CurrentLevelInfomation = LevelInfomation;
	TSharedPtr<FTextBlockStyle> CategoryTextStyle = MakeShared<FTextBlockStyle>(FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>( "NormalText" ));
	CategoryTextStyle->SetFontSize(15);
	ChildSlot
	[
		SNew(SBorder)
		.Padding(10)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("BaseInfomation", "Base Infomation"))
				.TextStyle(CategoryTextStyle.Get())
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STextBlock)
				.Text_Lambda([&]()->FText
				{
					if(CurrentLevelInfomation)
					{
                    return FText::Format(
                    	LOCTEXT("Width Height Info", "Width:{0}    Height:{1}" ),
                    	CurrentLevelInfomation->TileInfo.Witdh,
                    	CurrentLevelInfomation->TileInfo.Height );
                    }
					return FText();
				})
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Background Level Asset", "Background Level Asset: "))
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
						SAssignNew(BGSelectorSelctorButton, SComboButton)
						.ButtonStyle( FEditorStyle::Get(), "PropertyEditor.AssetComboStyle" )
						.ForegroundColor(FEditorStyle::GetColor("PropertyEditor.AssetName.ColorAndOpacity"))
						.OnGetMenuContent( this, &SInfoParamsPanel::OnGetBGSelectorMenuContent)
						.OnMenuOpenChanged( this, &SInfoParamsPanel::OnBGSelectorMenuOpenChanged)
						.ContentPadding(2.0f)
						.ButtonContent()
				        [
				        	SNew(STextBlock)
				        	.Text_Lambda([&]()->FText
				        	{
				        		if(!CurrentLevelInfomation->BackgroundWorld.IsNull())
				        			return FText::FromString(CurrentLevelInfomation->BackgroundWorld.GetAssetName());
				        		else
				        			return FText::FromString("None");
				        	})
				        ]
					//.OnAssetDropped(this, )
				]
			]
			// 设置波数
			+SVerticalBox::Slot()
			.AutoHeight()
			[
			SNew(STextBlock)
            .Text(LOCTEXT("WaveSettings", "Wave Settings"))
            .TextStyle(CategoryTextStyle.Get())
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[

			]
		]
	];
}

TSharedRef<SWidget> SInfoParamsPanel::OnGetBGSelectorMenuContent()
{
	 return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		FAssetData(),
		false,
		{UWorld::StaticClass()},
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses({UWorld::StaticClass()}),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateSP(this, &SInfoParamsPanel::OnBackgroundLevelSelected),
		FSimpleDelegate());
}

void SInfoParamsPanel::OnBGSelectorMenuOpenChanged(bool bOpen)
{
	if ( bOpen == false )
	{
		BGSelectorSelctorButton->SetMenuContent(SNullWidget::NullWidget);
	}
}

void SInfoParamsPanel::OnBackgroundLevelSelected(const FAssetData& AssetData)
{
	UE_LOG(LogTemp, Display, TEXT("OnBackgroundLevelSelected"));
	CurrentLevelInfomation->BackgroundWorld = TSoftObjectPtr<UWorld>(AssetData.ToSoftObjectPath());
	CurrentLevelInfomation->MarkPackageDirty();
	BGSelectorSelctorButton->SetIsOpen(false);
}

bool SInfoParamsPanel::OnBackgroundIsAssetAcceptableForDrop(const UObject* InObject)
{
	if(InObject->GetClass()->IsChildOf(UWorld::StaticClass()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

#undef LOCTEXT_NAMESPACE

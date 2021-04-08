#include "SMapCreatorPanel.h"

#define LOCTEXT_NAMESPACE "LevelCreatorEditor"

void SMapCreatorPanel::Construct(const SMapCreatorPanel::FArguments& InArgs)
{
	ChildSlot
[
    SNew(STextBlock)
    .Text(LOCTEXT("MapCreator", "MapCreator"))
];
}
#undef LOCTEXT_NAMESPACE
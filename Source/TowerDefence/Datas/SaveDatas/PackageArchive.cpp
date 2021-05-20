#include "PackageArchive.h"

void UPackageArchive::DeleteFood(int32 FoodID, int32 Num)
{
	int32* PreviousNum = Foods.Find(FoodID);
	if(PreviousNum!=nullptr)
	{
		(*PreviousNum) -= Num;
	}
	if((*PreviousNum)<=0)
	{
		Foods.Remove(FoodID);
	}
}

void UPackageArchive::AddFood(int32 FoodID, int32 Num)
{
	int32* PreviousNum = Foods.Find(FoodID);
	if(PreviousNum== nullptr)
	{
		Foods.Add(FoodID, Num);
	}
	else
	{
		(*PreviousNum) +=Num;
	}
}

void UPackageArchive::AddDevelopment(int32 DevelopID, int32 Num)
{
	int32* PreviousNum = Developments.Find(DevelopID);
	if(PreviousNum== nullptr)
	{
		Developments.Add(DevelopID, Num);
	}
	else
	{
		(*PreviousNum) +=Num;
	}
}

void UPackageArchive::DeleteDevelopment(int32 DevelopID, int32 Num)
{
	int32* PreviousNum = Developments.Find(DevelopID);
	if(PreviousNum!=nullptr)
	{
		(*PreviousNum) -= Num;
	}
	if((*PreviousNum)<=0)
	{
		Foods.Remove(DevelopID);
	}
}

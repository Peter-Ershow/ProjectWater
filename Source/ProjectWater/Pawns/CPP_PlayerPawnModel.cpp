// PlayerPawnModel.cpp

#include "CPP_PlayerPawnModel.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "ProjectWater/Instances/CPP_GameInstance.h"

float ACPP_PlayerPawnModel::GetElapsedTime() const
{
	const FDateTime CurrentTime = FDateTime::Now();
	const FTimespan ElapsedTime = CurrentTime - Cast<UCPP_GameInstance>(GetWorld()->GetGameInstance())->StartTime;
	return ElapsedTime.GetTotalSeconds();
}

// Generic function to export data to a CSV file
void ACPP_PlayerPawnModel::ExportDataToCsv(const FString& FilePrefix, const FString& Header, const FString& DataRow)
{
    FString CSVString;
    const FString FilePath = FPaths::ProjectDir() + "/Saved/" + FilePrefix + "_Data.csv";
    if (!FPaths::FileExists(FilePath))
    {
        CSVString = Header;
    }
    
    CSVString += DataRow;
    
    if (FFileHelper::SaveStringToFile(CSVString, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append))
    {
        UE_LOG(LogTemp, Log, TEXT("CSV file saved successfully: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save CSV file: %s"), *FilePath);
    }
}

void ACPP_PlayerPawnModel::ExportLookDataToCsv(FString ObjectName, float TimeLookedAt) const
{
    const float ElapsedSeconds = GetElapsedTime();
    const FString Header = "ObjectName,TimeLookedAt,ElapsedTime\n";
    const FString DataRow = FString::Printf(TEXT("%s,%.2f,%.2f\n"), *ObjectName, TimeLookedAt, ElapsedSeconds);
    ExportDataToCsv("LookData", Header, DataRow);
}

void ACPP_PlayerPawnModel::ExportGrabDataToCsv(FString ObjectName) const
{
    const float ElapsedSeconds = GetElapsedTime();
    const FString Header = "ObjectName,ElapsedTime\n";
    const FString DataRow = FString::Printf(TEXT("%s,%.2f\n"), *ObjectName, ElapsedSeconds);
    ExportDataToCsv("GrabData", Header, DataRow);
}

void ACPP_PlayerPawnModel::ExportLevelDataToCsv(FString StepName) const
{
    const float ElapsedSeconds = GetElapsedTime();
    const FString Header = "Step,ElapsedTime\n";
    const FString DataRow = FString::Printf(TEXT("%s,%.2f\n"), *StepName, ElapsedSeconds);
    ExportDataToCsv("StepsData", Header, DataRow);
}


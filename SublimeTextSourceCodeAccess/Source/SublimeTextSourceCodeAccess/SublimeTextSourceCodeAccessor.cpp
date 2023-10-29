#include "SublimeTextSourceCodeAccessor.h"
#include "Internationalization/Internationalization.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Windows/AllowWindowsPlatformTypes.h"

#define LOCTEXT_NAMESPACE "SublimeTextSourceCodeAccessor"

void FSublimeTextSourceCodeAccessor::RefreshAvailability() {
	// find sublime text executable path from windows registry
	FString OpenCommand;

	if (!FWindowsPlatformMisc::QueryRegKey(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Classes\\Applications\\sublime_text.exe\\shell\\open\\command\\"), TEXT(""), OpenCommand)) {
		FWindowsPlatformMisc::QueryRegKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Classes\\Applications\\sublime_text.exe\\shell\\open\\command\\"), TEXT(""), OpenCommand);
	}

	FString PatternString(TEXT("\"?(.*)\"? \".*\""));
	FRegexPattern Pattern(PatternString);
	FRegexMatcher Matcher(Pattern, OpenCommand);

	if (Matcher.FindNext()) {
		executablePath = Matcher.GetCaptureGroup(1);
	}

	sublimeTextExecutableFound = !executablePath.IsEmpty();
}

bool FSublimeTextSourceCodeAccessor::CanAccessSourceCode() const {
	return sublimeTextExecutableFound;
}

FName FSublimeTextSourceCodeAccessor::GetFName() const {
	return FName("SublimeTextSourceCodeAccessor");
}

FText FSublimeTextSourceCodeAccessor::GetNameText() const {
	return LOCTEXT("SublimeTextDisplayName", "Sublime Text");
}

FText FSublimeTextSourceCodeAccessor::GetDescriptionText() const {
	return LOCTEXT("SublimeTextDisplayDesc", "Create a c++ project with Sublime Text.");
}

bool FSublimeTextSourceCodeAccessor::OpenSolution() {
	if (!sublimeTextExecutableFound) {
		return false;
	}

	const FString Path = FString::Printf(TEXT("\"%s\""), *FPaths::ConvertRelativePathToFull(*FPaths::ProjectDir()));

	UE_LOG(LogTemp, Warning, TEXT("OPEN SOLUTION PATH:%s"), *Path)

	FPlatformProcess::CreateProc(*executablePath, *Path, true, true, false, nullptr, 0, nullptr, nullptr);

	return true;
}

bool FSublimeTextSourceCodeAccessor::OpenSolutionAtPath(const FString& InSolutionPath) {
	if (!sublimeTextExecutableFound) {
		return false;
	}

	FString CorrectSolutionPath = InSolutionPath;

	if (InSolutionPath.EndsWith(TEXT("UE5"))) {
		CorrectSolutionPath.LeftInline(CorrectSolutionPath.Len() - 3);
	}

	// UE5 passes the project folder and name, so strip the name off
	int32 LastPathIndex = CorrectSolutionPath.Find(TEXT("/"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

	if (LastPathIndex > -1) {
		CorrectSolutionPath.LeftInline(LastPathIndex + 1);
	}

	// make sure the path is wrapped in "" properly
	CorrectSolutionPath = FString::Printf(TEXT("\"%s%s.sublime-project\""), *CorrectSolutionPath, FApp::GetProjectName());

	UE_LOG(LogTemp, Warning, TEXT("OPEN SOLUTION AT PATH:%s"), *CorrectSolutionPath)

	return FPlatformProcess::CreateProc(*executablePath, *CorrectSolutionPath, true, true, false, nullptr, 0, nullptr, nullptr).IsValid();
}

bool FSublimeTextSourceCodeAccessor::DoesSolutionExist() const {
	FString Path = FPaths::Combine(*FPaths::ConvertRelativePathToFull(FPaths::ProjectDir()), FApp::GetProjectName(), TEXT(".sublime-project"));

	if (FPaths::FileExists(Path)) {
		return true;
	}

	return false;
}

bool FSublimeTextSourceCodeAccessor::OpenFileAtLine(const FString& FullPath, int32 LineNumber, int32 ColumnNumber) {
	if (!sublimeTextExecutableFound) {
		return false;
	}

	const FString Path = FString::Printf(TEXT("\"%s\" --line %d \"%s\""), *FPaths::ConvertRelativePathToFull(*FPaths::ProjectDir()), LineNumber, *FullPath);

	UE_LOG(LogTemp, Warning, TEXT("OPEN FILE AT LINE:%s"), *Path)

	FProcHandle Proc = FPlatformProcess::CreateProc(*executablePath, *Path, true, true, false, nullptr, 0, nullptr, nullptr);

	if (!Proc.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Opening file (%s) at a specific line failed."), *Path);
		FPlatformProcess::CloseProc(Proc);
		return false;
	}

	return true;
}

bool FSublimeTextSourceCodeAccessor::OpenSourceFiles(const TArray<FString>& AbsoluteSourcePaths) {
	if (!sublimeTextExecutableFound) {
		return false;
	}

	FString SourceFilesList = "";

	// build our paths based on what unreal sends to be opened
	for (const auto& SourcePath : AbsoluteSourcePaths) {
		SourceFilesList = FString::Printf(TEXT("%s \"%s\""), *SourceFilesList, *SourcePath);
	}

	// trim any whitespace on our source file list
	SourceFilesList.TrimStartInline();
	SourceFilesList.TrimEndInline();

	UE_LOG(LogTemp, Warning, TEXT("OPEN SOURCE FILES:%s"), *SourceFilesList)

	FProcHandle Proc = FPlatformProcess::CreateProc(*executablePath, *SourceFilesList, true, false, false, nullptr, 0, nullptr, nullptr);

	if (!Proc.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Opening the source file (%s) failed."), *SourceFilesList);
		FPlatformProcess::CloseProc(Proc);
		return false;
	}

	return true;
}

bool FSublimeTextSourceCodeAccessor::AddSourceFiles(const TArray<FString>& AbsoluteSourcePaths, const TArray<FString>& AvailableModules) {
	return false;
}

bool FSublimeTextSourceCodeAccessor::SaveAllOpenDocuments() const {
	return false;
}

#undef LOCTEXT_NAMESPACE
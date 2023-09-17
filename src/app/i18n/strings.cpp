#include <fstream>

#include "app/assets/assets.h"
#include "app/i18n/strings.hpp"
#include "fs/fs.hpp"
#include "log/log.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

static std::vector<String> LanguageFiles;
static UISTR_Arr Language;

const UISTR_Arr& UIString::Get() {
	return Language;
}

void UIString::UpdateEntries() {
	LanguageFiles.clear();
	Fs::ListDir(Fs::GetLanguagesDir().c_str(), [&](const char* entryName, bool isFile) -> bool {
		if (isFile) {
			LanguageFiles.push_back(entryName);
		}
		return true;
	});
}

void UIString::ListAll(OnListCB cb) {
	for (auto const& filePath : LanguageFiles) {
		cb(filePath.c_str());
	}
}

const char* GetPropertyName(UISTR i);
const char* GetEntryName(UISTR i);

char* string_dup(const char* const str) {
	if (str) {
		u32 len = strlen(str);
		char* s = new char[len + 1]();
		memcpy(s, str, len);
		return s;
	}

	return nullptr;
}

bool UIString::LoadFile(const String& filePath) {
	if (Fs::IsRegularFile(filePath) != 1) {
		return false;
	}

	std::ifstream f(filePath);
	json p = json::parse(f);

	for (u32 i = 0; i < UISTR::COUNT; i++) {
		if (Language[i] != nullptr)
			delete[] Language[i];

		Language[i] = string_dup(p[GetPropertyName((UISTR)i)][GetEntryName((UISTR)i)].get<String>().c_str());
	}

	return true;
}

void UIString::LoadDefault() {
	i32 sz = 0;
	const char* data = (const char*)assets_get("assets/languages/english.json", &sz);
	json p = json::parse(data, data + sz);

	for (u32 i = 0; i < UISTR::COUNT; i++) {
		if (Language[i] != nullptr)
			delete[] Language[i];

		Language[i] = string_dup(p[GetPropertyName((UISTR)i)][GetEntryName((UISTR)i)].get<String>().c_str());
	}
}

const char* GetEntryName(UISTR i) {
	switch (i) {
		case UISTR::Menu_File: return "File";
		case UISTR::MenuItem_New: return "New";
		case UISTR::MenuItem_Open: return "Open";

		case UISTR::Menu_Help: return "Help";
		case UISTR::MenuItem_About: return "About";
		case UISTR::MenuItem_GitHub: return "GitHub";

		case UISTR::Popup_OpenFile: return "OpenFile";

		case UISTR::Popup_NewDocument: return "NewDocument";
		case UISTR::Popup_NewDocument_WidthInput: return "WidthInput";
		case UISTR::Popup_NewDocument_HeightInput: return "HeightInput";
		case UISTR::Popup_NewDocument_OkButton: return "OkButton";
		case UISTR::Popup_NewDocument_CancelButton: return "CancelButton";

		case UISTR::Popup_AboutCsprite: return "AboutCsprite";
		case UISTR::Popup_AboutCsprite_Contrib_Header: return "ContributorHeader";
		case UISTR::Popup_AboutCsprite_Contrib_Paragraph: return "ContributorParagraph";
		case UISTR::Popup_AboutCsprite_Contrib_Link: return "ContributorLink";
		case UISTR::Popup_AboutCsprite_OpenSrcProjects_Header: return "OsProjectsHeader";
		case UISTR::Popup_AboutCsprite_OpenSrcProjects_Text: return "OsProjectsText";

		default: return "";
	}
}

const char* GetPropertyName(UISTR i) {
	switch (i) {
		case UISTR::Menu_File: return "FileMenu";
		case UISTR::MenuItem_New: return "FileMenu";
		case UISTR::MenuItem_Open: return "FileMenu";

		case UISTR::Menu_Help: return "HelpMenu";
		case UISTR::MenuItem_About: return "HelpMenu";
		case UISTR::MenuItem_GitHub: return "HelpMenu";

		case UISTR::Popup_OpenFile: return "OpenFilePopup";

		case UISTR::Popup_NewDocument: return "NewDocumentPopup";
		case UISTR::Popup_NewDocument_WidthInput: return "NewDocumentPopup";
		case UISTR::Popup_NewDocument_HeightInput: return "NewDocumentPopup";
		case UISTR::Popup_NewDocument_OkButton: return "NewDocumentPopup";
		case UISTR::Popup_NewDocument_CancelButton: return "NewDocumentPopup";

		case UISTR::Popup_AboutCsprite: return "AboutCspritePopup";
		case UISTR::Popup_AboutCsprite_Contrib_Header: return "AboutCspritePopup";
		case UISTR::Popup_AboutCsprite_Contrib_Paragraph: return "AboutCspritePopup";
		case UISTR::Popup_AboutCsprite_Contrib_Link: return "AboutCspritePopup";
		case UISTR::Popup_AboutCsprite_OpenSrcProjects_Header: return "AboutCspritePopup";
		case UISTR::Popup_AboutCsprite_OpenSrcProjects_Text: return "AboutCspritePopup";

		default: return "";
	}
}


#pragma once

#include "../../BetterEdit.hpp"

#include <BrownAlertDelegate.hpp>
#include <GDMake.h>
#include <InputNode.hpp>
#include <array>

// class MoreOptionsLayer : public FLAlertLayer,
//     TextInputDelegate,
//     GooglePlayDelegate
// {
//     protected:
//         bool init() {
//             return reinterpret_cast<bool(__fastcall*)(MoreOptionsLayer*)>(
//                 base + 0x1de8f0
//             )(this);
//         };
// };

bool shouldGetFoolAchievement();
void showedFoolAchievement();

using BE_Callback = void (*)(std::string const&);
using BE_Callback_B = void (*)(bool);

class BESettingsLayer : public BrownAlertDelegate, TextInputDelegate {
protected:
	unsigned int m_nItemCount = 0;
	int m_nCurrentPage = 0;
	unsigned int m_nDestPage = 0;
	std::vector<std::vector<CCNode*>> m_vPages;
	CCMenuItemSpriteExtra* m_pPrevPageBtn;
	CCMenuItemSpriteExtra* m_pNextPageBtn;
	EditorPauseLayer* m_pPauseLayer;

	static constexpr float const s_fItemPaddingV = 35.0f;
	static constexpr unsigned int const s_nMaxItemsOnPage = 10;

	void setup() override;
	void onClose(CCObject*);
	void keyDown(enumKeyCodes) override;

	CCPoint getItemPos(bool large = false, bool center = false);
	void incrementPageCount(bool skip = false);
	void addItem(CCNode* item);

	void addInput(
		char const* text, std::string const& value, BE_Callback cb,
		std::string const& filter = "0123456789"
	);
	void addToggle(
		char const* text, char const* desc, bool value, BE_Callback_B cb, bool experimental = false,
		bool showDescOnToggle = false
	);
	void addSlider(char const* text, SEL_MenuHandler onChange, float val = 0.0f);
	CCLabelBMFont* addTitle(char const* text, char const* font = "goldFont.fnt");
	CCLabelBMFont* addSubtitle(char const* text, bool centered = false);
	CCMenuItemSpriteExtra* addButton(CCNode* sprite, SEL_MenuHandler callback, bool large = false);
	CCMenuItemSpriteExtra* addButton(
		char const* label, SEL_MenuHandler callback, bool large = false
	);

	void onInfo(CCObject*);
	void onPage(CCObject*);
	void onToggle(CCObject*);
	void textChanged(CCTextInputNode*) override;

	void onShowAccount(CCObject*);
	void onURL(CCObject*);
	void onFool(CCObject*);

public:
	static BESettingsLayer* create(EditorPauseLayer*);
};

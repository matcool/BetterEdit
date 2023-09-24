#include "loadVisibilityTab.hpp"

#include "../../hooks/EditorPauseLayer.hpp"
#include "../RotateSaws/rotateSaws.hpp"
#include "VisibilityToggle.hpp"

// for patching stuff
static constexpr float const f_96 = 96.0f;
static constexpr float const f_0 = 0.0f;

bool g_bShowPosLine = true;
bool g_bDashLines = true;

class CCVoidPointer : public CCObject {
public:
	CCVoidPointer(void* v) :
		m_pValue(v) {}

	void* getValue() const {
		return m_pValue;
	}

	static CCVoidPointer* create(void* v) {
		CCVoidPointer* pRet = new CCVoidPointer(v);
		pRet->autorelease();
		return pRet;
	}

private:
	void* m_pValue;
};

enum VisibilityButtonType {
	kVisibilityNone,
	kVisibilityFunc,
	kVisibilityBool,
	kVisibilityBEFunc,
	kVisibilityGMVar,
	kVisibilityGMBool,
	kVisibilityAddress,
};

#define UPD_IMG(_var_, _tag_, _tvar_)                                          \
	if (_var_)                                                                 \
		as<ButtonSprite*>(as<CCMenuItemSpriteExtra*>(_var_)->getNormalImage()) \
			->updateBGImage(_tvar_ == _tag_ ? "GJ_button_02.png" : "GJ_button_01.png");

class VisibilityToggleCallbacks : public EditorUI {
public:
	void onToggle(CCObject* pSender) {
		auto data = as<bool*>(as<CCNode*>(pSender)->getUserData());

		*data = !as<CCMenuItemToggler*>(pSender)->isToggled();
	}

	// void onToggleGMVar(CCObject* pSender) {
	// 	auto var = as<CCString*>(as<CCNode*>(pSender)->getUserObject())->getCString();

	// 	GameManager::sharedState()->toggleGameVariable(var);

	// 	this->m_editorLayer->updateEditorMode();
	// }

	void onToggleBEFunc(CCObject* pSender) {
		auto func = as<void (*)(bool)>(as<CCNode*>(pSender)->getUserData());

		func(!as<CCMenuItemToggler*>(pSender)->isToggled());
	}

	void onToggleAddr(CCObject* pSender) {
		auto addr = as<uintptr_t>(as<CCNode*>(pSender)->getUserData());

		if (!as<CCMenuItemToggler*>(pSender)->isToggled()) {
			unpatch(addr);
		}
		else {
			patch(addr, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
		}
	}
};

ButtonSprite* createBtnSprite(char const* file) {
	auto spr = ButtonSprite::create(
		createBESprite(file), 0x32, true, 0x32, .6f, 0, "GJ_button_01.png", true
	);

	return spr;
}

void makeVisibilityPatches() {
	// TODO: fun patches
	// if (BetterEdit::getDisableVisibilityTab()) {
	// 	unpatch(0x768ed);
	// 	unpatch(0x8c9f8);
	// 	unpatch(0x7af6b);
	// 	unpatch(0x7cc7b);

	// 	unpatch(0x16e218);

	// 	return;
	// }

	// patch(0x768ed, intToBytes(as<int>(&f_96)), true);
	// patch(0x8c9f8, intToBytes(as<int>(&f_0)), true);
	// patch(0x7af6b, intToBytes(as<int>(&f_0)), true);
	// patch(0x7cc7b, intToBytes(as<int>(&f_0)), true);

	// patch(
	// 	0x16e218,
	// 	{
	// 		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	// 		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
	// 	},
	// 	true
	// );
}

void loadVisibilityTab(EditorUI* self) {
	BetterEdit::saveGlobalBool("pos-line", &g_bShowPosLine);
	BetterEdit::saveGlobalBool("dash-lines", &g_bDashLines);

	if (BetterEdit::getDisableVisibilityTab()) {
		return;
	}

	static constexpr float const item_sep = 43.0f;

	auto scr = self->m_buildModeBtn->getParent()->convertToNodeSpace(
		{ CCDirector::sharedDirector()->getScreenLeft() + 25.0f,
		  CCDirector::sharedDirector()->getScreenBottom() + 23.0f }
	);

	self->m_buildModeBtn->setNormalImage(createBtnSprite("BE_create_01.png"));
	self->m_buildModeBtn->setContentSize(
		self->m_buildModeBtn->getNormalImage()->getScaledContentSize()
	);
	self->m_buildModeBtn->setPosition(scr.x, scr.y + item_sep);

	self->m_editModeBtn->setNormalImage(createBtnSprite("BE_edit_01.png"));
	self->m_editModeBtn->setContentSize(self->m_editModeBtn->getNormalImage()->getScaledContentSize(
	));
	self->m_editModeBtn->setPosition(scr.x + item_sep, scr.y + item_sep);

	self->m_deleteModeBtn->setNormalImage(createBtnSprite("BE_delete_01.png"));
	self->m_deleteModeBtn->setContentSize(
		self->m_deleteModeBtn->getNormalImage()->getScaledContentSize()
	);
	self->m_deleteModeBtn->setPosition(scr.x, scr.y);

	auto vTabBtn = CCMenuItemSpriteExtra::create(
		createBtnSprite("BE_visible_01.png"), self, (SEL_MenuHandler)&EditorUI::toggleMode
	);

	vTabBtn->setTag(4);
	vTabBtn->setPosition(scr.x + item_sep, scr.y);

	self->m_buildModeBtn->getParent()->addChild(vTabBtn);

	UPD_IMG(self->m_buildModeBtn, 2, self->m_selectedMode);
	UPD_IMG(self->m_deleteModeBtn, 1, self->m_selectedMode);
	UPD_IMG(self->m_editModeBtn, 3, self->m_selectedMode);
	UPD_IMG(self->m_buildModeBtn->getParent()->getChildByTag(4), 4, self->m_selectedMode);

	auto btns = CCArray::create();

	// TODO: reenable?
	// btns->addObject(VisibilityToggle::create(
	// 	"BE_v_rotate.png",
	// 	[]() -> bool {
	// 		return shouldRotateSaw();
	// 	},
	// 	[&](bool s, auto p) -> void {
	// 		reinterpret_cast<EditorPauseLayer_CB*>(self)->onRotateSaws(p);
	// 	}
	// ));

	btns->addObject(VisibilityToggle::create(
		"BE_v_pulse.png",
		[]() -> bool {
			return BetterEdit::getPulseObjectsInEditor();
		},
		[](bool b, auto) -> void {
			BetterEdit::setPulseObjectsInEditor(b);
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_prevmode.png",
		[self]() -> bool {
			return GameManager::sharedState()->getGameVariable("0036");
		},
		[self](bool b, auto) -> void {
			GameManager::sharedState()->setGameVariable("0036", b);

			self->m_editorLayer->updateEditorMode();
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_bpm_line.png",
		[self]() -> bool {
			return GameManager::sharedState()->m_showSongMarkers;
		},
		[self](bool b, auto) -> void {
			GameManager::sharedState()->m_showSongMarkers = b;
		}
	));

	btns->addObject(VisibilityToggle::create(
						"BE_v_pos_line.png",
						[self]() -> bool {
							return g_bShowPosLine;
						},
						[self](bool b, auto) -> void {
							g_bShowPosLine = b;

							if (g_bShowPosLine) {
								unpatch(0x16e310);
							}
							else {
								patch(0x16e310, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 });
							}
						}
	)->invokeSetter());

	btns->addObject(VisibilityToggle::create(
		"BE_v_dur_line.png",
		[self]() -> bool {
			return GameManager::sharedState()->getGameVariable("0058");
		},
		[self](bool b, auto) -> void {
			GameManager::sharedState()->setGameVariable("0058", b);

			self->m_editorLayer->updateOptions();
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_eff_line.png",
		[self]() -> bool {
			return GameManager::sharedState()->getGameVariable("0043");
		},
		[self](bool b, auto) -> void {
			GameManager::sharedState()->setGameVariable("0043", b);

			self->m_editorLayer->updateOptions();
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_grnd.png",
		[self]() -> bool {
			return GameManager::sharedState()->getGameVariable("0037");
		},
		[self](bool b, auto) -> void {
			GameManager::sharedState()->setGameVariable("0037", b);

			self->m_editorLayer->updateOptions();
			self->m_editorLayer->m_groundLayer->setVisible(b);
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_grid.png",
		[self]() -> bool {
			return GameManager::sharedState()->getGameVariable("0038");
		},
		[self](bool b, auto) -> void {
			GameManager::sharedState()->setGameVariable("0038", b);

			self->m_editorLayer->updateOptions();
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_highlight.png",
		[]() -> bool {
			return BetterEdit::getHighlightTriggers();
		},
		[](bool value, auto) -> void {
			BetterEdit::setHighlightTriggers(value);
		}
	));

	btns->addObject(VisibilityToggle::create(
		"BE_v_dash.png",
		[]() -> bool {
			return g_bDashLines;
		},
		[](bool b, auto) -> void {
			g_bDashLines = b;
		}
	));

	auto buttonBar = EditButtonBar::create(
		btns, { CCDirector::sharedDirector()->getWinSize().width / 2, 86.0f },
		self->m_tabsArray->count(), false, GameManager::sharedState()->getIntGameVariable("0049"),
		GameManager::sharedState()->getIntGameVariable("0050")
	);
	buttonBar->setTag(VIEWBUTTONBAR_TAG);
	buttonBar->setVisible(self->m_selectedMode == 4);

	self->addChild(buttonBar, 10);
}

void updateVisibilityTab(EditorUI* self) {
	if (BetterEdit::getDisableVisibilityTab()) {
		return;
	}

	auto bbar = as<EditButtonBar*>(self->getChildByTag(VIEWBUTTONBAR_TAG));

	if (!bbar) {
		return;
	}

	bbar->reloadItemsInNormalSize();

	CCARRAY_FOREACH_B_TYPE(bbar->m_buttonArray, btn, VisibilityToggle) {
		btn->updateState();
	}
}

void showVisibilityTab(EditorUI* self, bool show) {
	CATCH_NULL(as<EditButtonBar*>(self->getChildByTag(VIEWBUTTONBAR_TAG)))
		->setVisible(show && self->m_selectedMode == 4);

	CATCH_NULL(self->m_buildModeBtn->getParent()->getChildByTag(4))->setVisible(show);
}

bool shouldShowDashLines() {
	return g_bDashLines;
}

#include <Geode/modify/EditorUI.hpp>

class $modify(EditorUI) {
	void toggleMode(CCObject* pSender) {
		if (!pSender) {
			return;
		}

		if (!m_buildModeBtn->getParent()->getChildByTag(4) ||
			BetterEdit::getDisableVisibilityTab()) {
			return EditorUI::toggleMode(pSender);
		}

		auto tag = pSender->getTag();

		if (m_selectedMode != tag) {
			m_selectedMode = tag;

			UPD_IMG(m_buildModeBtn, 2, tag);
			UPD_IMG(m_deleteModeBtn, 1, tag);
			UPD_IMG(m_editModeBtn, 3, tag);
			UPD_IMG(m_buildModeBtn->getParent()->getChildByTag(4), 4, tag);

			// manually doing EditorUI::resetUI
			// because that replaces the sprites
			// and we don't want that

			m_deleteMenu->setVisible(tag == 1);
			this->updateDeleteMenu();
			this->updateCreateMenu(false);
			m_editButtonBar->setVisible(tag == 3);
			this->updateGridNodeSize();
			this->getChildByTag(VIEWBUTTONBAR_TAG)->setVisible(tag == 4);
		}
	}
};

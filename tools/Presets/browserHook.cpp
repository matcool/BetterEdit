#include "../../BetterEdit.hpp"
#include "PresetLayer.hpp"

#include <InputPrompt.hpp>

using namespace gdmake;

class EditLevelLayer_CB : public EditLevelLayer {
public:
	void onCreatePreset(cocos2d::CCObject* pSender) {
		auto lvl = extra::as<GJGameLevel*>(extra::as<cocos2d::CCNode*>(pSender)->getUserObject());

		InputPrompt::create(
			"Save Preset", "Preset Name",
			[this, lvl](char const* name) -> void {
				if (name && strlen(name)) {
					BetterEdit::sharedState()->addPreset({ name, lvl->m_levelString });

					this->addChild(TextAlertPopup::create("Preset Created!", .5f, .6f), 100);
				}
			},
			"Save"
		)
			->show();
	}
};

void setupCreatePresetButton(EditLevelLayer* self, GJGameLevel* level) {
	auto btn = CCMenuItemSpriteExtra::create(
		CCNodeConstructor().fromFrameName("GJ_plusBtn_001.png").scale(.6f).done(), self,
		(cocos2d::SEL_MenuHandler)&EditLevelLayer_CB::onCreatePreset
	);
	btn->setUserObject(level);

	cocos2d::CCMenu* menu = static_cast<CCMenu*>(self->getChildByID("folder-menu"));

	if (!menu) {
		menu = self->m_buttonMenu;
	}

	menu->addChild(btn);
	menu->updateLayout();
}

#include <Geode/modify/LevelBrowserLayer.hpp>

class $modify(LevelBrowserLayer) {
	void onNew(cocos2d::CCObject* pSender) {
		if (BetterEdit::sharedState()->getPresets().size()) {
			PresetLayer::create()->show();
		}
		else {
			LevelBrowserLayer::onNew(pSender);
		}
	}
};

#include "autoCT.hpp"

#include "../../BetterEdit.hpp"
#include "ColorTriggerPopup.hpp"

using namespace gd;
using namespace gdmake;
using namespace gdmake::extra;
using namespace cocos2d;

class EditorPauseLayer_CB : public EditorPauseLayer {
public:
	void onAutoColorTriggers(CCObject* pSender) {
		ColorTriggerPopup::create(this)->show();
	}
};

void loadColorTriggerButton(EditorPauseLayer* self) {
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto menu = static_cast<CCMenu*>(self->getChildByID("actions-menu"));

	auto btn = CCMenuItemSpriteExtra::create(
		CCNodeConstructor()
			.fromNode(ButtonSprite::create(
				"Create Color\nTriggers", 0x32, true, "bigFont.fnt", "GJ_button_04.png", 30.0, .6f
			))
			.done(),
		self, (SEL_MenuHandler)&EditorPauseLayer_CB::onAutoColorTriggers
	);
	menu->addChild(btn);
	menu->updateLayout();
}

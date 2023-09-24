#include "../tools/AutoSave/Backup/setupBackupButton.hpp"
#include "../tools/CustomStartPos/ellHook.hpp"
#include "../tools/Presets/browserHook.hpp"

#include <GDMake.h>

using namespace gdmake;

#include <Geode/modify/EditLevelLayer.hpp>

class $modify(EditLevelLayer) {
	bool init(GJGameLevel* level) {
		if (!EditLevelLayer::init(level)) {
			return false;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		// TODO: im lazy to position this
		// auto garageRope = CCMenuItemSpriteExtra::create(
		// 	CCSprite::createWithSpriteFrameName("garageRope_001.png"), this,
		// 	(SEL_MenuHandler)&LevelInfoLayer::onGarage
		// );
		// garageRope->setSizeMult(1.2f);
		// garageRope->useAnimationType(MenuAnimationType::Move);
		// garageRope->setPosition(winSize.width / 2 - 80.0f, winSize.height / 2);
		// garageRope->setOffset({ .2f, .2f });
		// garageRope->setDestination({ 0.0f, -8.0f });

		// this->m_buttonMenu->addChild(garageRope);

		setupCreatePresetButton(this, level);
		// loadStartPosButton(this, level);
		// TODO: do people use this?
		// setupBackupButton(this, level);

		return true;
	}
};
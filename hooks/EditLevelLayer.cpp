#include <GDMake.h>
#include "../tools/Presets/browserHook.hpp"
#include "../tools/CustomStartPos/ellHook.hpp"
#include "../tools/AutoSave/Backup/setupBackupButton.hpp"

using namespace gdmake;

bool  EditLevelLayer_init(EditLevelLayer* self,  GJGameLevel* level) {
    if (!matdash::orig<&EditLevelLayer_init>(self,  level))
        return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    auto garageRope = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("garageRope_001.png"),
        self,
        (SEL_MenuHandler)&LevelInfoLayer::onGarage
    );
    garageRope->setSizeMult(1.2f);
    garageRope->useAnimationType(kMenuAnimationTypeMove);
    garageRope->setPosition(winSize.width / 2 - 80.0f, winSize.height / 2);
    garageRope->setOffset({ .2f, .2f });
    garageRope->setDestination({ 0.0f, -8.0f });

    self->m_pButtonMenu->addChild(garageRope);

    setupCreatePresetButton(self, level);
    // loadStartPosButton(self, level);
    // setupBackupButton(self, level);

    return true;
} MAT_GDMAKE_HOOK(0x6f5d0, EditLevelLayer_init);

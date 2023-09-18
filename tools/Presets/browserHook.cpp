#include "../../BetterEdit.hpp"
#include "PresetLayer.hpp"
#include <InputPrompt.hpp>

using namespace gdmake;

class EditLevelLayer_CB : public EditLevelLayer {
    public:
        void onCreatePreset(cocos2d::CCObject* pSender) {
            auto lvl = extra::as<GJGameLevel*>(
                extra::as<cocos2d::CCNode*>(pSender)->getUserObject()
            );

            InputPrompt::create("Save Preset", "Preset Name", [this, lvl](const char* name) -> void {
                if (name && strlen(name)) {
                    BetterEdit::sharedState()->addPreset({
                        name, lvl->m_levelString
                    });

                    this->addChild(TextAlertPopup::create("Preset Created!", .5f, .6f), 100);
                }
            }, "Save")->show();
        }
};

void setupCreatePresetButton(EditLevelLayer* self, GJGameLevel* level) {
    auto btn = CCMenuItemSpriteExtra::create(
        CCNodeConstructor()
            .fromFrameName("GJ_plusBtn_001.png")
            .scale(.6f)
            .done(),
        self,
        (cocos2d::SEL_MenuHandler)&EditLevelLayer_CB::onCreatePreset
    );
    btn->setUserObject(level);

    cocos2d::CCMenu* menu = nullptr;

    CCARRAY_FOREACH_B_TYPE(self->getChildren(), obj, cocos2d::CCNode)
        if (dynamic_cast<cocos2d::CCMenu*>(obj) && obj->getChildrenCount() > 4)
            menu = extra::as<cocos2d::CCMenu*>(obj);
        
    if (!menu)
        menu = self->m_buttonMenu;

    auto folderBtn = extra::getChild<CCMenuItemSpriteExtra*>(menu, -1);

    btn->setPosition(
        folderBtn->getPositionX() + 35.0f,
        folderBtn->getPositionY()
    );

    menu->addChild(btn);
}

void  LevelBrowserLayer_onNew(LevelBrowserLayer* self,  cocos2d::CCObject* pSender) {
    if (BetterEdit::sharedState()->getPresets().size())
        PresetLayer::create()->show();
    else
        matdash::orig<&LevelBrowserLayer_onNew>(self,  pSender);
} MAT_GDMAKE_HOOK(0x15cbf0, LevelBrowserLayer_onNew);

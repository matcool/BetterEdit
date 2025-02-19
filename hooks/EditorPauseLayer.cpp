#include <GDMake.h>
#include "EditorPauseLayer.hpp"
#include "../tools/settings/BESettingsLayer.hpp"
#include "../tools/AutoColorTriggers/autoCT.hpp"
#include "../tools/LevelPercent/levelPercent.hpp"
#include "../tools/IDRemap/remapHook.hpp"
#include "../tools/PasteString/loadPasteButton.hpp"
#include "../tools/RotateSaws/rotateSaws.hpp"
#include "../tools/AutoSave/autoSave.hpp"
#include "../tools/VisibilityTab/loadVisibilityTab.hpp"

using namespace gdmake;

bool g_bRotateSaws = false;
bool g_bShowKeybinds = false;

bool shouldRotateSaw() { return g_bRotateSaws; }
void enableRotations(bool b) { g_bRotateSaws = b; }
void setupRotateSaws() { BetterEdit::saveGlobalBool("rotate-saws", &g_bRotateSaws); }

CCMenuItemSpriteExtra* createNewPlayBtn(
    EditorPauseLayer* self, const char* sprName, SEL_MenuHandler cb, float scale = 1.f
) {
    auto spr = CCScale9Sprite::create(
        "GJ_button_01.png", { 0, 0, 40, 40 }
    );
    spr->setContentSize({ 65.f, 65.f });
    auto addSpr = CCSprite::createWithSpriteFrameName(sprName);
    addSpr->setPosition(spr->getContentSize() / 2);
    addSpr->setScale(scale);
    spr->addChild(addSpr);
    auto btn = CCMenuItemSpriteExtra::create(
        spr, self, cb
    );
    return btn;
}

void EditorPauseLayer_CB::onBESettings(cocos2d::CCObject* pSender) {
    BESettingsLayer::create(this)->show();
}

void EditorPauseLayer_CB::onShowKeybinds(CCObject* pSender) {
    g_bShowKeybinds = !as<CCMenuItemToggler*>(pSender)->isToggled();
}

void EditorPauseLayer_CB::onRotateSaws(CCObject* pSender) {
    g_bRotateSaws = !as<CCMenuItemToggler*>(pSender)->isToggled();

    if (g_bRotateSaws)
        beginRotations(LevelEditorLayer::get());
    else
        stopRotations(LevelEditorLayer::get());
}

int countLDMObjects(LevelEditorLayer* lel) {
    int count = 0;
    CCARRAY_FOREACH_B_TYPE(lel->getAllObjects(), obj, GameObject) {
        if (obj->m_bHighDetail)
            count++;
    }
    return count;
}

void  EditorPauseLayer_keyDown(EditorPauseLayer* self,  enumKeyCodes key) {
    if (key == KEY_Escape)
        as<EditorPauseLayer*>(as<uintptr_t>(self) - 0xf8)->onResume(nullptr);
    else
        matdash::orig<&EditorPauseLayer_keyDown>(self,  key);
} MAT_GDMAKE_HOOK(0x758d0, EditorPauseLayer_keyDown);

void  EditorPauseLayer_onResume(EditorPauseLayer* self,  CCObject* pSender) {
    matdash::orig<&EditorPauseLayer_onResume>(self,  pSender);

    for (auto const& addr : std::initializer_list<int> {
        0x73169,
        0x856A4, 
        0x87B17,
        0x87BC7,
        0x87D95,
        0x880F4,
        0x160B06,
    })
        if (BetterEdit::getBypassObjectLimit())
            patch(addr, { 0xff, 0xff, 0xff, 0x7f });
        else unpatch(addr);
    
    if (BetterEdit::getBypassObjectLimit()) {
        patch(0x7A100, { 0xeb });
        patch(0x7A022, { 0xeb });
        patch(0x7A203, { 0x90, 0x90 });
    } else {
        unpatch(0x7A100);
        unpatch(0x7A022);
        unpatch(0x7A203);
    }

    if (BetterEdit::getUseUpArrowForGameplay()) {
        patch(0x91abb, { 0x3d, 0x26, 0x00 });
        patch(0x921a6, { 0x3d, 0x26, 0x00 });
    } else {
        unpatch(0x91abb);
        unpatch(0x921a6);
    }

    auto ui = LevelEditorLayer::get()->getEditorUI();

    updateVisibilityTab(ui);

    // if (getAutoSaveTimer(ui)->cancellable())
    //     getAutoSaveTimer(ui)->cancel();
        
    // getAutoSaveTimer(ui)->resume();

    updatePercentLabelPosition(ui);
    // showPositionLabel(LevelEditorLayer::get()->getEditorUI(), true);
} MAT_GDMAKE_HOOK(0x74fe0, EditorPauseLayer_onResume);

void  EditorPauseLayer_onExitEditor(
    EditorPauseLayer* self,
    
    CCObject* pSender
) {
    stopRotations(self->m_pEditorLayer);
    // resetAutoSaveTimer(self->m_pEditorLayer->m_pEditorUI);

    matdash::orig<&EditorPauseLayer_onExitEditor>(self,  pSender);

    self->removeFromParentAndCleanup(true);
} MAT_GDMAKE_HOOK(0x75660, EditorPauseLayer_onExitEditor);

bool  EditorPauseLayer_init(
    EditorPauseLayer* self,
    
    LevelEditorLayer* el
) {
    if (!matdash::orig<&EditorPauseLayer_init>(self,  el))
        return false;

    auto menu = as<CCMenu*>(self->m_pButton0->getParent());
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    auto mainMenu = as<CCMenu*>(self->getChildren()->objectAtIndex(0));

    if (BetterEdit::isEditorViewOnlyMode()) {
        CCARRAY_FOREACH_B_BASE(mainMenu->getChildren(), btn, CCMenuItemSpriteExtra*, ix) {
            if (!ix) continue;

            btn->setEnabled(false);
            as<ButtonSprite*>(btn->getNormalImage())->setCascadeColorEnabled(true);
            as<ButtonSprite*>(btn->getNormalImage())->setCascadeOpacityEnabled(true);
            as<ButtonSprite*>(btn->getNormalImage())->setColor({ 90, 90, 90 });
            as<ButtonSprite*>(btn->getNormalImage())->setOpacity(200);
        }
    }

    auto gdSettingsBtn = getChild<CCMenu*>(menu, menu->getChildrenCount() - 1);

    auto btn = CCMenuItemSpriteExtra::create(
        CCNodeConstructor()
            .fromFrameName("GJ_optionsBtn02_001.png")
            .scale(.8f)
            .color(ccc3(255, 0, 0))
            .done(),
        self,
        (SEL_MenuHandler)&EditorPauseLayer_CB::onBESettings
    );
    btn->setPosition(winSize.width / 2 - 70, gdSettingsBtn->getPositionY());
    menu->addChild(btn);

    gdSettingsBtn->setPositionX(winSize.width / 2 - 30);

    loadColorTriggerButton(self);
    loadRemapHook(self);
    loadPasteButton(self);

    GameToolbox::createToggleButton(
        (SEL_MenuHandler)&EditorPauseLayer_CB::onRotateSaws,
        g_bRotateSaws, as<CCMenu*>(self->m_pButton0->getParent()),
        self, self, .55f, .42f, 85.0f, "", false, 0, nullptr,
        "Preview Saws",
        {
            CCDirector::sharedDirector()->getScreenLeft() + 25.0f,
            CCDirector::sharedDirector()->getScreenBottom() + 192.0f
        },
        { 8.0f, 0.0f }
    );

    GameToolbox::createToggleButton(
        (SEL_MenuHandler)&EditorPauseLayer_CB::onShowKeybinds,
        g_bShowKeybinds, as<CCMenu*>(self->m_pButton0->getParent()),
        self, self, .55f, .42f, 85.0f, "", false, 0, nullptr,
        "Show Keybinds",
        {
            CCDirector::sharedDirector()->getScreenLeft() + 25.0f,
            CCDirector::sharedDirector()->getScreenBottom() + 216.0f
        },
        { 8.0f, 0.0f }
    );

    auto objCountLabel = getChild<CCLabelBMFont*>(self, 9);

    if (objCountLabel) {
        std::string str = objCountLabel->getString();
        int c = countLDMObjects(self->m_pEditorLayer);
        float p = static_cast<float>(c) /
            self->m_pEditorLayer->getAllObjects()->count() *
            100.f;
        p = roundf(p);
        str += " | " + std::to_string(c) + " LDM (" +
            BetterEdit::formatToString(p) + "%)";
        objCountLabel->setString(str.c_str());
    }

    // getAutoSaveTimer(LevelEditorLayer::get()->getEditorUI())->pause();

    return true;
} MAT_GDMAKE_HOOK(0x730e0, EditorPauseLayer_init);

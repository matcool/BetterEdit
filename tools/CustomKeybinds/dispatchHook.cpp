#pragma once

#include "KeybindManager.hpp"
#include "SuperKeyboardManager.hpp"
#include "SuperMouseManager.hpp"
#include "../settings/BESettingsLayer.hpp"

GDMAKE_HOOK("libcocos2d.dll::?dispatchScrollMSG@CCMouseDispatcher@cocos2d@@QAE_NMM@Z")
bool __fastcall dispatchScrollMSGHook(CCMouseDelegate* self, edx_t edx, float y, float x) {
    if (SuperMouseManager::get()->dispatchScrollEvent(y, x, getMousePos()))
        return true;

    return GDMAKE_ORIG(self, edx, y, x);
}

GDMAKE_HOOK("libcocos2d.dll::?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z")
void __fastcall dispatchKeyboardMSGHook(
    CCKeyboardDispatcher* self,
    edx_t edx,
    enumKeyCodes key,
    bool down
) {
    if (key == KEY_Tab && self->getAltKeyPressed())
        self->updateModifierKeys(false, false, false, false);

    if (SuperKeyboardManager::get()->dispatchEvent(key, down))
        return;
    
    KeybindManager::get()->registerKeyPress(key, down);

    GDMAKE_ORIG_V(self, edx, key, down);
}

void  AppDelegate_applicationWillEnterForeground(CCApplication* self) {
    matdash::orig<&AppDelegate_applicationWillEnterForeground>(self);
    
    CCDirector::sharedDirector()->getKeyboardDispatcher()
        ->updateModifierKeys(false, false, false, false);
    
    if (shouldGetFoolAchievement()) {
        AchievementNotifier::sharedState()->notifyAchievement(
            "You Fool...", "what the dog doin", "BE_trollface.png", true
        );
        showedFoolAchievement();
    }
} MAT_GDMAKE_HOOK(0x3d130, AppDelegate_applicationWillEnterForeground);

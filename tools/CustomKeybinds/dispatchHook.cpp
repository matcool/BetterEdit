#pragma once

#include "KeybindManager.hpp"
#include "SuperKeyboardManager.hpp"
#include "SuperMouseManager.hpp"
#include "../settings/BESettingsLayer.hpp"


matdash::cc::thiscall<bool> dispatchScrollMSGHook(CCMouseDelegate* self, float y, float x) {
    if (SuperMouseManager::get()->dispatchScrollEvent(y, x, getMousePos()))
        return true;

    return matdash::orig<&dispatchScrollMSGHook>(self, y, x);
} MAT_GDMAKE_HOOK_C("?dispatchScrollMSG@CCMouseDispatcher@cocos2d@@QAE_NMM@Z", dispatchScrollMSGHook)

matdash::cc::thiscall<void> dispatchKeyboardMSGHook(
    CCKeyboardDispatcher* self,
    enumKeyCodes key,
    bool down
) {
    if (key == KEY_Tab && self->getAltKeyPressed())
        self->updateModifierKeys(false, false, false, false);

    if (SuperKeyboardManager::get()->dispatchEvent(key, down))
        return {};

    KeybindManager::get()->registerKeyPress(key, down);

    matdash::orig<&dispatchKeyboardMSGHook>(self, key, down);
    return {};
} MAT_GDMAKE_HOOK_C("?dispatchKeyboardMSG@CCKeyboardDispatcher@cocos2d@@QAE_NW4enumKeyCodes@2@_N@Z", dispatchKeyboardMSGHook)

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

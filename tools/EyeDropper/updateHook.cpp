#include "../../BetterEdit.hpp"
#include "../CustomKeybinds/KeybindManager.hpp"
#include "../CustomKeybinds/SuperMouseManager.hpp"
#include "updateHook.hpp"
#include <mutex>

bool g_bLoadedResources = false;
std::map<int, bool> g_bPressedButtons;

bool isLeftMouseButtonPressed() {
    return g_bPressedButtons[0];
}

void CCEGLView_onGLFWMouseCallBack(CCEGLView* self, GLFWwindow* wnd, int btn, int pressed, int z) {
    g_bPressedButtons[btn] = pressed;

    KeybindManager::get()->registerMousePress(static_cast<MouseButton>(btn), pressed);

    if (SuperMouseManager::get()->dispatchClickEvent(
        static_cast<MouseButton>(btn), pressed, getMousePos()
    ))
        return;

    if (LevelEditorLayer::get()) {
        KeybindManager::get()->executeEditorCallbacks(
            Keybind(static_cast<MouseButton>(btn)),
            LevelEditorLayer::get()->getEditorUI(),
            pressed
        );
    }
    
    if (PlayLayer::get()) {
        KeybindManager::get()->executePlayCallbacks(
            Keybind(static_cast<MouseButton>(btn)),
            PlayLayer::get(),
            pressed
        );
    }

    return matdash::orig<&CCEGLView_onGLFWMouseCallBack>(self, wnd, btn, pressed, z);
} MAT_GDMAKE_HOOK_C("?onGLFWMouseCallBack@CCEGLView@cocos2d@@IAEXPAUGLFWwindow@@HHH@Z", CCEGLView_onGLFWMouseCallBack)

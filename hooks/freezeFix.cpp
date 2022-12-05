#include <GDMake.h>
#include "../tools/SliderImprovement/passTouch.hpp"
#include <BetterEdit.hpp>

using namespace gdmake;
using namespace gd;
using namespace cocos2d;

// thank you cos8o
// adapted from https://github.com/cos8oih/editorFreezeFix

void  EditorUI_onPlaytest(EditorUI* self,  CCObject* pSender) {
    if (
        BetterEdit::getPulseObjectsInEditor() &&
        !BetterEdit::getHasShownPulseVolumeWarning() &&
        !FMODAudioEngine::sharedEngine()->m_fBackgroundMusicVolume 
    ) {
        FLAlertLayer::create(
            nullptr,
            "Warning",
            "OK", nullptr,
            320.0f,
            "<cy>Pulsing objects</c> do not work when you have "
            "volume turned down! (You can turn up volume in "
            "<cg>BetterEdit settings</c>)"
        )->show();
        BetterEdit::setHasShownPulseVolumeWarning(true);
        return;
    }

    self->m_bTouchDown = false;
    if (!g_bHoldingDownTouch)
        matdash::orig<&EditorUI_onPlaytest>(self,  pSender);
} MAT_GDMAKE_HOOK(0x87600, EditorUI_onPlaytest);

#include <GDMake.h>

using namespace gdmake;

void  PauseLayer_onEdit(cocos2d::CCLayer* self,  cocos2d::CCObject* pSender) {
    matdash::orig<&PauseLayer_onEdit>(self,  pSender);

    // this fixes the scale buttons for
    // some reason idk
    self->removeFromParentAndCleanup(true);
} MAT_GDMAKE_HOOK(0x1e60e0, PauseLayer_onEdit);

#include "../BetterEdit.hpp"

bool  SetupPickupTriggerPopup_init(SetupPickupTriggerPopup* self,  EffectGameObject* obj, CCArray* arr) {
    if (!matdash::orig<&SetupPickupTriggerPopup_init>(self,  obj, arr))
        return false;
    
    self->m_countInput->setAllowedChars("0123456789-");

    return true;
} MAT_GDMAKE_HOOK(0x23d550, SetupPickupTriggerPopup_init);

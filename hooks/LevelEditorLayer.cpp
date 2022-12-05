#include "../BetterEdit.hpp"
#include "../tools/LevelPercent/levelPercent.hpp"
#include "../tools/RotateSaws/rotateSaws.hpp"
#include "../tools/AutoSave/autoSave.hpp"
#include "../tools/other/teleportScaleFix.hpp"
#include "../tools/other/dashOrbLine.hpp"

bool  LevelEditorLayer_init(LevelEditorLayer* self,  GJGameLevel* level) {
    if (!matdash::orig<&LevelEditorLayer_init>(self,  level))
        return false;

    BetterEdit::setEditorInitialized(true);

    updatePercentLabelPosition(self->m_pEditorUI);
    // getAutoSaveTimer(self->m_pEditorUI)->resetTimer();

    return true;
} MAT_GDMAKE_HOOK(0x15ee00, LevelEditorLayer_init);

void  LevelEditorLayer_destructorHook(LevelEditorLayer* self) {
    BetterEdit::setEditorInitialized(false);
    BetterEdit::setEditorViewOnlyMode(false);
    clearDashOrbLines();

    return matdash::orig<&LevelEditorLayer_destructorHook>(self);
} MAT_GDMAKE_HOOK(0x15e8d0, LevelEditorLayer_destructorHook);

void  LevelEditorLayer_addSpecial(
    LevelEditorLayer* self,
    
    GameObject* obj
) {
    matdash::orig<&LevelEditorLayer_addSpecial>(self,  obj);

    handleObjectAddForSlider(self, obj);

    registerDashOrb(obj);
    
    if (shouldRotateSaw() && objectIsSaw(obj))
        beginRotateSaw(obj);

    fixPortalScale(obj);
} MAT_GDMAKE_HOOK(0x162650, LevelEditorLayer_addSpecial);

void  LevelEditorLayer_removeObject(
    LevelEditorLayer* self,
    
    GameObject* obj,
    bool idk
) {
    matdash::orig<&LevelEditorLayer_removeObject>(self,  obj, idk);

    unregisterDashOrb(obj);

    handleObjectAddForSlider(self, obj);
    
    if (shouldRotateSaw() && objectIsSaw(obj))
        stopRotateSaw(obj);
} MAT_GDMAKE_HOOK(0x161cb0, LevelEditorLayer_removeObject);

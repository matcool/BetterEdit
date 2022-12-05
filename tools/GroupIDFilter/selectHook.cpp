#include <GDMake.h>
#include "AdvancedFilterLayer.hpp"
#include "../EditorLayerInput/editorLayerInput.hpp"
#include "../AutoSave/autoSave.hpp"
#include "../VisibilityTab/loadVisibilityTab.hpp"

using namespace gdmake;

static bool g_bUndid = false;

void  EditorUI_selectObject(EditorUI* self,  GameObject* obj, bool idk) {
    if (BetterEdit::isEditorViewOnlyMode())
        return;
    
    if (AdvancedFilterLayer::testSelectObject(obj) && testSelectObjectLayer(obj)) {
        matdash::orig<&EditorUI_selectObject>(self,  obj, idk);
    }
} MAT_GDMAKE_HOOK(0x86250, EditorUI_selectObject);

void  EditorUI_selectObjects(EditorUI* self,  cocos2d::CCArray* objs, bool idk) {
    if (BetterEdit::isEditorViewOnlyMode())
        return;

    if (!g_bUndid) {
        CCARRAY_FOREACH_B_BASE(objs, obj, GameObject*, ix) {
            if (!AdvancedFilterLayer::testSelectObject(obj) ||
                !testSelectObjectLayer(obj))
                objs->removeObjectAtIndex(ix--);
        }
    }
    
    matdash::orig<&EditorUI_selectObjects>(self,  objs, idk);
} MAT_GDMAKE_HOOK(0x864a0, EditorUI_selectObjects);

void  EditorUI_undoLastAction(EditorUI* self,  CCObject* pSender) {
    g_bUndid = true;
    matdash::orig<&EditorUI_undoLastAction>(self,  pSender);
    g_bUndid = false;
} MAT_GDMAKE_HOOK(0x87070, EditorUI_undoLastAction);

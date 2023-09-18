#include "../../BetterEdit.hpp"

matdash::cc::stdcall<void> ObjectToolbox_gridNodeSizeForKey(int objID) {
    if (!BetterEdit::sharedState()->getGridSizeEnabled()) {
        matdash::orig<&ObjectToolbox_gridNodeSizeForKey>(objID);
        return {};
    }

    float gridSize = BetterEdit::sharedState()->getGridSize();
    __asm {
        movss xmm0, gridSize
        add esp, 0x4
    }
    return {};
} MAT_GDMAKE_HOOK(0x1dc920, ObjectToolbox_gridNodeSizeForKey);

void  EditorUI_updateGridNodeSize(EditorUI* self) {
    if (!BetterEdit::sharedState()->getGridSizeEnabled())
        return matdash::orig<&EditorUI_updateGridNodeSize>(self);

    auto actualMode = self->m_nSelectedMode;
    self->m_nSelectedMode = EditorUI::Mode_Create;

    matdash::orig<&EditorUI_updateGridNodeSize>(self);

    self->m_nSelectedMode = actualMode;
} MAT_GDMAKE_HOOK(0x78f60, EditorUI_updateGridNodeSize);

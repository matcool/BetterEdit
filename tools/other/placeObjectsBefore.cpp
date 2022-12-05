#include "../../BetterEdit.hpp"
#include "dashOrbLine.hpp"

float g_fEdge = 180.0f;
float g_fEdgeInverted = -g_fEdge;

void updateEdge() {
    g_fEdge = BetterEdit::getObjectsPreStartLine() * 30.0f;
    g_fEdgeInverted = -g_fEdge;
}

void  DrawGridLayer_draw(DrawGridLayer* self) {
    // updateEdge();

    // __asm { movss xmm2, g_fEdge }

    matdash::orig<&DrawGridLayer_draw>(self);

    drawDashOrbLines(self);
} MAT_GDMAKE_HOOK(0x16ce90, DrawGridLayer_draw);

void  EditorUI_constrainGameLayerPosition(EditorUI* self) {
    // updateEdge();
    
    // __asm { movss xmm3, g_fEdge }

    return matdash::orig<&EditorUI_constrainGameLayerPosition>(self);
} MAT_GDMAKE_HOOK(0x8f920, EditorUI_constrainGameLayerPosition);

void loadPlaceObjectsBefore() {
    // patch onCreateObject
    // patch(0x857c8, { 0xeb, 0x58, 0x90 });

    // patch getLimitedPosition
    // patch(0x8de77, intToBytes(as<int>(&g_fEdgeInverted)));
    // patch(0x8de89, intToBytes(as<int>(&g_fEdgeInverted)));

    // patch camera to use -180.0f which is stored in xmm3
    // subss xmm1, xmm3
    // patch(0x8f9ce, { 0xf3, 0x0f, 0x5c, 0xcb });
}

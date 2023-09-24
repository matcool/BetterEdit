#include "../../BetterEdit.hpp"
#include "dashOrbLine.hpp"

float g_fEdge = 180.0f;
float g_fEdgeInverted = -g_fEdge;

void updateEdge() {
	g_fEdge = BetterEdit::getObjectsPreStartLine() * 30.0f;
	g_fEdgeInverted = -g_fEdge;
}

// #include <Geode/modify/DrawGridLayer.hpp>

// class $modify(DrawGridLayer) {
// 	void draw() {
// 		updateEdge();
// 		// TODO: figure out what the asms were doing here
// 		DrawGridLayer::draw();
// 	}
// };

// #include <Geode/modify/EditorUI>

// class $modify(EditorUI) {
// 	void constrainGameLayerPosition(float x, float y) {
// 		updateEdge();
// 		// TODO: figure out what the asms were doing here
// 		EditorUI::constrainGameLayerPosition(x, y);
// 	}
// };

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

#pragma once

#include <GDMake.h>

static constexpr const int ZOOMINPUT_TAG = 8003;

cocos2d::CCMenu* getGridButtonParent(EditorUI* self);
void loadGridButtons(EditorUI*);
void showGridButtons(EditorUI*, bool);
void zoomEditorGrid(EditorUI*, bool zoomIn);

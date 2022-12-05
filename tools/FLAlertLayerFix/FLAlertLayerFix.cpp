#include "../../BetterEdit.hpp"

void fixLayering(CCNode* node) {
    auto target = CCDirector::sharedDirector()->getRunningScene();

    int layer_count = target->getChildrenCount();

    node->setZOrder(100 + layer_count * 200);
}

void fixLayering(FLAlertLayer* layer) {
    auto target = layer->m_pTargetLayer ?
        layer->m_pTargetLayer : 
        CCDirector::sharedDirector()->getRunningScene();

    int layer_count = target->getChildrenCount();

    layer->m_nZOrder2 = 100 + layer_count * 200;
    layer->setZOrder(layer->m_nZOrder2);
}

void  FLAlertLayer_show(FLAlertLayer* layer) {
    matdash::orig<&FLAlertLayer_show>(layer);
    fixLayering(layer);
} MAT_GDMAKE_HOOK(0x23560, FLAlertLayer_show);

void  ColorSelectPopup_show(ColorSelectPopup* layer) {
    matdash::orig<&ColorSelectPopup_show>(layer);
    fixLayering(layer);
} MAT_GDMAKE_HOOK(0x46ff0, ColorSelectPopup_show);

void  CustomSongLayer_show(FLAlertLayer* layer) {
    matdash::orig<&CustomSongLayer_show>(layer);
    fixLayering(layer);
} MAT_GDMAKE_HOOK(0x672c0, CustomSongLayer_show);

void loadFLAlertLayerFix() {}


#include "../../BetterEdit.hpp"
#include "RGBColorInputWidget.hpp"

static constexpr const int WIDGET_TAG = 69696;

bool  ColorSelectPopup_init(
    ColorSelectPopup* self,
    
    EffectGameObject* eff_obj,
    CCArray* arr,
    ColorAction* action
) {
    if (!matdash::orig<&ColorSelectPopup_init>(self,  eff_obj, arr, action))
        return false;
    
    auto layer = self->getAlertLayer();

    auto widget = RGBColorInputWidget::create(self);
    widget->setTag(WIDGET_TAG);

    auto center = CCDirector::sharedDirector()->getWinSize() / 2.f;

    if (self->isColorTrigger)
        widget->setPosition({center.width - 155.f, center.height + 29.f});
    else
        widget->setPosition({center.width + 127.f, center.height - 90.f});

    layer->addChild(widget);
    widget->setVisible(!self->copyColor);

    return true;
} MAT_GDMAKE_HOOK(0x43ae0, ColorSelectPopup_init);

void  ColorSelectPopup_colorValueChanged(ColorSelectPopup* self) {
    matdash::orig<&ColorSelectPopup_colorValueChanged>(self);
    CATCH_NULL(as<RGBColorInputWidget*>(self->getAlertLayer()->getChildByTag(WIDGET_TAG)))
        ->update_labels(true, true);
} MAT_GDMAKE_HOOK(0x46f30, ColorSelectPopup_colorValueChanged);

void  ColorSelectPopup_updateCopyColor(ColorSelectPopup* self) {
    matdash::orig<&ColorSelectPopup_updateCopyColor>(self);
    CATCH_NULL(as<RGBColorInputWidget*>(self->getAlertLayer()->getChildByTag(WIDGET_TAG)))
        ->setVisible(!self->copyColor);
} MAT_GDMAKE_HOOK(0x479c0, ColorSelectPopup_updateCopyColor);

bool  SetupPulsePopup_init(SetupPulsePopup* self,  EffectGameObject* eff_obj, CCArray* arr) {
    if (!matdash::orig<&SetupPulsePopup_init>(self,  eff_obj, arr))
        return false;

    auto layer = as<ColorSelectPopup*>(self)->getAlertLayer();

    auto widget = RGBColorInputWidget::create(as<ColorSelectPopup*>(self));
        // the color picker is in the same offset in both classe s
    widget->setTag(WIDGET_TAG);

    auto center = CCDirector::sharedDirector()->getWinSize() / 2.f;

    self->colorPicker->setPositionX(self->colorPicker->getPositionX() + 3.7f);
    widget->setPosition({center.width - 132.f, center.height + 32.f});

    auto square_width = self->currentColorSpr->getScaledContentSize().width;
    auto x = widget->getPositionX() - square_width / 2.f;

    self->currentColorSpr->setPosition({x + 20.0f, center.height + 85.f});
    self->prevColorSpr->setPosition({x + 20.0f + square_width, center.height + 85.f});

    layer->addChild(widget);
    widget->setVisible(self->pulseMode == 0);

    return true;
} MAT_GDMAKE_HOOK(0x23e980, SetupPulsePopup_init);

void  SetupPulsePopup_colorValueChanged(SetupPulsePopup* self) {
    matdash::orig<&SetupPulsePopup_colorValueChanged>(self);
    CATCH_NULL(as<RGBColorInputWidget*>(as<ColorSelectPopup*>(self)->getAlertLayer()->getChildByTag(WIDGET_TAG)))
        ->update_labels(true, true);
} MAT_GDMAKE_HOOK(0x2426b0, SetupPulsePopup_colorValueChanged);

void  SetupPulsePopup_updatePulseMode(SetupPulsePopup* self) {
    matdash::orig<&SetupPulsePopup_updatePulseMode>(self);
    CATCH_NULL(as<RGBColorInputWidget*>(as<ColorSelectPopup*>(self)->getAlertLayer()->getChildByTag(WIDGET_TAG)))
        ->setVisible(self->pulseMode == 0);
} MAT_GDMAKE_HOOK(0x242cf0, SetupPulsePopup_updatePulseMode);

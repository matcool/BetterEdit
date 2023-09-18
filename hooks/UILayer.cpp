#include "../BetterEdit.hpp"
#include "../tools/CustomKeybinds/KeybindManager.hpp"

void  UILayer_keyDown(UILayer* self,  enumKeyCodes key) {
    KeybindManager::get()->executePlayCallbacks(
        Keybind(key), PlayLayer::get(), true
    );
} MAT_GDMAKE_HOOK(0x25f890, UILayer_keyDown);

void  UILayer_keyUp(UILayer* self,  enumKeyCodes key) {
    KeybindManager::get()->executePlayCallbacks(
        Keybind(key), PlayLayer::get(), false
    );
} MAT_GDMAKE_HOOK(0x25fa10, UILayer_keyUp);

bool  UILayer_init(UILayer* self) {
    if (!matdash::orig<&UILayer_init>(self))
        return false;

    struct get_bool_from_byte {
        bool a, b, c, d;
    };

    auto app = CCApplication::sharedApplication();
    // if controller plugged in
    if (as<get_bool_from_byte*>((as<int>(app) + 0x90))->b)
        return true;

    if (!self->m_checkPointMenu)
        return true;

    auto zBind = KeybindManager::get()->getTargetByID("gd.play.place_checkpoint");
    std::string z = "";
    for (auto b : KeybindManager::get()->getKeybindsForCallback(zBind.type, zBind.bind))
        z += b.toString() + ", ";
    z = z.substr(0, z.size() - 2);
    
    auto btnZ = getChild<CCMenuItemSpriteExtra*>(self->m_checkPointMenu, 0);
    if (!btnZ)
        return true;
    
    auto zLabel = getChild<CCLabelBMFont*>(btnZ->getNormalImage(), 0);
    if (!zLabel)
        return true;
    
    zLabel->setString(z.c_str());
    zLabel->limitLabelWidth(60.0f, zLabel->getScale(), .1f);

    auto yBind = KeybindManager::get()->getTargetByID("gd.play.delete_checkpoint");
    std::string y = "";
    for (auto b : KeybindManager::get()->getKeybindsForCallback(yBind.type, yBind.bind))
        y += b.toString() + ", ";
    y = y.substr(0, y.size() - 2);

    auto btnY = getChild<CCMenuItemSpriteExtra*>(self->m_checkPointMenu, 1);
    if (!btnY)
        return true;
    
    auto yLabel = getChild<CCLabelBMFont*>(btnY->getNormalImage(), 0);
    if (!yLabel)
        return true;

    yLabel->setString(y.c_str());
    yLabel->limitLabelWidth(60.0f, yLabel->getScale(), .1f);

    return true;
} MAT_GDMAKE_HOOK(0x25f3b0, UILayer_init);

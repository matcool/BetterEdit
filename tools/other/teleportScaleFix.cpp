#include "teleportScaleFix.hpp"

// i have no idea where to place this function
void __fastcall TeleportPortalobject_setRScale(TeleportPortalObject* self, void*, float scale) {
    // stupid c++ wont let me call GameObject::setRScale directly
    reinterpret_cast<void(__thiscall*)(GameObject*, float)>(geode::base::get() + 0xe5280)(self, scale);
    
    if (self->m_orangePortal) {
        self->m_orangePortal->m_scale = self->m_scale;
        self->m_orangePortal->setRScale(1.f);
    }
}

void fixPortalScale(GameObject* obj) {
    if (obj->m_objectID == 747) {
        auto op = as<TeleportPortalObject*>(obj)->m_orangePortal;
        // idk why rob uses a negative scale instead of just flip x
        op->m_scale = obj->m_scale;
        op->setRScale(1.f);
    }
}

void loadTeleportScaleFix() {
    patch(0x2e3e50, intToBytes(as<uintptr_t>(&TeleportPortalobject_setRScale)));
}

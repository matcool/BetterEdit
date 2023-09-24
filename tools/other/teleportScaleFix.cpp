#include "teleportScaleFix.hpp"

#include <Geode/modify/TeleportPortalObject.hpp>

class $modify(AddRScale, TeleportPortalObject) {
	static void onModify(auto& self) {
		// TODO: a way to vtable patch
	}

	void setRScaleAdd(float scale) {
		GameObject::setRScale(scale);

		if (m_orangePortal) {
			m_orangePortal->m_scale = m_scale;
			m_orangePortal->setRScale(1.f);
		}
	}
};

// i have no idea where to place this function

void fixPortalScale(GameObject* obj) {
	if (obj->m_objectID == 747) {
		auto op = as<TeleportPortalObject*>(obj)->m_orangePortal;
		// idk why rob uses a negative scale instead of just flip x
		op->m_scale = obj->m_scale;
		op->setRScale(1.f);
	}
}

void loadTeleportScaleFix() {}

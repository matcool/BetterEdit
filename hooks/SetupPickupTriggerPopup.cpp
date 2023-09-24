#include "../BetterEdit.hpp"

#include <Geode/modify/SetupPickupTriggerPopup.hpp>

class $modify(SetupPickupTriggerPopup) {
	bool init(EffectGameObject* obj, CCArray* arr) {
		if (!SetupPickupTriggerPopup::init(obj, arr)) {
			return false;
		}

		m_countInput->setAllowedChars("0123456789-");

		return true;
	}

	// MAT_GDMAKE_HOOK(0x23d550, SetupPickupTriggerPopup_init);
};

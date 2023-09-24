#include "../BetterEdit.hpp"
#include "../tools/AutoSave/autoSave.hpp"
#include "../tools/LevelPercent/levelPercent.hpp"
#include "../tools/RotateSaws/rotateSaws.hpp"
#include "../tools/other/dashOrbLine.hpp"
#include "../tools/other/teleportScaleFix.hpp"

#include <Geode/modify/LevelEditorLayer.hpp>

class $modify(LevelEditorLayer) {
	bool init(GJGameLevel* level) {
		if (!LevelEditorLayer::init(level)) {
			return false;
		}

		BetterEdit::setEditorInitialized(true);

		updatePercentLabelPosition(m_editorUI);
		// getAutoSaveTimer(self->m_pEditorUI)->resetTimer();

		return true;
	}

	void destructor() {
		BetterEdit::setEditorInitialized(false);
		BetterEdit::setEditorViewOnlyMode(false);
		clearDashOrbLines();

		LevelEditorLayer::~LevelEditorLayer();
	}

	void addSpecial(GameObject* obj) {
		LevelEditorLayer::addSpecial(obj);

		handleObjectAddForSlider(this, obj);

		registerDashOrb(obj);

		// TODO: readd?
		// if (shouldRotateSaw() && objectIsSaw(obj)) {
		// 	beginRotateSaw(obj);
		// }

		fixPortalScale(obj);
	}

	void removeSpecial(GameObject* obj) {
		LevelEditorLayer::removeSpecial(obj);

		unregisterDashOrb(obj);

		handleObjectAddForSlider(this, obj);

		// TODO: readd?
		// if (shouldRotateSaw() && objectIsSaw(obj)) {
		// 	stopRotateSaw(obj);
		// }
	}
};
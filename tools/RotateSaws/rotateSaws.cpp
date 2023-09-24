#include "rotateSaws.hpp"

#include <array>

static constexpr int const ROTATEACTION_TAG = 0x42069;

std::map<GameObject*, float> g_startRotations;
std::array<int, 100> g_sawIDs = {
	1705,
	1706,
	1707,
	1708,
	1709,
	1710,
	1734,
	1735,
	1736,
	678,
	679,
	680,
	186,
	187,
	188,
	740,
	741,
	742,
	1619,
	1620,
	183,
	184,
	185,
	85,
	86,
	87,
	97,
	1831,
	1832,
	1833,
	1834,
	1058,
	1059,
	1060,
	1061,
	1752,
	1055,
	1056,
	1057,
	1019,
	1020,
	1021,
	997,
	998,
	999,
	1000,
	1521,
	1522,
	180,
	181,
	182,
	154,
	155,
	156,
	137,
	138,
	139,
	222,
	223,
	224,
	1523,
	1524,
	1525,
	1526,
	1527,
	1528,
	394,
	395,
	396,
	375,
	376,
	377,
	378,
	// old black 1.9 saws
	675,
	676,
	677,
	// orbs
	1022,
	1330,
};

bool objectIsSaw(GameObject* obj) {
	for (auto id : g_sawIDs) {
		if (id == obj->m_objectID) {
			return true;
		}
	}

	return false;
}

void beginRotateSaw(GameObject* obj) {
	if (obj->m_pMyAction) {
		return;
	}

	if (obj->m_bSawIsDisabled) {
		return;
	}

	CCAction* r;
	switch (obj->m_objectID) {
		// the big invisible saw, for some reason
		case 0xba: r = obj->createRotateAction(300.0f, 0); break;
		// the big reaper saw
		case 0x653: r = obj->createRotateAction(720.0f, -1); break;
		// the small reaper saw
		case 0x654: r = obj->createRotateAction(1080.0f, -1); break;
		default: r = obj->createRotateAction(360.0f, 0); break;
	}
	r->setTag(ROTATEACTION_TAG);
	g_startRotations[obj] = obj->getRotation();
	obj->m_pMyAction = r;
	obj->m_pMyAction->retain();
	obj->runAction(r);
}

void beginRotations(LevelEditorLayer* self) {
	CCARRAY_FOREACH_B_TYPE(self->m_pObjects, obj, GameObject)
	if (objectIsSaw(obj)) {
		beginRotateSaw(obj);
	}
}

void stopRotateSaw(GameObject* obj) {
	obj->stopActionByTag(ROTATEACTION_TAG);
	if (g_startRotations.count(obj)) {
		obj->setRotation(g_startRotations[obj]);
		g_startRotations.erase(obj);
	}
	if (obj->m_pMyAction) {
		obj->m_pMyAction->release();
	}
	obj->m_pMyAction = nullptr;
}

void stopRotations(LevelEditorLayer* self) {
	CCARRAY_FOREACH_B_TYPE(self->m_pObjects, obj, GameObject)
	if (objectIsSaw(obj)) {
		stopRotateSaw(obj);
	}

	g_startRotations.clear();
}

void resumeRotations(LevelEditorLayer* self) {
	CCARRAY_FOREACH_B_TYPE(self->m_pObjects, obj, GameObject)
	if (objectIsSaw(obj)) {
		if (obj->m_pMyAction) {
			obj->runAction(obj->m_pMyAction);
		}
	}
}

void pauseRotations(LevelEditorLayer* self) {
	CCARRAY_FOREACH_B_TYPE(self->m_pObjects, obj, GameObject)
	if (objectIsSaw(obj)) {
		obj->stopActionByTag(ROTATEACTION_TAG);
	}
}

void SetupRotatePopup_keyBackClicked(CCLayer* self) {
	if (shouldRotateSaw()) {
		stopRotations(LevelEditorLayer::get());
		beginRotations(LevelEditorLayer::get());
	}

	matdash::orig<&SetupRotatePopup_keyBackClicked>(self);
}

MAT_GDMAKE_HOOK(0x2441a0, SetupRotatePopup_keyBackClicked);

void SetupRotatePopup_onClose(CCLayer* self, CCObject* pSender) {
	if (shouldRotateSaw()) {
		stopRotations(LevelEditorLayer::get());
		beginRotations(LevelEditorLayer::get());
	}

	matdash::orig<&SetupRotatePopup_onClose>(self, pSender);
}

MAT_GDMAKE_HOOK(0x244150, SetupRotatePopup_onClose);

void LevelEditorLayer_onPlaytest(LevelEditorLayer* self) {
	matdash::orig<&LevelEditorLayer_onPlaytest>(self);

	if (BetterEdit::getRotateSawsInEditor() && !shouldRotateSaw()) {
		beginRotations(self);
	}
}

MAT_GDMAKE_HOOK(0x1695a0, LevelEditorLayer_onPlaytest);

void LevelEditorLayer_onResumePlaytest(LevelEditorLayer* self) {
	matdash::orig<&LevelEditorLayer_onResumePlaytest>(self);

	if (BetterEdit::getRotateSawsInEditor() && !shouldRotateSaw()) {
		resumeRotations(self);
	}
}

MAT_GDMAKE_HOOK(0x169d90, LevelEditorLayer_onResumePlaytest);

void LevelEditorLayer_onPausePlaytest(LevelEditorLayer* self) {
	matdash::orig<&LevelEditorLayer_onPausePlaytest>(self);

	if (BetterEdit::getRotateSawsInEditor() && !shouldRotateSaw()) {
		pauseRotations(self);
	}
}

MAT_GDMAKE_HOOK(0x169cc0, LevelEditorLayer_onPausePlaytest);

void LevelEditorLayer_onStopPlaytest(LevelEditorLayer* self) {
	matdash::orig<&LevelEditorLayer_onStopPlaytest>(self);

	if (BetterEdit::getRotateSawsInEditor() && !shouldRotateSaw()) {
		stopRotations(self);
	}
}

MAT_GDMAKE_HOOK(0x169f10, LevelEditorLayer_onStopPlaytest);

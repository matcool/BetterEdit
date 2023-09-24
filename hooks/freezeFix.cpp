#include "../tools/SliderImprovement/passTouch.hpp"

#include <BetterEdit.hpp>
#include <GDMake.h>

using namespace gdmake;
using namespace gd;
using namespace cocos2d;

// thank you cos8o
// adapted from https://github.com/cos8oih/editorFreezeFix

#include <Geode/modify/EditorUI.hpp>

class $modify(EditorUI) {
	void onPlaytest(CCObject* pSender) {
		if (BetterEdit::getPulseObjectsInEditor() && !BetterEdit::getHasShownPulseVolumeWarning() &&
			!FMODAudioEngine::sharedEngine()->m_backgroundMusicVolume) {
			FLAlertLayer::create(
				nullptr, "Warning",
				"<cy>Pulsing objects</c> do not work when you have "
				"volume turned down! (You can turn up volume in "
				"<cg>BetterEdit settings</c>)"
				"OK",
				"Cancel", nullptr, 320.0f
			)
				->show();
			BetterEdit::setHasShownPulseVolumeWarning(true);
			return;
		}

		m_touchDown = false;
		if (!g_bHoldingDownTouch) {
			EditorUI::onPlaytest(pSender);
		}
	}
};

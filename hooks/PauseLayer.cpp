#include <GDMake.h>

using namespace gdmake;

#include <Geode/modify/PauseLayer.hpp>

class $modify(PauseLayer) {
	void onEdit(cocos2d::CCLayer* self, cocos2d::CCObject* pSender) {
		PauseLayer::onEdit(pSender);

		// this fixes the scale buttons for
		// some reason idk
		self->removeFromParentAndCleanup(true);
	}
};

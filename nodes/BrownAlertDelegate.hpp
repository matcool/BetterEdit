#pragma once

#include <GDMake.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

class BrownAlertDelegate : public FLAlertLayer {
protected:
	// layer size is stored here
	cocos2d::CCSize m_pLrSize;
	cocos2d::extension::CCScale9Sprite* m_pBGSprite;

	virtual bool init(
		float width, float height, char const* bg = "GJ_square01.png", char const* title = ""
	);
	virtual void setup() = 0;
	virtual void keyDown(cocos2d::enumKeyCodes) override;

	virtual void keyBackClicked() override;
	void onClose(cocos2d::CCObject*);
};

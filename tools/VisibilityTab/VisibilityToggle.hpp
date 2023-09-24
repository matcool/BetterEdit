#pragma once

#include "../../BetterEdit.hpp"

static ButtonSprite* createEditBtnSprite(char const* file, bool sel) {
	auto sprOff = ButtonSprite::create(
		createBESprite(file), 0x32, true, 0x32, 1.0f, 0,
		sel ? "GJ_button_02.png" : "GJ_button_01.png", true
	);
	sprOff->m_subSprite->setScale(1.25f);

	return sprOff;
}

class VisibilityToggle : public CCMenuItemToggler {
public:
	using Getter = std::function<bool(void)>;
	using Setter = std::function<void(bool, VisibilityToggle*)>;

protected:
	Getter m_pGetter;
	Setter m_pSetter;

	void onToggle(CCObject*);

public:
	void updateState();
	VisibilityToggle* invokeSetter();

	static VisibilityToggle* create(char const* sprite, Getter getter, Setter setter);

	static VisibilityToggle* create(char const* sprite);
};

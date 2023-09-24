#pragma once

#include <BrownAlertDelegate.hpp>
#include <GDMake.h>
#include <InputNode.hpp>

class InputPrompt : public BrownAlertDelegate {
public:
	using InputResult = std::function<void(char const*)>;

protected:
	InputNode* m_pInput;
	InputResult m_pResultFunc;
	CCMenuItemSpriteExtra* m_pTrashBtn;
	bool m_bApplyOnEsc = false;

	inline void setup() override {};
	void onApply(cocos2d::CCObject*);
	void onTrash(cocos2d::CCObject*);
	void keyBackClicked() override;
	bool init(char const* title, char const* inputText, InputResult resFunc, char const* applyText);

public:
	static InputPrompt* create(
		char const* title, char const* inputText, InputResult resFunc,
		char const* applyText = "Apply"
	);

	inline InputPrompt* setApplyOnEsc(bool on) {
		this->m_bApplyOnEsc = on;
		return this;
	}

	inline InputPrompt* setTrashButton(bool on) {
		this->m_pTrashBtn->setVisible(on);
		return this;
	}

	inline InputPrompt* setCallback(InputResult resFunc) {
		this->m_pResultFunc = resFunc;
		return this;
	}

	inline InputPrompt* setFilter(std::string const& filter) {
		this->m_pInput->getInputNode()->setAllowedChars(filter);
		return this;
	}

	inline InputNode* getInputNode() {
		return m_pInput;
	}
};

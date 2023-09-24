#include "loadPasteButton.hpp"

#include <InputPrompt.hpp>

class EditorPauseLayer_CB : public EditorPauseLayer {
public:
	void onPasteFromString(CCObject*) {
		auto p = InputPrompt::create(
			"Paste Object String", "String",
			[this](char const* val) -> void {
				if (val && strlen(val)) {
					// auto objs = this->m_editorLayer->m_pEditorUI->getSelectedObjects();
					this->m_editorLayer->m_editorUI->pasteObjects(val);
					this->m_editorLayer->m_editorUI->deselectAll();
					// this->m_editorLayer->m_pEditorUI->selectObjects(objs, true);
				}
			},
			"Paste"
		);

		p->getInputNode()->getInputNode()->setAllowedChars(inputf_Default);
		p->show();
	}
};

void loadPasteButton(EditorPauseLayer* self) {
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto menu = static_cast<CCMenu*>(self->getChildByID("actions-menu"));

	auto btn = CCMenuItemSpriteExtra::create(
		CCNodeConstructor()
			.fromNode(ButtonSprite::create(
				"Paste String", 0x32, true, "bigFont.fnt", "GJ_button_04.png", 30.0, .6f
			))
			.done(),
		self, (SEL_MenuHandler)&EditorPauseLayer_CB::onPasteFromString
	);

	menu->addChild(btn);
	menu->updateLayout();
}

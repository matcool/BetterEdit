#include "addTab.hpp"

#include "../BetterEdit.hpp"

using namespace gdmake;

std::vector<std::tuple<std::string, addEditorTabFunc>> g_tabs;

void addEditorTab(char const* spr, addEditorTabFunc bbar) {
	g_tabs.push_back({ spr, bbar });
}

#include <Geode/modify/EditorUI.hpp>

class $modify(EditorUI) {
	void setupCreateMenu() {
		EditorUI::setupCreateMenu();

		if (BetterEdit::isEditorViewOnlyMode()) {
			return;
		}

		auto pos = m_tabsMenu->getPosition();

		for (auto [spr, bbarf] : g_tabs) {
			auto newTab_off = cocos2d::CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
			auto off_aspr = cocos2d::CCSprite::createWithSpriteFrameName(spr.c_str());

			off_aspr->setScale(.35f);
			off_aspr->setPosition(newTab_off->getScaledContentSize() / 2);

			newTab_off->addChild(off_aspr);

			newTab_off->setOpacity(150);

			auto newTab_on = cocos2d::CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");

			newTab_on->addChild(off_aspr);

			auto newTab = CCMenuItemToggler::create(
				newTab_off, newTab_on, this, (cocos2d::SEL_MenuHandler)&EditorUI::onSelectBuildTab
			);

			newTab->setSizeMult(1.2f);
			newTab->setClickable(false);
			newTab->setContentSize(
				extra::as<cocos2d::CCNode*>(m_tabsArray->objectAtIndex(0))->getContentSize()
			);

			newTab->setTag(m_tabsArray->count());

			auto bbar = bbarf(this);

			bbar->setVisible(false);

			m_tabsArray->addObject(newTab);
			m_tabsMenu->addChild(newTab);
			m_createButtonBars->addObject(bbar);
			this->addChild(bbar, 10);
		}

		// fix F1 & F2
		patch(0x92044, { 0xb9, static_cast<uint8_t>(m_tabsArray->count()) });
		patch(0x9207a, { 0x83, 0xf8, static_cast<uint8_t>(m_tabsArray->count() - 1) });

		auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
		auto ratio = winSize.width / winSize.height;

		if (ratio > 1.5f) {
			m_tabsMenu->alignItemsHorizontallyWithPadding(-3.0f);
		}
		else {
			m_tabsMenu->alignItemsHorizontallyWithPadding(-1.0f);
		}

		m_tabsMenu->setPosition(pos);
	}
};

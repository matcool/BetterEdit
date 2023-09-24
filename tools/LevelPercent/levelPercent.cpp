#include "levelPercent.hpp"

#include "../../utils/moveGameLayer.hpp"
#include "../AutoSave/autoSave.hpp"
#include "../CustomKeybinds/BEKeybinds.hpp"
#include "../EditorLayerInput/editorLayerInput.hpp"
#include "../RotateSaws/rotateSaws.hpp"

#include <InputPrompt.hpp>

static constexpr int const SLIDERLABEL_TAG = 420;
static constexpr int const EPOSITION_TAG = 421;
GameObject* g_lastObject = nullptr;
int g_bDontUpdateSlider = 0;

void updateLastObjectX(LevelEditorLayer* lel, GameObject* obj = nullptr);

float getLevelLength() {
	// from camden314/gdp/finished_works/PlayLayer/createObjectsFromSetup.cpp

	float screenEnd = CCDirector::sharedDirector()->getScreenRight() + 300.0f;
	auto res = screenEnd;

	// g_lastObject will sometimes be a released
	// objects;
	// i think we *might* want to hook undos /
	// redos to account for that
	// or just call retain on it and check the
	// retain count to see if it should be
	// released lol
	if (g_lastObject) {
		if (g_lastObject->retainCount() == 1u) {
			g_lastObject->release();
			g_lastObject = nullptr;
			updateLastObjectX(LevelEditorLayer::get());
		}
		else {
			res = g_lastObject->getPositionX() + 340.0f;
		}
	}

	if (res < screenEnd) {
		res = screenEnd;
	}

	return res;
}

void updateLastObjectX(LevelEditorLayer* lel, GameObject* obj) {
	if (obj == nullptr) {
		CCARRAY_FOREACH_B_TYPE(lel->m_objects, pObj, GameObject) {
			if (!g_lastObject) {
				g_lastObject = pObj;
				continue;
			}

			if (pObj->getPositionX() > g_lastObject->getPositionX()) {
				g_lastObject = pObj;
			}
		}
	}
	else {
		if (!g_lastObject) {
			g_lastObject = obj;
		}

		if (obj->getPositionX() > g_lastObject->getPositionX()) {
			g_lastObject = obj;
		}
	}

	if (g_lastObject) {
		g_lastObject->retain();
	}
}

void resetSliderPercent(EditorUI* self) {
	g_lastObject = nullptr;
}

void updatePercentLabelPosition(EditorUI* self) {
	if (!self) {
		return;
	}

	auto menu = as<CCMenu*>(self->m_positionSlider->getChildByTag(SLIDERLABEL_TAG));

	if (menu) {
		auto label = as<CCLabelBMFont*>(menu->getUserData());

		if (BetterEdit::getDisablePercentage()) {
			return menu->setVisible(false);
		}
		menu->setVisible(true);

		menu->setPositionX(self->m_positionSlider->m_touchLogic->m_thumb->getPositionX());

		float val = 0.0f;
		if (!BetterEdit::getUseOldProgressBar()) {
			val = self->m_positionSlider->getValue() * 100.0f;
		}
		else if (getLevelLength()) {
			val = self->m_editorLayer->getObjectLayer()
					  ->convertToNodeSpace(CCDirector::sharedDirector()->getWinSize() / 2)
					  .x /
				getLevelLength() * 100.0f;
		}

		val = std::min(val, 100.0f);
		val = std::max(val, 0.0f);

		label->setString(
			(BetterEdit::formatToString(val, BetterEdit::getPercentageAccuracy()) + "%").c_str()
		);
		label->setOpacity(255);

		if (BetterEdit::getFadeOutPercentage() && !BetterEdit::getDisablePercentage()) {
			label->stopAllActions();
			label->runAction(
				CCSequence::create(CCDelayTime::create(.5f), CCFadeOut::create(.5f), nullptr)
			);
		}
	}

	auto posLabel = as<CCLabelBMFont*>(self->getChildByTag(EPOSITION_TAG));

	if (posLabel) {
		auto pos = self->m_editorLayer->getObjectLayer()->getPosition() /
				self->m_editorLayer->getObjectLayer()->getScale() -
			CCDirector::sharedDirector()->getWinSize() / 2;

		posLabel->setString((std::to_string(-static_cast<int>(pos.x)) + "," +
							 std::to_string(-static_cast<int>(pos.y)))
								.c_str());
	}
}

/*
void showPositionLabel(EditorUI* self, bool show) {
	if (BetterEdit::getDisableEditorPos())
		CATCH_NULL(as<CCLabelBMFont*>(self->getChildByTag(EPOSITION_TAG)))->setVisible(false);
	else
		CATCH_NULL(as<CCLabelBMFont*>(self->getChildByTag(EPOSITION_TAG)))->setVisible(show);

	auto menu = as<CCMenu*>(self->m_positionSlider->getChildByTag(SLIDERLABEL_TAG));
	menu->setVisible(!BetterEdit::getDisablePercentage());
} //*/

void setIgnoreNewObjectsForSliderPercent(bool b) {
	if (b) {
		g_bDontUpdateSlider++;
	}
	else if (g_bDontUpdateSlider > 0) {
		g_bDontUpdateSlider--;
	}
}

void handleObjectAddForSlider(LevelEditorLayer* self, GameObject* obj) {
	if (g_bDontUpdateSlider || !self) {
		return;
	}

	if (obj) {
		updateLastObjectX(self, obj);
	}

	updatePercentLabelPosition(self->m_editorUI);

	if (self->m_editorUI) {
		self->m_editorUI->updateSlider();
	}
}

#include <Geode/modify/EditorUI.hpp>

class $modify(LevelPercentHooks, EditorUI) {
	void onGoToPercentage(CCObject* pSender) {
		auto p = InputPrompt::create(
			"Go To %", "%",
			[this](char const* txt) -> void {
				if (txt && strlen(txt)) {
					float val = 0.0f;
					try {
						val = std::stof(txt);
					}
					catch (...) {
					}

					auto width = CCDirector::sharedDirector()->getWinSize().width;

					this->m_editorLayer->getObjectLayer()->setPosition(
						{ (-getLevelLength() * std::min(val, 100.0f) / 100.0f + width / 2) *
							  this->m_editorLayer->getObjectLayer()->getScale(),
						  this->m_editorLayer->getObjectLayer()->getPositionY() }
					);

					this->constrainGameLayerPosition();
					this->updateSlider();
				}
			},
			"Go"
		);
		p->getInputNode()->getInputNode()->setAllowedChars("0123456789.");
		p->getInputNode()->getInputNode()->setMaxLabelWidth(6);
		p->show();
	}

	// hooks
	void sliderChanged(Slider* pSlider) {
		if (BetterEdit::getUseOldProgressBar()) {
			EditorUI::sliderChanged(pSlider);
			updatePercentLabelPosition(this);
			return;
		}

		float val = m_positionSlider->getValue() * 100.0f;

		auto width = CCDirector::sharedDirector()->getWinSize().width;

		float posX = 0.0f;
		posX = (-getLevelLength() * std::min(val, 100.0f) / 100.0f + width / 2) *
			m_editorLayer->getObjectLayer()->getScale();

		m_editorLayer->getObjectLayer()->setPosition(
			{ posX, m_editorLayer->getObjectLayer()->getPositionY() }
		);

		this->constrainGameLayerPosition();

		updatePercentLabelPosition(this);
	}

	float valueFromXPosRemake(float value) {
		if (BetterEdit::getUseOldProgressBar()) {
			return EditorUI::valueFromXPos(value);
		}

		float val = 0.0f;
		if (getLevelLength()) {
			val = m_editorLayer->getObjectLayer()
					  ->convertToNodeSpace(CCDirector::sharedDirector()->getWinSize() / 2)
					  .x /
				getLevelLength() * 100.0f;
		}

		val = std::min(val, 100.0f);
		val = std::max(val, 0.0f);

		val /= 100.0f;

		return val;
	}

	void moveObject(GameObject* obj, CCPoint pos) {
		// because gd sometimes passes a good ol'
		// nullptr to moveObject when you're
		// free moving and then press undo
		// (yes very specific bugfix)

		if (obj == nullptr) {
			return;
		}

		EditorUI::moveObject(obj, pos);

		if (g_bDontUpdateSlider) {
			return;
		}

		updateLastObjectX(m_editorLayer, obj);

		updatePercentLabelPosition(this);

		EditorUI::updateSlider();
	}

	void updateSlider() {
		m_positionSlider->setValue(
			this->valueFromXPosRemake(m_editorLayer->m_objectLayer->getPosition().x)
		);

		updatePercentLabelPosition(this);
	}
};

void loadSliderPercent(EditorUI* self) {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	auto menu = CCMenu::create();
	menu->setTag(SLIDERLABEL_TAG);

	auto label = CCLabelBMFont::create("100%", "bigFont.fnt");

	label->setPosition(0, 0);
	label->setScale(.4f);

	auto btn = CCMenuItemSpriteExtra::create(
		label, self, menu_selector(LevelPercentHooks::onGoToPercentage)
	);

	menu->setPosition(self->m_positionSlider->m_touchLogic->m_thumb->getPositionX(), -25.0f);
	menu->setUserData(label);
	menu->addChild(btn);
	self->m_positionSlider->addChild(menu, 999);

	updateLastObjectX(self->m_editorLayer);

	// self->addChild(
	//     CCNodeConstructor<CCLabelBMFont*>()
	//         .fromText("x,x", "bigFont.fnt")
	//         .scale(.3f)
	//         .move(winSize / 2 + CCPoint { -100.0f, 140.0f })
	//         .tag(EPOSITION_TAG)
	//         .done()
	// );

	// showPositionLabel(self, true);
	updatePercentLabelPosition(self);
}
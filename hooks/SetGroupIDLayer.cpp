#include "../BetterEdit.hpp"
#include "../utils.hpp"

#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <InputPrompt.hpp>

std::string g_nextFreeInput = "";
static constexpr int const NEXTFREE_TAG = 5000;
bool g_bZOrderMixed = false;
bool g_bEditorLayerMixed = false;
bool g_bEditorLayer2Mixed = false;

struct ObjectValueKeeper {
	int originalZOrder = 0;
	int originalELayer = 0;
	int originalELayer2 = 0;
};

std::map<GameObject*, ObjectValueKeeper> g_mStartValues;

// class AddTextDelegate : public CCNode, public TextInputDelegate {
// public:
// 	SetGroupIDLayer* m_pGIL;

// 	virtual void textChanged(CCTextInputNode* input) override {
// 		reinterpret_cast<void(__thiscall*)(SetGroupIDLayer*, CCTextInputNode*)>(base + 0x22d610)(
// 			this->m_pGIL, input
// 		);
// 	}

// 	static AddTextDelegate* create(SetGroupIDLayer* gil) {
// 		auto ret = new AddTextDelegate();

// 		if (ret && ret->init()) {
// 			ret->m_pGIL = gil;
// 			ret->autorelease();
// 			return ret;
// 		}

// 		CC_SAFE_DELETE(ret);
// 		return nullptr;
// 	}
// };

void clampZOrder(int& val) {
	if (val < -999) {
		val = -999;
	}
	if (val > 9999) {
		val = 9999;
	}
}

void clampELayer(int& val) {
	if (val < 0) {
		val = 0;
	}

	// since layer locking doesn't support
	// anything past 500, maybe i should
	// limit this aswell. but idk
}

void updateInput(bool many, bool mixed, CCLabelBMFont* label, int val) {
	if (many || !mixed) {
		label->setString(CCString::createWithFormat("%i", val)->getCString());
	}
	else {
		if (val) {
			if (val > 0) {
				label->setString(CCString::createWithFormat("Mix+%i", val)->getCString());
			}
			else {
				label->setString(CCString::createWithFormat("Mix%i", val)->getCString());
			}
		}
		else {
			label->setString("Mixed");
		}
	}
}

#include <Geode/modify/SetGroupIDLayer.hpp>

class $modify(SetGroupIDLayer) {
	void turnLabelIntoInput(
		CCLabelBMFont* text, int id, char const* ov, std::string const& ac = "0123456789"
	) {
		text->setVisible(false);

		auto spr =
			extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });

		spr->setScale(.5f);
		spr->setColor({ 0, 0, 0 });
		spr->setOpacity(100);
		spr->setContentSize({ 80.0f, 60.0f });
		spr->setPosition(text->getPosition());

		auto eLayerInput = CCTextInputNode::create(50.0f, 40.0f, "0", 24, "bigFont.fnt");

		eLayerInput->setPosition(text->getPosition());
		eLayerInput->setLabelPlaceholderColor({ 120, 120, 120 });
		eLayerInput->setAllowedChars(ac);
		eLayerInput->setAnchorPoint({ 0, 0 });
		eLayerInput->setScale(.7f);
		eLayerInput->setDelegate(this);
		eLayerInput->setUserData(reinterpret_cast<void*>(id));
		eLayerInput->setTag(id + 64);
		eLayerInput->setString(ov);

		this->addChild(spr);
		this->addChild(eLayerInput);
	}

	void figureOutMixed(SetGroupIDLayer* self) {
		g_bZOrderMixed = m_zOrderValue == -1000;
		g_bEditorLayerMixed = m_editorLayerValue == -1;
		g_bEditorLayer2Mixed = m_editorLayer2Value == -1;

		if (g_bZOrderMixed) {
			m_zOrderValue = 0;
		}
		if (g_bEditorLayerMixed) {
			m_editorLayerValue = 0;
		}
		if (g_bEditorLayer2Mixed) {
			m_editorLayer2Value = 0;
		}

		g_mStartValues.clear();
		CCARRAY_FOREACH_B_TYPE(m_targetObjects, obj, GameObject) {
			g_mStartValues[obj] = { obj->m_gameZOrder, obj->m_editorLayer, obj->m_editorLayer2 };
		}
	}

	void onCustomNextFree(CCObject* pSender) {
		auto p =
			InputPrompt::create(
				"Next Free Offset", "ID",
				[this](char const* txt) -> void {
					if (txt && strlen(txt)) {
						auto startID = 0;
						try {
							startID = std::stoi(txt);
						}
						catch (...) {
						}

						auto bytes = intToBytes(startID);
						bytes.insert(bytes.begin(), 0xbe);

						patchBytes(0x164c59, bytes);
						auto id = LevelEditorLayer::get()->getNextFreeGroupID(nullptr);

						this->m_groupIDValue = id;
						this->updateGroupIDLabel();

						g_nextFreeInput = txt;
					}
					else {
						g_nextFreeInput = "";
						patchBytes(0x164c59, { 0xbe, 0x01, 0x00, 0x00, 0x00 });
					}

					auto btn =
						as<CCMenuItemSpriteExtra*>(this->m_buttonMenu->getChildByTag(NEXTFREE_TAG));

					if (btn) {
						auto spr = as<ButtonSprite*>(btn->getNormalImage());
						spr->setString(g_nextFreeInput.size() ? g_nextFreeInput.c_str() : "0");
						spr->updateBGImage(
							g_nextFreeInput.size() ? "GJ_button_02.png" : "GJ_button_04.png"
						);
					}
				},
				"Apply"
			)
				->setApplyOnEsc(true)
				->setTrashButton(true);
		p->getInputNode()->getInputNode()->setAllowedChars("0123456789");
		p->getInputNode()->getInputNode()->setMaxLabelWidth(6);
		p->getInputNode()->setString(g_nextFreeInput.c_str());
		p->show();
	}

	void updateEditorLayerIDRemake() {
		if (m_targetObject) {
			m_targetObject->m_editorLayer = m_editorLayerValue;
			clampELayer(m_targetObject->m_editorLayer);
		}
		CCARRAY_FOREACH_B_TYPE(m_targetObjects, obj, GameObject) {
			if (g_bEditorLayerMixed) {
				obj->m_editorLayer = g_mStartValues[obj].originalELayer + m_editorLayerValue;
			}
			else {
				obj->m_editorLayer = m_editorLayerValue;
			}
			clampELayer(obj->m_editorLayer);
		}
	}

	void updateEditorLayerID2Remake() {
		if (m_targetObject) {
			m_targetObject->m_editorLayer2 = m_editorLayer2Value;
			clampELayer(m_targetObject->m_editorLayer2);
		}
		CCARRAY_FOREACH_B_TYPE(m_targetObjects, obj, GameObject) {
			if (g_bEditorLayer2Mixed) {
				obj->m_editorLayer2 = g_mStartValues[obj].originalELayer2 + m_editorLayer2Value;
			}
			else {
				obj->m_editorLayer2 = m_editorLayer2Value;
			}
			clampELayer(obj->m_editorLayer2);
		}
	}

	void updateZOrderRemake() {
		if (m_targetObject) {
			m_targetObject->m_gameZOrder = m_zOrderValue;
			m_targetObject->m_shouldUpdateColorSprite = true;
			clampZOrder(m_targetObject->m_gameZOrder);
		}

		CCARRAY_FOREACH_B_TYPE(m_targetObjects, obj, GameObject) {
			if (g_bZOrderMixed) {
				obj->m_gameZOrder = g_mStartValues[obj].originalZOrder + m_zOrderValue;
			}
			else {
				obj->m_gameZOrder = m_zOrderValue;
			}
			clampZOrder(obj->m_gameZOrder);
			obj->m_shouldUpdateColorSprite = true;
		}
	}

	// hooks

	void onEditorLayer(CCObject* pSender) {
		m_editorLayerValue += pSender->getTag() ? 1 : -1;

		if (!g_bEditorLayerMixed) {
			clampELayer(m_editorLayerValue);
		}

		updateInput(m_targetObject, g_bEditorLayerMixed, m_editorLayerText, m_editorLayerValue);

		CATCH_NULL(as<CCTextInputNode*>(this->getChildByTag(69)))
			->setString(m_editorLayerText->getString());

		this->updateEditorLayerIDRemake();
	}

	void onEditorLayer2(CCObject* pSender) {
		m_editorLayer2Value += pSender->getTag() ? 1 : -1;

		if (!g_bEditorLayer2Mixed) {
			clampELayer(m_editorLayer2Value);
		}

		updateInput(m_targetObject, g_bEditorLayer2Mixed, m_editorLayer2Text, m_editorLayer2Value);

		CATCH_NULL(as<CCTextInputNode*>(this->getChildByTag(70)))
			->setString(m_editorLayer2Text->getString());

		this->updateEditorLayerID2Remake();
	}

	void onZOrder(CCObject* pSender) {
		auto add = pSender->getTag() ? 1 : -1;
		m_zOrderValue += add;

		// if (m_zOrderValue == 0)
		//     m_zOrderValue = add;

		updateInput(m_targetObject, g_bZOrderMixed, m_zOrderText, m_zOrderValue);

		CATCH_NULL(as<CCTextInputNode*>(this->getChildByTag(71)))
			->setString(m_zOrderText->getString());

		this->updateZOrderRemake();
	}

	void textChanged(CCTextInputNode* input) {
		int val = 0;
		bool isInt = true;
		bool mixed = false;

		if (input->getString() && strlen(input->getString())) {
			try {
				std::string str = input->getString();
				if (stringToLower(str).find("m") != std::string::npos) {
					mixed = true;
				}
				while (str.size() && (!isdigit(str[0]) && str[0] != '-')) {
					str = str.substr(1);
				}
				if (str.size() && str != "-") {
					val = std::stoi(str);
				}
			}
			catch (...) {
				isInt = false;
			}
		}

		switch (reinterpret_cast<intptr_t>(input->getUserData())) {
			case 5:
				if (!isInt) {
					return;
				}
				if (!mixed) {
					g_bEditorLayerMixed = false;
				}
				m_editorLayerValue = val;
				this->updateEditorLayerIDRemake();
				break;

			case 6:
				if (!isInt) {
					return;
				}
				if (!mixed) {
					g_bEditorLayer2Mixed = false;
				}
				m_editorLayer2Value = val;
				this->updateEditorLayerID2Remake();
				break;

			case 7:
				if (!isInt) {
					return;
				}
				if (!mixed) {
					g_bZOrderMixed = false;
				}
				m_zOrderValue = val;
				this->updateZOrderRemake();
				break;

			default: return SetGroupIDLayer::textChanged(input);
		}
	}

	bool init(GameObject* obj, CCArray* arr) {
		if (!SetGroupIDLayer::init(obj, arr)) {
			return false;
		}

		figureOutMixed(this);

		// auto nextFreeBtn = getChild<CCMenuItemSpriteExtra*>(m_buttonMenu, 2);
		// nextFreeBtn->setPositionX(nextFreeBtn->getPositionX() - 20.0f);

		// auto customNextFreeBtn = CCMenuItemSpriteExtra::create(
		// 	ButtonSprite::create(
		// 		g_nextFreeInput.size() ? g_nextFreeInput.c_str() : "0", 20, true, "goldFont.fnt",
		// 		g_nextFreeInput.size() ? "GJ_button_02.png" : "GJ_button_04.png", 25, .6f
		// 	),
		// 	self, (SEL_MenuHandler)&SetGroupIDLayer_CB::onCustomNextFree
		// );
		// customNextFreeBtn->setTag(NEXTFREE_TAG);
		// customNextFreeBtn->setPosition(
		// 	nextFreeBtn->getPositionX() + 58.0f, nextFreeBtn->getPositionY()
		// );
		// m_buttonMenu->addChild(customNextFreeBtn);

		turnLabelIntoInput(m_editorLayerText, 5, m_editorLayerText->getString());
		turnLabelIntoInput(m_editorLayer2Text, 6, m_editorLayer2Text->getString());
		turnLabelIntoInput(m_zOrderText, 7, m_zOrderText->getString(), "MIXEDmixed-+0123456789");

		return true;
	}
};

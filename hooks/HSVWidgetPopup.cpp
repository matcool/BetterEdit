#include <GDMake.h>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

using namespace gdmake;
using namespace gdmake::extra;
using namespace gd;
using namespace cocos2d;

intptr_t g_hsvColorChannel = 0;
static constexpr int const HSVSQUARE1_TAG = 0xB00B5; // i am incredibly mature
static constexpr int const HSVSQUARE2_TAG = 0xB00BA;

class AddHSVTextDelegate : public CCNode, public TextInputDelegate {
public:
	ConfigureHSVWidget* m_pGIL;

	virtual void textChanged(CCTextInputNode* input) override {
		float val = 0.0f;

		if (input->getString() && strlen(input->getString())) {
			val = static_cast<float>(std::atof(input->getString()));
		}

		switch (reinterpret_cast<intptr_t>(input->getUserData())) {
			case 5:
				m_pGIL->m_hueSlider->setValue((val + 180.0f) / 360.0f);
				m_pGIL->m_value.h = val;
				break;

			case 6:
				if (m_pGIL->m_value.absoluteSaturation) {
					m_pGIL->m_saturationSlider->setValue((val + 1.0f) / 2);
				}
				else {
					m_pGIL->m_saturationSlider->setValue(val / 2);
				}
				m_pGIL->m_value.s = val;
				break;

			case 7:
				if (m_pGIL->m_value.absoluteBrightness) {
					m_pGIL->m_brightnessSlider->setValue((val + 1.0f) / 2);
				}
				else {
					m_pGIL->m_brightnessSlider->setValue(val / 2);
				}
				m_pGIL->m_value.v = val;
				break;
		}
	}

	static AddHSVTextDelegate* create(ConfigureHSVWidget* gil) {
		auto ret = new AddHSVTextDelegate();

		if (ret && ret->init()) {
			ret->m_pGIL = gil;
			ret->autorelease();
			return ret;
		}

		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

void updateInputText(CCTextInputNode* upd, float val) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << val;
	std::string s = stream.str();

	upd->setString(s.c_str());
}

void turnLabelIntoInput(
	ConfigureHSVWidget* self, CCLabelBMFont* text, AddHSVTextDelegate* ad, int id, float ov,
	char const* title, std::string const& ac = "-0123456789."
) {
	text->setVisible(false);

	auto spr = extension::CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });

	spr->setScale(.3f);
	spr->setColor({ 0, 0, 0 });
	spr->setOpacity(100);
	spr->setContentSize({ 235.0f, 70.0f });
	spr->setPosition(text->getPosition() + CCPoint { 50.0f, 0.0f });

	auto eLayerInput = CCTextInputNode::create(70.0f, 20.0f, "0", 24, "bigFont.fnt");

	eLayerInput->setPosition(text->getPosition() + CCPoint { 50.0f, 0.0f });
	eLayerInput->setLabelPlaceholderColor({ 120, 120, 120 });
	eLayerInput->setAllowedChars(ac);
	eLayerInput->setAnchorPoint({ 0, 0 });
	eLayerInput->setMaxLabelWidth(4);
	eLayerInput->setScale(.7f);
	eLayerInput->setDelegate(ad);
	eLayerInput->setUserData(reinterpret_cast<void*>(id));
	eLayerInput->setTag(id + 64);
	updateInputText(eLayerInput, ov);

	auto txt = CCLabelBMFont::create(title, "goldFont.fnt");
	txt->limitLabelWidth(85.0f, 1.0f, .3f);
	txt->setPosition(text->getPosition() - CCPoint { 35.0f, 0.0f });

	self->addChild(spr);
	self->addChild(eLayerInput);
	self->addChild(txt);
}

ccColor3B getColorChannelBaseColor(int channelID, int stackLimit = 0) {
	auto col = LevelEditorLayer::get()->m_levelSettings->m_effectManager->getColorAction(channelID);

	if (stackLimit > 10 || !col) {
		return { 255, 255, 255 };
	}

	if (col->m_copyID) {
		auto copy = getColorChannelBaseColor(col->m_copyID, stackLimit + 1);

		auto col3b = GameToolbox::transformColor(copy, col->m_copyHSV);

		return col3b;
	}

	return col->m_color;
}

void updateHSVPreviewColor(HSVWidgetPopup* self) {
	auto prev = as<CCSprite*>(self->m_mainLayer->getChildByTag(HSVSQUARE2_TAG));

	if (prev) {
		auto col = getColorChannelBaseColor(as<intptr_t>(prev->getUserData()));

		auto col3b = GameToolbox::transformColor(col, self->m_configureWidget->m_value);

		prev->setColor(col3b);
	}
}

#include <Geode/modify/ConfigureHSVWidget.hpp>

class $modify(ConfigureHSVWidget) {
	void updateLabels() {
		ConfigureHSVWidget::updateLabels();

		if (this->getChildByTag(69)) {
			updateInputText(reinterpret_cast<CCTextInputNode*>(this->getChildByTag(69)), m_value.h);
		}
		if (this->getChildByTag(70)) {
			updateInputText(reinterpret_cast<CCTextInputNode*>(this->getChildByTag(70)), m_value.s);
		}
		if (this->getChildByTag(71)) {
			updateInputText(reinterpret_cast<CCTextInputNode*>(this->getChildByTag(71)), m_value.v);
		}

		if (this->getUserData()) {
			updateHSVPreviewColor(as<HSVWidgetPopup*>(this->getUserData()));
		}
	}

	bool init(int abs, ccHSVValue val) {
		if (!ConfigureHSVWidget::init(abs, val)) {
			return false;
		}

		auto hsva = AddHSVTextDelegate::create(this);

		turnLabelIntoInput(this, m_hueLabel, hsva, 5, m_value.h, "Hue");
		turnLabelIntoInput(this, m_saturationLabel, hsva, 6, m_value.s, "Saturation");
		turnLabelIntoInput(this, m_brightnessLabel, hsva, 7, m_value.v, "Brightness");

		this->addChild(hsva);

		return true;
	}
};

#include <Geode/modify/CustomizeObjectLayer.hpp>

class $modify(CustomizeObjectLayer) {
	void onHSV(CCMenuItemSpriteExtra* pSender) {
		g_hsvColorChannel = this->getActiveMode(true);

		CustomizeObjectLayer::onHSV(pSender);
	}
};

#include <Geode/modify/HSVWidgetPopup.hpp>

class $modify(HSVWidgetPopup) {
	bool init(ccHSVValue hsv, HSVWidgetPopupDelegate* delegate, std::string str) {
		if (!HSVWidgetPopup::init(hsv, delegate, str)) {
			return false;
		}

		if (g_hsvColorChannel < 1) {
			return true;
		}

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto col = getColorChannelBaseColor(g_hsvColorChannel);

		auto square1 = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");

		square1->setPosition({ winSize.width / 2 - 155.0f, winSize.height / 2 - 15.0f });
		square1->setColor(col);
		square1->setTag(HSVSQUARE1_TAG);
		square1->setUserData(as<void*>(g_hsvColorChannel));

		m_mainLayer->addChild(square1);

		auto square2 = CCSprite::createWithSpriteFrameName("whiteSquare60_001.png");

		square2->setPosition({ winSize.width / 2 - 155.0f, winSize.height / 2 + 15.0f });
		square2->setColor(col);
		square2->setTag(HSVSQUARE2_TAG);
		square2->setUserData(as<void*>(g_hsvColorChannel));

		m_mainLayer->addChild(square2);
		m_configureWidget->setUserData(this);

		updateHSVPreviewColor(this);

		g_hsvColorChannel = 0;

		return true;
	}
};

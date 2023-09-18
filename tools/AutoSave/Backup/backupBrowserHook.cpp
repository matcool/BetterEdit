#include "LevelBackupManager.hpp"

void LevelCell_loadLocalLevelCell(LevelCell* self) {
    matdash::orig<&LevelCell_loadLocalLevelCell>(self);

    if (LevelBackupManager::get()->levelHasBackups(self->m_pLevel)) {
        auto label = getChild<CCLabelBMFont*>(self->m_mainLayer, 0);

        auto backupIcon = CCSprite::createWithSpriteFrameName("GJ_colorBtn_001.png");

        auto bLabel = CCLabelBMFont::create("B", "bigFont.fnt");

        bLabel->setScale(.9f);
        // bLabel->setColor({ 0, 0, 0 });
        bLabel->setPosition(backupIcon->getContentSize() / 2);

        backupIcon->addChild(bLabel);

        backupIcon->setScale(.35f);
        backupIcon->setColor({ 55, 185, 255 });
        backupIcon->setPosition({
            label->getPositionX() + label->getScaledContentSize().width + 12.5f,
            label->getPositionY()
        });

        self->m_mainLayer->addChild(backupIcon);
    }
} MAT_GDMAKE_HOOK(0x5be30, LevelCell_loadLocalLevelCell)

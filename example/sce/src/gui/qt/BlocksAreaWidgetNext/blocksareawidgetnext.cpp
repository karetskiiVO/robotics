#include <QGroupBox>
#include <QVBoxLayout>
#include "blocksareawidgetnext.h"
#include "ui_blocksareawidgetnext.h"

BlocksAreaWidgetNext::BlocksAreaWidgetNext(QWidget *parent)
    : AbstractSettingsWidget(parent)
    , m_jsonSettings(nlohmann::json::object())
    , m_oldJsonSettings(nlohmann::json::object())
    , ui(new Ui::BlocksAreaWidgetNext)
{
    ui->setupUi(this);
}

BlocksAreaWidgetNext::~BlocksAreaWidgetNext()
{
    delete ui;
}

std::string BlocksAreaWidgetNext::SettingsJSON() const
{
    return nlohmann::to_string(m_oldJsonSettings);
}

void BlocksAreaWidgetNext::AddBlockSettingsWidget(const std::string& name, AbstractSettingsWidget* settings)
{
    m_children.append({name, settings});
    auto groupBox = new QGroupBox(tr("Settings of ") + QString::fromStdString(name), this);
    auto layout = new QVBoxLayout(groupBox);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(settings, 0, Qt::AlignTop | Qt::AlignLeft);
    groupBox->setLayout(layout);
    ui->settingsAreaLayout->addWidget(groupBox, 0, Qt::AlignTop | Qt::AlignLeft);
    // Changing the child's settings means changing our settings
    connect(settings, &AbstractSettingsWidget::settingsChanged, this, [this]() {
        emit this->settingsChanged();
    });
}

void BlocksAreaWidgetNext::restoreSettings()
{
    for (auto& pair: m_children) {
        pair.widget->restoreSettings();
    }
    m_jsonSettings = m_oldJsonSettings;
}

void BlocksAreaWidget::saveSettings()
{
    m_jsonSettings.clear();
    for (auto& pair: m_children) {
        pair.widget->saveSettings();
        m_jsonSettings[pair.name] = nlohmann::json(pair.widget->SettingsJSON());
    }
    m_oldJsonSettings = m_jsonSettings;
}
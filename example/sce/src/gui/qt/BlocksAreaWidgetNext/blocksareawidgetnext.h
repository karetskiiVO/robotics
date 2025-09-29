#ifndef BLOCKSAREAWIDGETNEXT_H
#define BLOCKSAREAWIDGETNEXT_H

#include <nlohmann/json.hpp>
#include "abstractsettingswidget.h"

namespace Ui {
class BlocksAreaWidgetNext;
}

// This widget should replace the current BlocksAreaWidget after the API is fixed.
class BlocksAreaWidgetNext : public AbstractSettingsWidget {
    Q_OBJECT

public:
    explicit BlocksAreaWidgetNext(QWidget *parent = nullptr);
    ~BlocksAreaWidgetNext() override;
    std::string SettingsJSON() const override;
    void AddBlockSettingsWidget(const std::string& name, AbstractSettingsWidget* settings);

public slots:
    void saveSettings() override;
    void restoreSettings() override;

private:
    struct NameWidgetPair {
        std::string name;
        AbstractSettingsWidget* widget;
    };
    QList<NameWidgetPair> m_children;
    nlohmann::json m_jsonSettings;
    nlohmann::json m_oldJsonSettings;
    Ui::BlocksAreaWidgetNext *ui;
};

#endif // BLOCKSAREAWIDGETNEXT_H

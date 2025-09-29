#ifndef QLONGSPINBOX_H
#define QLONGSPINBOX_H
#include <cstdint>
#include <QAbstractSpinBox>

class Q_GUI_EXPORT QLongSpinBox : public QAbstractSpinBox
{
    Q_OBJECT

    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix)
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix)
    Q_PROPERTY(QString cleanText READ cleanText)
    Q_PROPERTY(qlonglong minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qlonglong maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qlonglong singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(qlonglong value READ value WRITE setValue NOTIFY valueChanged USER true)
public:
    explicit QLongSpinBox(QWidget *parent = nullptr);

    qlonglong value() const;

    QString prefix() const;
    void setPrefix(const QString &prefix);

    QString suffix() const;
    void setSuffix(const QString &suffix);

    QString cleanText() const;

    qlonglong singleStep() const;
    void setSingleStep(qlonglong val);

    qlonglong minimum() const;
    void setMinimum(qlonglong min);

    qlonglong maximum() const;
    void setMaximum(qlonglong max);

    void setRange(qlonglong min, qlonglong max);

public slots:
    void setValue(qlonglong value);

signals:
    void valueChanged(qlonglong i);
    void valueChanged(const QString &text);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void stepBy(int steps) override;
    StepEnabled stepEnabled() const override;
    QValidator::State validate(QString &input, int &pos) const override;

private slots:
    void onTextChanged(const QString &newText);

private:
    void lineEditEditingFinalize();
    void selectCleanText();

    QString m_prefix;
    QString m_suffix;
    qlonglong m_singleStep;
    qlonglong m_minimum;
    qlonglong m_maximum;
    qlonglong m_value;

private:
    Q_DISABLE_COPY(QLongSpinBox)
};

#endif //QLONGSPINBOX_H

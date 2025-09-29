// Copyright 2014-2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#include "qlongspinbox.h"
#include <limits>
#include <QKeyEvent>
#include <QLineEdit>

QLongSpinBox::QLongSpinBox(QWidget *parent)
    : QAbstractSpinBox(parent)
{
    m_minimum = std::numeric_limits<qlonglong>::min();
    m_maximum = std::numeric_limits<qlonglong>::max();
    m_value = 0;
    m_singleStep = 1;

    setValue(m_value);
    connect(lineEdit(), &QLineEdit::textChanged, this, &QLongSpinBox::onTextChanged);
}

qlonglong QLongSpinBox::value() const
{
    return m_value;
}

void QLongSpinBox::setValue(qlonglong expectedNewValue)
{
    const qlonglong newValue = qBound(m_minimum, expectedNewValue, m_maximum);
    const QString newValueString = QString::number(newValue);
    lineEdit()->setText(m_prefix + newValueString + m_suffix);
    if (m_value != newValue) {
        m_value = newValue;

        emit valueChanged(newValue);
        emit valueChanged(newValueString);
    }
}

QString QLongSpinBox::prefix() const
{
    return m_prefix;
}

void QLongSpinBox::setPrefix(const QString &prefix)
{
    m_prefix = prefix;

    setValue(m_value);
}

QString QLongSpinBox::suffix() const
{
    return m_suffix;
}

void QLongSpinBox::setSuffix(const QString &suffix)
{
    m_suffix = suffix;

    setValue(m_value);
}

QString QLongSpinBox::cleanText() const
{
    return QString::number(m_value);
}

qlonglong QLongSpinBox::singleStep() const
{
    return m_singleStep;
}

void QLongSpinBox::setSingleStep(qlonglong step)
{
    m_singleStep = step;
}

qlonglong QLongSpinBox::minimum() const
{
    return m_minimum;
}

void QLongSpinBox::setMinimum(qlonglong min)
{
    m_minimum = min;
    if (m_maximum < m_minimum) {
        m_maximum = m_minimum;
    }

    setValue(m_value);
}

qlonglong QLongSpinBox::maximum() const
{
    return m_maximum;
}

void QLongSpinBox::setMaximum(qlonglong max)
{
    m_maximum = max;
    if (m_maximum < m_minimum) {
        m_maximum = m_minimum;
    }

    setValue(m_value);
}

void QLongSpinBox::setRange(qlonglong min, qlonglong max)
{
    if (min < max) {
        m_minimum = min;
        m_maximum = max;
    } else {
        m_minimum = max;
        m_maximum = min;
    }

    setValue(m_value);
}

void QLongSpinBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        selectCleanText();
        lineEditEditingFinalize();
    default:;
    }

    QAbstractSpinBox::keyPressEvent(event);
}

void QLongSpinBox::focusOutEvent(QFocusEvent *event)
{
    lineEditEditingFinalize();

    QAbstractSpinBox::focusOutEvent(event);
}

QAbstractSpinBox::StepEnabled QLongSpinBox::stepEnabled() const
{
    if (isReadOnly()) {
        return StepNone;
    }

    StepEnabled se = StepNone;
    if (wrapping() || m_value < m_maximum) {
        se |= StepUpEnabled;
    }
    if (wrapping() || m_value > m_minimum) {
        se |= StepDownEnabled;
    }

    return se;
}

void QLongSpinBox::stepBy(int steps)
{
    if (isReadOnly()) {
        return;
    }

    if (m_prefix + QString::number(m_value) + m_suffix != lineEdit()->text()) {
        lineEditEditingFinalize();
    }

    qlonglong newValue = m_value + (steps * m_singleStep);
    if (wrapping()) {
        // emulating the behavior of QSpinBox
        if (newValue > m_maximum) {
            if (m_value == m_maximum) {
                newValue = m_minimum;
            } else {
                newValue = m_maximum;
            }
        } else if (newValue < m_minimum) {
            if (m_value == m_minimum) {
                newValue = m_maximum;
            } else {
                newValue = m_minimum;
            }
        }
    } else {
        newValue = qBound(m_minimum, newValue, m_maximum);
    }

    setValue(newValue);
    selectCleanText();
}

QValidator::State QLongSpinBox::validate(QString &input, int &pos) const
{
    // first, we try to interpret as a number without prefixes
    bool ok;
    if (const qlonglong value = input.toLongLong(&ok);
        input.isEmpty() || (ok && value <= m_maximum)) {
        input = m_prefix + input + m_suffix;
        pos += m_prefix.length();
        return QValidator::Acceptable;
    }

    // if string of text editor aren't simple number, try to interpret it
    // as a number with prefix and suffix
    bool valid = true;
    if (!m_prefix.isEmpty() && !input.startsWith(m_prefix)) {
        valid = false;
    }
    if (!m_suffix.isEmpty() && !input.endsWith(m_suffix)) {
        valid = false;
    }

    if (valid) {
        const int start = m_prefix.length();
        const int length = input.length() - start - m_suffix.length();

        ok = false;
        const QString number = input.mid(start, length);
        if (const qlonglong value = number.toLongLong(&ok);
            number.isEmpty() || (ok && value <= m_maximum)) {
            return QValidator::Acceptable;
        }
    }

    // otherwise not acceptable
    return QValidator::Invalid;
}

void QLongSpinBox::lineEditEditingFinalize()
{
    const QString text = lineEdit()->text();

    // first, we try to read as a number without prefixes
    bool ok;
    qlonglong value = text.toLongLong(&ok);
    if (ok) {
        setValue(value);
        return;
    }

    // if string of text editor aren't simple number, try to interpret it
    // as a number with prefix and suffix
    bool valid = true;
    if (!m_prefix.isEmpty() && !text.startsWith(m_prefix)) {
        valid = false;
    } else if (!m_suffix.isEmpty() && !text.endsWith(m_suffix)) {
        valid = false;
    }

    if (valid) {
        const int start = m_prefix.length();
        const int length = text.length() - start - m_suffix.length();

        ok = false;
        value = text.mid(start, length).toLongLong(&ok);
        if (ok) {
            setValue(value);
            return;
        }
    }

    // otherwise set old value
    setValue(m_value);
}

void QLongSpinBox::selectCleanText()
{
    lineEdit()->setSelection(
        m_prefix.length(), lineEdit()->text().length() - m_prefix.length() - m_suffix.length());
}

void QLongSpinBox::onTextChanged(const QString &newText)
{
    bool ok = false;
    qlonglong newVal = newText.toLongLong(&ok);
    if (!ok) {

        if (!m_prefix.isEmpty() && newText.startsWith(m_prefix) &&
            !m_suffix.isEmpty() && newText.endsWith(m_suffix)) {
            const int start = m_prefix.length();
            const int length = newText.length() - start - m_suffix.length();
            newVal = newText.mid(start, length).toLongLong(&ok);
            }
    }


    if (ok) {

        newVal = qBound(m_minimum, newVal, m_maximum);
        if (newVal != m_value) {
            m_value = newVal;

            lineEdit()->blockSignals(true);
            lineEdit()->setText(m_prefix + QString::number(m_value) + m_suffix);
            lineEdit()->blockSignals(false);

            emit valueChanged(m_value);
            emit valueChanged(QString::number(m_value));
        }
    }
}

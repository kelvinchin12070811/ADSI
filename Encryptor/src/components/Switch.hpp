/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QCheckBox>

namespace components {
/**
 * @brief Toggle switches.
 */
class Switch : public QCheckBox
{
public:
    /**
     * @brief Create switch with default value.
     * @param parent Parent of the switch, nullptr if no parent.
     */
    explicit Switch(QWidget *parent = nullptr);
    /**
     * @brief Create switch with @p value of state.
     * @param value State of the switch.
     * @param parent Parent of the switch, nullptr if no parent.
     */
    explicit Switch(bool value, QWidget *parent = nullptr);

protected:
    /**
     * @brief Custom widget painting.
     * @param paintEvent Controls of current event.
     */
    void paintEvent(QPaintEvent *paintEvent) override;
};
}
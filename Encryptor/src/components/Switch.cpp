/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QPainter>

#include "components/Switch.hpp"

namespace components
{
    Switch::Switch(QWidget *parent):
        QCheckBox(parent)
    {
    }
    
    Switch::Switch(bool value, QWidget *parent):
        QCheckBox(parent)
    {
        this->setChecked(value);
    }
    
    void Switch::paintEvent(QPaintEvent *paintEvent)
    {
        static_cast<void>(paintEvent);
        constexpr int labelLeftMargin{ 5 };
        constexpr int labelTopMargin{ 2 };

        QPainter painter{ this };
        QRect drawRect{ this->pos(), this->size() };
        QColor bgColor = Qt::GlobalColor::gray;

        painter.fillRect(
            QRect{
                QPoint{ drawRect.x(), drawRect.y() },
                QSize{ static_cast<int>(std::roundf(1.5f * drawRect.height())), drawRect.height() }
            },
            bgColor
        );
        painter.drawText(
            drawRect.x() + static_cast<int>(std::roundf(1.5 * drawRect.height()) + labelLeftMargin),
            drawRect.y() + static_cast<int>(std::roundf(drawRect.height() * .5) + labelTopMargin),
            this->text()
        );
    }
}
/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QPainter>
#include <QPainterPath>

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
        constexpr int labelTopMargin{ 0 };

        QPainter painter{ this };
        QRect drawRect{ this->pos(), this->size() };
        const int circleRadius{ static_cast<int>(drawRect.height() / 2.5f) };
        QColor bgColour = this->isChecked() ? QColor{ 0x64, 0xB5, 0xF6 } : QColor(0xBD, 0xBD, 0xBD);
        QPainterPath switchOutline;
        switchOutline.addRoundedRect(
            QRect{
                QPoint{ drawRect.x(), drawRect.y() + static_cast<int>(.15f * drawRect.height()) },
                QSize{
                    static_cast<int>(std::roundf(1.5f * drawRect.height())),
                    static_cast<int>(std::roundf(.5f * drawRect.height()))
                }
            },
            static_cast<int>(.75 * circleRadius),
            static_cast<int>(.75 * circleRadius)
        );
        QPainterPath switchToggler;
        
        if (!this->isChecked())
        {
            switchToggler.addEllipse(
                QPoint{ drawRect.x() + circleRadius, drawRect.y() + circleRadius },
                circleRadius,
                circleRadius
            );
        }
        else
        {
            switchToggler.addEllipse(
                QPoint{
                    drawRect.x() + circleRadius +
                        static_cast<int>(std::roundf(1.5 * drawRect.height() - (circleRadius * 2))),
                    drawRect.y() + circleRadius
                },
                circleRadius,
                circleRadius
            );
        }

        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::HighQualityAntialiasing);

        painter.fillPath(switchOutline, bgColour);

        if (!this->isChecked())
            painter.fillPath(switchToggler, QColor{ 0x75, 0x75, 0x75 });
        else
            painter.fillPath(switchToggler, QColor{ 0x21, 0x96, 0xF3 });

        painter.drawText(
            drawRect.x() + static_cast<int>(std::roundf(1.5 * drawRect.height()) + labelLeftMargin),
            drawRect.y() + static_cast<int>(std::roundf(drawRect.height() * .5) + labelTopMargin),
            this->text()
        );
    }
}
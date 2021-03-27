/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QLabel>
#include <QImage>

namespace components {
/**
 * @brief Preview panel to render image which targeted to sign.
*/
class ImagePreview : public QLabel
{
public:
    /**
     * @brief Create preview panel of the image.
     * @param image Image to preview, must not be null.
     * @throws std::
     */
    explicit ImagePreview(QImage *image);

private:
    /**
     * @brief Image to preview.
     */
    QImage *image_ { nullptr };
};
}
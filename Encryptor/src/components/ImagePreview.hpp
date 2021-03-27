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
     * @param parent Parent of the widget.
     */
    explicit ImagePreview(QWidget *parent = nullptr);
    /**
     * @brief Create preview panel of the image.
     * @param image Image to preview, must not be null.
     * @param parent parent of the widget.
     * @throws std::invalid_argument if image is null.
     */
    explicit ImagePreview(const QImage *image, QWidget *parent = nullptr);

public: // Setter
    /**
     * @brief Set image to preview.
     * @param image Image to preview, null to take image down.
     */
    void setImage(const QImage *image);

protected:
    /**
     * @brief Resize image when the component get new size.
     * @param evResize Resize event of the component, assigned by Qt.
     */
    void resizeEvent(QResizeEvent *evResize) override;

private:
    /**
     * @brief Resize and apply image to the component with current component size.
     * 
     * @sa setPreviewImage(const QSize &)
     */
    void setPreviewImage();
    /**
     * @brief Resize and apply image to the component.
     * @param szNew Size to scale.
     * 
     * @sa setPreviewImage()
     */
    void setPreviewImage(const QSize &szNew);

private:
    /**
     * @brief Image to preview.
     * 
     * @sa setImage(const QImage *)
     */
    const QImage *image_ { nullptr };
};
}
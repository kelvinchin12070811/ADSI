/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QResizeEvent>

#include <stdexcept>

#include "components/ImagePreview.hpp"

namespace components {
ImagePreview::ImagePreview(QWidget *parent) : QLabel(parent) { }

ImagePreview::ImagePreview(const QImage *image, QWidget *parent) : QLabel(parent), image_ { image }
{
    if (image_ == nullptr)
        throw std::invalid_argument { "image must not be nullptr but it seems to be." };

    setPreviewImage();
}

void ImagePreview::setImage(const QImage *image)
{
    image_ = image;
    if (image_ == nullptr) return;
    setPreviewImage();
}

void ImagePreview::resizeEvent(QResizeEvent *evResize)
{
    if (image_ == nullptr) return;
    setPreviewImage(evResize->size());
}

void ImagePreview::setPreviewImage()
{
    setPreviewImage(this->size());
}

void ImagePreview::setPreviewImage(const QSize &szNew)
{
    auto imgPreview =
            QPixmap::fromImage(image_->scaled(szNew, Qt::AspectRatioMode::KeepAspectRatio,
                                              Qt::TransformationMode::SmoothTransformation));
    this->setPixmap(imgPreview);
}
}
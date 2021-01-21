#include <memory>

#include <QApplication>
#include <QLabel>

int main(int argc, char** argv)
{
    QApplication app{ argc, argv };

    auto label = std::make_unique<QLabel>("Hello world");
    label->resize(640, 480);
    label->show();

    return app.exec();
}
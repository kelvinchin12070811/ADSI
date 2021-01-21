#include <memory>

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

namespace window
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(QMainWindow* parent = nullptr);
    private:
        std::unique_ptr<Ui::MainWindow> ui{ nullptr };
    };
}
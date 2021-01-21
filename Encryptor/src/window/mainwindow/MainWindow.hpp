/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
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
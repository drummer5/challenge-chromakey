#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    // Initialize the app
    QApplication app(argc, argv);

    MainWindow mainWindow;

    // Show the mainWindow and start the event loop
    mainWindow.show();

    // This blocks until the application is closed
    return app.exec();
}
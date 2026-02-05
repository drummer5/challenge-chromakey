#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    // Initialize the Qt application
    QApplication app(argc, argv);

    // Create the mainWindow object
    MainWindow mainWindow;

    // Show the mainWindow and start the event loop
    mainWindow.show();

    // This blocks until the application is closed
    return app.exec();
}
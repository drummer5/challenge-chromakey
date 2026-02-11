#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "VideoProcessor.hpp"

#include <QApplication> // for qApp->quit();
#include <QMainWindow>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // slots for menu items
    void on_menuBrowseForMP4_clicked();
    void on_menuBrowseForPNG_clicked();
    void onQuit();

private:
    void setupUi();
    void createActions();
    void createMenus();

    // this widget manages the pixmap where the virtual background is displayed
    VideoProcessor* vbWidget;

    // menu helps User browse to files
    QMenu* fileMenu = nullptr;
    QAction* onBrowseForMP4 = nullptr;
    QAction* onBrowseForPNG = nullptr;
    QAction* quitApp = nullptr;

    // start browsing for files from application directory
    QString m_MP4File = QApplication::applicationDirPath();
    QString m_PngFile = QApplication::applicationDirPath();
};

#endif // MAINWINDOW_H
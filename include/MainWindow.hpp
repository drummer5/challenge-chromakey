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
    // slots for our menu items
    void on_menuBrowseForMP4_clicked();
    void on_menuBrowseForPNG_clicked();
    void onQuit();

private:
    void setupUi();
    void createActions();
    void createMenus();

    VideoProcessor* vbWidget;

    QAction* onBrowseForMP4 = nullptr;
    QAction* onBrowseForPNG = nullptr;
    QAction* quitApp = nullptr;
    QMenu* fileMenu = nullptr;

    // start browsing for files from application directory
    QString m_MP4File = QApplication::applicationDirPath();
    QString m_PngFile = QApplication::applicationDirPath();
};

#endif // MAINWINDOW_H
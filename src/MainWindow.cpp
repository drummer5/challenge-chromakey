#include "MainWindow.hpp"
#include "VideoProcessor.hpp"
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Virtual Background");
    resize(1000, 1000);
    statusBar()->showMessage("Load an MP4 and PNG background to start", 9000);
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    // setup and connect signals/slots
    createActions();
    createMenus();

    // Create central widget + layout
    vbWidget = new VideoProcessor(this);
    setCentralWidget(vbWidget);
    QVBoxLayout* layout = new QVBoxLayout(vbWidget);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);
}


void MainWindow::createActions()
{
    // define all menu items
    onBrowseForMP4 = new QAction(tr("&Load .mp4 File"), this);
    onBrowseForMP4->setShortcut(tr("Ctrl+O"));
    onBrowseForMP4->setStatusTip(tr("Load .mp4 File"));
    connect(onBrowseForMP4, &QAction::triggered, this, &MainWindow::on_menuBrowseForMP4_clicked);

    onBrowseForPNG = new QAction(tr("&Load .png File for Background"), this);
    onBrowseForPNG->setShortcut(tr("Ctrl+B"));
    onBrowseForPNG->setStatusTip(tr("Load .png File"));
    connect(onBrowseForPNG, &QAction::triggered, this, &MainWindow::on_menuBrowseForPNG_clicked);

    quitApp = new QAction(tr("&Quit"), this);
    quitApp->setShortcut(tr("Ctrl+Q"));
    quitApp->setStatusTip(tr("Exit the application"));
    connect(quitApp, &QAction::triggered, this, &MainWindow::onQuit);
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(onBrowseForMP4);
    fileMenu->addAction(onBrowseForPNG);
    fileMenu->addSeparator();
    fileMenu->addAction(quitApp);
}


void MainWindow::on_menuBrowseForMP4_clicked()
{
    // slot for loading .mp4 video file
    QString fileName = QFileDialog::getOpenFileName(
        this,                                   // parent widget
        tr("Open .mp4 File"),                   // title
        m_MP4File.isEmpty() ? QDir::homePath() : QFileInfo(m_MP4File).absolutePath(),  // start here
        tr("mp4 Files (*.mp4);;All Files (*)")
    );

    if (!(fileName.isEmpty()))
    {
        m_MP4File = fileName;
        qDebug() << "Selected file:" << fileName;

        if (vbWidget->loadVideo(m_MP4File) == false)
        {
            QMessageBox::warning(this, "Error", "Could not open file");
        }
    }
}


void MainWindow::on_menuBrowseForPNG_clicked()
{
    // slot for loading .png background file
    QString fileName = QFileDialog::getOpenFileName(
        this,                                   // parent widget
        tr("Open .png File"),                   // title
        m_PngFile.isEmpty() ? QDir::homePath() : QFileInfo(m_PngFile).absolutePath(),
        tr("png Files (*.png);;All Files (*)")
    );

    if (!(fileName.isEmpty()))
    {
        m_PngFile = fileName;
        qDebug() << "Selected file:" << fileName;

        if (vbWidget->loadBackground(m_PngFile) == false)
        {
            QMessageBox::warning(this, "Error", "Could not open file");
        }
    }
    // show User the path + name of the current background file
    setWindowTitle(m_PngFile);
}


void MainWindow::onQuit()
{
    // slot for quitting app
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Quit", "Are you sure you want to quit?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qApp->quit();
    }
}
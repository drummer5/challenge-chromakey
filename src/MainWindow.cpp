#include "MainWindow.hpp"
#include "VideoProcessor.hpp"
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("My Virtual Chromakey Application");
    resize(1000, 1000);
    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    createActions();
    createMenus();

    // Create central widget + layout
    vbWidget = new VideoProcessor(this);
    setCentralWidget(vbWidget);
    //QWidget* centralWidget = new QWidget(this);
    //setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(vbWidget);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    // Example content
    //centralLabel = new QLabel("My Virtual Chromakey Application", centralWidget);
    //centralLabel->setAlignment(Qt::AlignCenter);
    //centralLabel->setStyleSheet(
    //    "font-size: 36px;"
    //    "font-weight: bold;"
    //    "color: #1e40af;"
    //    "background-color: #f3f4f6;"
    //    "padding: 20px;"
    //    "border-radius: 12px;"
    //);

    //layout->addWidget(centralLabel);

    // statusBar()->showMessage("Ready", 5000);
}


void MainWindow::createActions()
{
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
    fileMenu = menuBar()->addMenu(tr("&Files"));
    fileMenu->addAction(onBrowseForMP4);
    fileMenu->addAction(onBrowseForPNG);
    fileMenu->addSeparator();
    fileMenu->addAction(quitApp);
}


void MainWindow::on_menuBrowseForMP4_clicked()
{
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
    // QMessageBox::information(this, "File Opened", "Selected:\n" + fileName);

    vbWidget->startProcessing();
}

/*
// In mainwindow.cpp constructor (example)
VideoProcessor *vbWidget = new VideoProcessor(this);
setCentralWidget(vbWidget);

// In your onOpenFile slots (adapt as needed)
void MainWindow::onOpenMP4() {
    QString mp4Path = QFileDialog::getOpenFileName(this, "Open MP4 Video", "", "MP4 Files (*.mp4)");
    if (!mp4Path.isEmpty()) {
        vbWidget->loadVideo(mp4Path);
    }
}

void MainWindow::onOpenPNG() {
    QString pngPath = QFileDialog::getOpenFileName(this, "Open PNG Background", "", "PNG Files (*.png)");
    if (!pngPath.isEmpty()) {
        vbWidget->loadBackground(pngPath);
    }
}

// Add a menu item or button to start
void MainWindow::onStartProcessing() {
    vbWidget->startProcessing();
}
*/

void MainWindow::onQuit()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Quit", "Are you sure you want to quit?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qApp->quit();
    }
}
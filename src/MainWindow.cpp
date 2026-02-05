#include "MainWindow.hpp"
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
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    // Example content
    centralLabel = new QLabel("My Virtual Chromakey Application", centralWidget);
    centralLabel->setAlignment(Qt::AlignCenter);
    centralLabel->setStyleSheet(
        "font-size: 36px;"
        "font-weight: bold;"
        "color: #1e40af;"
        "background-color: #f3f4f6;"
        "padding: 20px;"
        "border-radius: 12px;"
    );

    layout->addWidget(centralLabel);

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
        m_lastOpenedFile.isEmpty() ? QDir::homePath() : QFileInfo(m_lastOpenedFile).absolutePath(),  // start here
        tr("mp4 Files (*.mp4);;All Files (*)")
    );

    if (fileName.isEmpty()) {
        // User canceled → do nothing
        return;
    }

    // Remember for next time (optional but user-friendly)
    m_lastOpenedFile = fileName;

    // Now do something with the file (examples):
    qDebug() << "Selected file:" << fileName;

    // Option 1: Show a message
    // QMessageBox::information(this, "File Opened", "Selected:\n" + fileName);

    // Option 2: Load/read the file contents (example)
    // QFile file(fileName);
    // if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QTextStream in(&file);
    //     QString content = in.readAll();
    //     // e.g. put into a QTextEdit if you have one: ui->textEdit->setPlainText(content);
    //     file.close();
    // } else {
    //     QMessageBox::warning(this, "Error", "Could not open file");
    // }
}


void MainWindow::on_menuBrowseForPNG_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,                                   // parent widget
        tr("Open File"),                        // title
        m_lastOpenedFile.isEmpty() ? QDir::homePath() : QFileInfo(m_lastOpenedFile).absolutePath(),  // start here
        tr("png Files (*.png);;All Files (*)")
    );

    if (fileName.isEmpty()) {
        // User canceled → do nothing
        return;
    }

    // Remember for next time (optional but user-friendly)
    m_lastOpenedFile = fileName;

    // Now do something with the file (examples):
    qDebug() << "Selected file:" << fileName;

    // Option 1: Show a message
    // QMessageBox::information(this, "File Opened", "Selected:\n" + fileName);

    // Option 2: Load/read the file contents (example)
    // QFile file(fileName);
    // if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     QTextStream in(&file);
    //     QString content = in.readAll();
    //     // e.g. put into a QTextEdit if you have one: ui->textEdit->setPlainText(content);
    //     file.close();
    // } else {
    //     QMessageBox::warning(this, "Error", "Could not open file");
    // }


/*
QString fileName = QFileDialog::getOpenFileName(
    this,                                   // parent
    tr("Open Document"),                    // title
    QDir::homePath(),                       // starting directory
    tr("Text Files (*.txt *.md *.csv);;All Files (*)")   // filter
);

if (fileName.isEmpty()) {
    return;  // user canceled
}

m_currentFilePath = fileName;
ui->lineEditFilePath->setText(fileName);

// Optional: show message or load the file
// QMessageBox::information(this, "Selected", "File: " + fileName);
// QFile file(fileName); if (file.open(...)) { ... }
*/
}


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
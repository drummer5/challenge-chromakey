#include "VideoProcessor.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>


VideoProcessor::VideoProcessor(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_displayLabel = new QLabel(this);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setScaledContents(true);  // match label size
    layout->addWidget(m_displayLabel);

    // Control bar for buttons - Play  Pause  Restart
    QWidget* controlBar = new QWidget(this);
    QHBoxLayout* controlLayout = new QHBoxLayout(controlBar);
    controlLayout->setContentsMargins(10, 8, 10, 8);
    controlLayout->setSpacing(8);

    m_btnPlay = new QPushButton("Play", this);
    m_btnPause = new QPushButton("Pause", this);
    m_btnRestart = new QPushButton("Restart", this);

    m_btnPlay->setFixedWidth(90);
    m_btnPause->setFixedWidth(90);
    m_btnRestart->setFixedWidth(90);

    controlLayout->addStretch();
    controlLayout->addWidget(m_btnPlay);
    controlLayout->addWidget(m_btnPause);
    controlLayout->addWidget(m_btnRestart);
    controlLayout->addStretch();

    layout->addWidget(controlBar);

    // TODO replace Timer: worker QObject, start a new QThread, use std::chrono
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &VideoProcessor::processFrame);

    // Button connections for signals/slots
    connect(m_btnPlay, &QPushButton::clicked, this, &VideoProcessor::play);
    connect(m_btnPause, &QPushButton::clicked, this, &VideoProcessor::pause);
    connect(m_btnRestart, &QPushButton::clicked, this, &VideoProcessor::restart);

    // Initial state
    m_btnPause->setEnabled(false);
    setMouseTracking(true);
    m_videoLoadedOk = false;
    m_backgroundLoadedOk = false;
}

VideoProcessor::~VideoProcessor()
{
    m_timer->stop();

    if (m_vidCapture.isOpened()) {
        m_vidCapture.release();
    }
}


void VideoProcessor::processFrame()
{
    if (!m_vidCapture.isOpened()) return;

    cv::Mat frame;
    m_vidCapture >> frame;

    if (frame.empty()) {
        // TODO? Loop the video
        m_vidCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
        m_vidCapture >> frame;

        if (frame.empty()) {
            pause();
            return;
        }
    }

    // Resize background to match frame size
    cv::Mat bg_resized;
    cv::resize(m_background, bg_resized, frame.size(), 0, 0, cv::INTER_LINEAR);

    // Convert frame to HSV
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Create mask for the specific key-color
    cv::Mat mask;
    cv::inRange(hsv, m_lowerColor, m_upperColor, mask);

    // Inverse mask
    cv::Mat mask_inv;
    cv::bitwise_not(mask, mask_inv);

    // Create result: start with background
    cv::Mat result = bg_resized.clone();

    // Copy non-key-color parts from frame to result
    frame.copyTo(result, mask_inv);

    // Convert to RGB for QImage
    cv::cvtColor(result, result, cv::COLOR_BGR2RGB);

    // Create QImage and display
    QImage qimg(result.data, result.cols, result.rows, static_cast<int>(result.step), QImage::Format_RGB888);
    m_displayLabel->setPixmap(QPixmap::fromImage(qimg));

    // Ensure label resizes with widget
    m_displayLabel->resize(this->size());
}


bool VideoProcessor::loadVideo(const QString& mp4Path)
{
    m_vidCapture.open(mp4Path.toStdString());

    if (!m_vidCapture.isOpened()) {
        qDebug() << "Failed to load video:" << mp4Path;
        return false;
    }
    m_videoLoadedOk = true;
    return true;
}


bool VideoProcessor::loadBackground(const QString& pngPath)
{
    m_background = cv::imread(pngPath.toStdString(), cv::IMREAD_COLOR);

    if (m_background.empty()) {
        qDebug() << "Failed to load background:" << pngPath;
        return false;
    }
    m_backgroundLoadedOk = true;

    // Reset
    m_isPlaying = false;
    m_timer->stop();
    m_btnPlay->setEnabled(true);
    m_btnPause->setEnabled(false);

    return true;
}


void VideoProcessor::play()
{
    if (checkIfFilesAreLoaded())
    {
        if (!m_vidCapture.isOpened()) return;

        if (!m_isPlaying) {
            double fps = m_vidCapture.get(cv::CAP_PROP_FPS);
            int interval = (fps > 0) ? static_cast<int>(1000.0 / fps) : 33;

            m_timer->start(interval);
            m_isPlaying = true;

            m_btnPlay->setEnabled(false);
            m_btnPause->setEnabled(true);
        }
    }
}

void VideoProcessor::pause()
{
    if (checkIfFilesAreLoaded())
    {
        m_timer->stop();
        m_isPlaying = false;

        m_btnPlay->setEnabled(true);
        m_btnPause->setEnabled(false);
    }
}

void VideoProcessor::restart()
{
    if (checkIfFilesAreLoaded())
    {
        if (!m_vidCapture.isOpened()) return;

        m_vidCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
        pause();
        play();
    }
}

bool VideoProcessor::checkIfFilesAreLoaded()
{
    if (!m_videoLoadedOk)
    {
        QMessageBox::warning(this, "Error", "mp4 file has not been loaded");
        return false;
    }

    if (!m_backgroundLoadedOk)
    {
        QMessageBox::warning(this, "Error", "png file has not been loaded");
        return false;
    }
    return true;
}


// TODO mouseMoveEvent not called when the cursor is over the pixmap
void VideoProcessor::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_originalFrame.empty() && m_displayLabel->pixmap())
    {
        // mouse position on label
        QPoint pos = m_displayLabel->mapFrom(this, event->pos());

        // Get displayed pixmap size and original image size
        QSize dispSize = m_displayLabel->pixmap(Qt::ReturnByValue).size();
        QSize origSize = QSize(m_originalFrame.cols, m_originalFrame.rows);

        // Calculate scaling factors
        float scaleX = static_cast<float>(origSize.width()) / dispSize.width();
        float scaleY = static_cast<float>(origSize.height()) / dispSize.height();

        // Map to original image coordinates
        int imgX = qRound(pos.x() * scaleX);
        int imgY = qRound(pos.y() * scaleY);

        // Clamp to valid range
        imgX = qBound(0, imgX, origSize.width() - 1);
        imgY = qBound(0, imgY, origSize.height() - 1);

        updateStatusBarWithHSV(imgX, imgY);
    }
    qDebug() << "mouse-ing around"; // TODO delete
    QWidget::mouseMoveEvent(event);
}

// TODO fix mouseMoveEvent problem and then this will get called
void VideoProcessor::updateStatusBarWithHSV(int x, int y)
{
    if (m_originalFrame.empty()) return;

    cv::Vec3b pixel = m_originalFrame.at<cv::Vec3b>(y, x); // BGR order
    cv::Mat hsv;
    cv::cvtColor(m_originalFrame, hsv, cv::COLOR_BGR2HSV);
    cv::Vec3b hsvPixel = hsv.at<cv::Vec3b>(y, x);

    QString msg = QString("Pixel (%1, %2) | BGR: (%3, %4, %5) | HSV: H=%6 S=%7 V=%8")
        .arg(x).arg(y)
        .arg(pixel[2]).arg(pixel[1]).arg(pixel[0])  // OpenCV BGR → show as RGB-like
        .arg(hsvPixel[0]).arg(hsvPixel[1]).arg(hsvPixel[2]);

    // Assuming MainWindow has statusBar()
    if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(window())) {
        mainWin->statusBar()->showMessage(msg);
    }
    else {
        qDebug() << msg;
    }
}
#include "VideoProcessor.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>


VideoProcessor::VideoProcessor(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_displayLabel = new QLabel(this);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setScaledContents(true);  // Scale pixmap to label size
    layout->addWidget(m_displayLabel);

    // Control bar
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

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &VideoProcessor::processFrame);

    // Button connections
    connect(m_btnPlay, &QPushButton::clicked, this, &VideoProcessor::play);
    connect(m_btnPause, &QPushButton::clicked, this, &VideoProcessor::pause);
    connect(m_btnRestart, &QPushButton::clicked, this, &VideoProcessor::restart);

    // Initial state
    m_btnPause->setEnabled(false);
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
        // Loop the video
        m_vidCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
        m_vidCapture >> frame;
        if (frame.empty()) {
            // m_timer->stop();
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

    // Create mask for the specific color (green)
    cv::Mat mask;
    cv::inRange(hsv, m_lowerColor, m_upperColor, mask);

    // Inverse mask (non-green areas)
    cv::Mat mask_inv;
    cv::bitwise_not(mask, mask_inv);

    // Create result: start with background
    cv::Mat result = bg_resized.clone();

    // Copy non-green parts from frame to result
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
        qDebug() << "Failed to open video:" << mp4Path;
        return false;
    }
    return true;
}

bool VideoProcessor::loadBackground(const QString& pngPath)
{
    m_background = cv::imread(pngPath.toStdString(), cv::IMREAD_COLOR);
    if (m_background.empty()) {
        qDebug() << "Failed to load background:" << pngPath;
        return false;
    }

    // Reset
    m_isPlaying = false;
    m_timer->stop();
    m_btnPlay->setEnabled(true);
    m_btnPause->setEnabled(false);

    return true;
}


void VideoProcessor::play()
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

void VideoProcessor::pause()
{
    m_timer->stop();
    m_isPlaying = false;

    m_btnPlay->setEnabled(true);
    m_btnPause->setEnabled(false);
}

void VideoProcessor::restart()
{
    if (!m_vidCapture.isOpened()) return;

    m_vidCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
    pause();               // stop if was playing
    play();                // immediately start again
}

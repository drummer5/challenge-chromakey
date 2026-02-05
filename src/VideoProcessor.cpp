#include "VideoProcessor.hpp"
#include <QVBoxLayout>
#include <QDebug>


VideoProcessor::VideoProcessor(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_displayLabel = new QLabel(this);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setScaledContents(true);  // Scale pixmap to label size
    layout->addWidget(m_displayLabel);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &VideoProcessor::processFrame);
}

VideoProcessor::~VideoProcessor()
{
    m_timer->stop();

    if (m_cap.isOpened()) {
        m_cap.release();
    }
}


void VideoProcessor::startProcessing()
{
    if (!m_cap.isOpened() || m_background.empty()) {
        qDebug() << "Video or background not loaded";
        return;
    }

    // Set timer interval based on video FPS (or default 30ms ~33fps)
    double fps = m_cap.get(cv::CAP_PROP_FPS);
    int interval = (fps > 0) ? static_cast<int>(1000 / fps) : 30;
    m_timer->start(interval);
}

void VideoProcessor::processFrame()
{
    if (!m_cap.isOpened()) return;

    cv::Mat frame;
    m_cap >> frame;
    if (frame.empty()) {
        // Loop the video
        m_cap.set(cv::CAP_PROP_POS_FRAMES, 0);
        m_cap >> frame;
        if (frame.empty()) {
            m_timer->stop();
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
    m_cap.open(mp4Path.toStdString());
    if (!m_cap.isOpened()) {
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
    return true;
}

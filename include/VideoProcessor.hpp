// VideoProcessor.h (updated for chroma key with OpenCV)

#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>  // Include OpenCV headers
#include <QWidget>
#include <QLabel>
#include <QTimer>

class VideoProcessor : public QWidget
{
    Q_OBJECT

public:
    explicit VideoProcessor(QWidget* parent = nullptr);
    ~VideoProcessor();

    bool loadVideo(const QString& mp4Path);
    bool loadBackground(const QString& pngPath);

public slots:
    void startProcessing();  // Call this after loading files to begin

private slots:
    void processFrame();

private:
    cv::VideoCapture m_cap;
    cv::Mat m_background;

    QLabel* m_displayLabel;
    QTimer* m_timer;

    // Chroma key parameters (hardcoded for green; adjust as needed)
    cv::Scalar m_lowerColor = cv::Scalar(35, 50, 50);   // HSV lower bound for green
    cv::Scalar m_upperColor = cv::Scalar(85, 255, 255); // HSV upper bound
};

#endif // VIDEOPROCESSOR_H
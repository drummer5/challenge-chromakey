#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
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
    void play();
    void pause();
    void restart();

private slots:
    void processFrame();

private:
    cv::VideoCapture m_vidCapture;
    cv::Mat m_background;

    QLabel* m_displayLabel;
    QPushButton* m_btnPlay = nullptr;
    QPushButton* m_btnPause = nullptr;
    QPushButton* m_btnRestart = nullptr;

    QTimer* m_timer = nullptr;

    bool m_isPlaying = false;

    // Chroma key parameters, hardcoded for green
    cv::Scalar m_lowerColor = cv::Scalar(35, 50, 50);   // HSV lower bound for green
    cv::Scalar m_upperColor = cv::Scalar(85, 255, 255); // HSV upper bound
};

#endif // VIDEOPROCESSOR_H
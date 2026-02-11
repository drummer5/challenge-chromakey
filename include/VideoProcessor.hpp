#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
// use MainWindow status bar  TODO fix mouse bug in order to update StatusBar
#include <QMainWindow>
#include <QStatusBar>

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

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

private slots:
    void processFrame();

private:
    cv::VideoCapture m_vidCapture;
    cv::Mat m_background;
    cv::Mat m_originalFrame; // before muxing, this lets User choose key color

    QLabel* m_displayLabel;
    QPushButton* m_btnPlay = nullptr;
    QPushButton* m_btnPause = nullptr;
    QPushButton* m_btnRestart = nullptr;

    QTimer* m_timer = nullptr; // TODO replace with more accurate worker Thread

    bool m_isPlaying = false;

    // Chroma key parameters, hardcoded for green
    cv::Scalar m_lowerColor = cv::Scalar(35, 50, 50);   // HSV lower bound
    cv::Scalar m_upperColor = cv::Scalar(85, 255, 255); // HSV upper bound

    // necessary to prevent crashes when clicking Play Pause Restart buttons
    bool m_videoLoadedOk;
    bool m_backgroundLoadedOk;
    bool checkIfFilesAreLoaded();

    void updateStatusBarWithHSV(int x, int y); // shown on parent
};

#endif // VIDEOPROCESSOR_H
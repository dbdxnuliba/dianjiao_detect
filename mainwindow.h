#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>

#include "boardctrl.h"
#include "thresholdsetting.h"
#include "fileReadThread.h"
#include "dialogtemplatesetting.h"
#include "ui_mainwindow.h"
#include "DetectResLog.h"

using namespace cv;
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void image_disp(const cv::Mat & src, QImage::Format format);

    void on_pushButton_src_clicked();
    void setting_dialog_update();

    void on_pushButton_clear_clicked();
    void on_pushButton_stop_clicked();
    void recv_file_path(QString & file_path, QString & num_str);
    void template_para_changed();

signals:
    void run_file_search_signal();
    void stop_file_search_signal();
private:
    void action_init();
    void widget_init();
    void board_A_area_disp(const Board_A_detec::Board_A_area_item & area, unsigned int res);
    void board_B_area_disp(const Board_B_detec::Board_B_area_item & area, unsigned int res);
    void board_detect(QString & num_str);
    void board_detec_auto();
    void startObjThread();
    void error_info_with_err_detect();
    void clear_detec_res();
private:
    Ui::MainWindow *ui;
    thresholdSetting * m_settingsDialog = nullptr;
    DialogTemplateSetting * m_templateSet = nullptr;
    BoardCtrl m_board_ctrl;
    DetectResLog m_log;

    FileReadThread * m_file_read_thread;
    QThread * m_objThread;

    int is_gray = 0;
    Mat src_image;
    Mat gray_image;

    int m_board_info;
    unsigned m_board_res;

    bool m_template_setting_is_OK;
    QVector<QLineEdit *> m_board_A_line_edit_vec;
    QVector<QLineEdit *> m_board_B_line_edit_vec;
};
#endif // MAINWINDOW_H

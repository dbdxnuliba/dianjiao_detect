#ifndef THRESHOLDSETTING_H
#define THRESHOLDSETTING_H

#include <QDialog>
#include "boardctrl.h"

namespace Ui {
class thresholdSetting;
}

class thresholdSetting : public QDialog
{
    Q_OBJECT

public:
    explicit thresholdSetting(QWidget *parent = nullptr);
    ~thresholdSetting();
    void get_board_A_para(Board_A_detec::Board_A_area_item & para);
    void get_board_B_para(Board_B_detec::Board_B_area_item & para);

public slots:
    void update_ui();

signals:
    void update_para_signal();

private slots:

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::thresholdSetting *ui;
    Board_A_detec::Board_A_area_item m_board_A_para;
    Board_B_detec::Board_B_area_item m_board_B_para;
};

#endif // THRESHOLDSETTING_H

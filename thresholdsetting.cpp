#include "thresholdsetting.h"
#include "ui_thresholdsetting.h"

const double threshold_JALVC164245_arr[5] = {2500, 2500, 2500, 2500, 2500};
const double threshold_GH137S_arr[5] = {2500, 2500, 2500, 2500, 2500};
const double threshold_JDSPF28335_arr[8] = {9000, 9000, 9000, 9000, 600, 600, 600, 600};
const double threshold_SM1040_arr[2] = {1500, 1500};
const double threshold_J16H281_arr[2] = {1500, 1500};
const double threshold_SM490_arr[2] = {1500, 1500};
const double threshold_100_arr[2] = {1200, 1200};

const double threshold_B_GH137S_arr[5] = {3000, 3000, 3000, 3000, 3000};
const double threshold_B_JT6H281_arr[2] = {3000, 3000};
const double threshold_B_SM1040_arr[2] = {2500, 2500};
const double threshold_B_121_arr[2] = {2000, 2000};
const double threshold_B_100_arr[2] = {2000, 2000};


thresholdSetting::thresholdSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::thresholdSetting)
{
    ui->setupUi(this);

    ui->lineEdit_A1->setText(QString::number(threshold_JALVC164245_arr[0]));
    ui->lineEdit_A2->setText(QString::number(threshold_JALVC164245_arr[1]));
    ui->lineEdit_A3->setText(QString::number(threshold_JALVC164245_arr[2]));
    ui->lineEdit_A4->setText(QString::number(threshold_JALVC164245_arr[3]));
    ui->lineEdit_A5->setText(QString::number(threshold_JALVC164245_arr[4]));
    for (int var = 0; var < 5; ++var) {
        m_board_A_para.threshold_JALVC164245.push_back(threshold_JALVC164245_arr[var]);
    }

    ui->lineEdit_A6->setText(QString::number(threshold_GH137S_arr[0]));
    ui->lineEdit_A7->setText(QString::number(threshold_GH137S_arr[1]));
    ui->lineEdit_A8->setText(QString::number(threshold_GH137S_arr[2]));
    ui->lineEdit_A9->setText(QString::number(threshold_GH137S_arr[3]));
    ui->lineEdit_A10->setText(QString::number(threshold_GH137S_arr[4]));
    for (int var = 0; var < 5; ++var) {
        m_board_A_para.threshold_GH137S.push_back(threshold_GH137S_arr[var]);
    }

    ui->lineEdit_A11->setText(QString::number(threshold_SM1040_arr[0]));
    ui->lineEdit_A12->setText(QString::number(threshold_SM1040_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_A_para.threshold_SM1040.push_back(threshold_SM1040_arr[var]);
    }

    ui->lineEdit_A13->setText(QString::number(threshold_J16H281_arr[0]));
    ui->lineEdit_A14->setText(QString::number(threshold_J16H281_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_A_para.threshold_J16H281.push_back(threshold_J16H281_arr[var]);
    }

    ui->lineEdit_A15->setText(QString::number(threshold_SM490_arr[0]));
    ui->lineEdit_A16->setText(QString::number(threshold_SM490_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_A_para.threshold_SM490.push_back(threshold_SM490_arr[var]);
    }

    ui->lineEdit_A17->setText(QString::number(threshold_100_arr[0]));
    ui->lineEdit_A18->setText(QString::number(threshold_100_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_A_para.threshold_100.push_back(threshold_100_arr[var]);
    }

    ui->lineEdit_A19->setText(QString::number(threshold_JDSPF28335_arr[0]));
    ui->lineEdit_A20->setText(QString::number(threshold_JDSPF28335_arr[1]));
    ui->lineEdit_A21->setText(QString::number(threshold_JDSPF28335_arr[2]));
    ui->lineEdit_A22->setText(QString::number(threshold_JDSPF28335_arr[3]));
    ui->lineEdit_A23->setText(QString::number(threshold_JDSPF28335_arr[4]));
    ui->lineEdit_A24->setText(QString::number(threshold_JDSPF28335_arr[5]));
    ui->lineEdit_A25->setText(QString::number(threshold_JDSPF28335_arr[6]));
    ui->lineEdit_A26->setText(QString::number(threshold_JDSPF28335_arr[7]));
    for (int var = 0; var < 8; ++var) {
        m_board_A_para.threshold_JDSPF28335.push_back(threshold_JDSPF28335_arr[var]);
    }

    ui->lineEdit_B1->setText(QString::number(threshold_B_GH137S_arr[0]));
    ui->lineEdit_B2->setText(QString::number(threshold_B_GH137S_arr[1]));
    ui->lineEdit_B3->setText(QString::number(threshold_B_GH137S_arr[2]));
    ui->lineEdit_B4->setText(QString::number(threshold_B_GH137S_arr[3]));
    ui->lineEdit_B5->setText(QString::number(threshold_B_GH137S_arr[4]));
    for (int var = 0; var < 5; ++var) {
        m_board_B_para.threshold_GH137S.push_back(threshold_B_GH137S_arr[var]);
    }

    ui->lineEdit_B6->setText(QString::number(threshold_B_JT6H281_arr[0]));
    ui->lineEdit_B7->setText(QString::number(threshold_B_JT6H281_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_B_para.threshold_JT6H281.push_back(threshold_B_JT6H281_arr[var]);
    }

    ui->lineEdit_B8->setText(QString::number(threshold_B_SM1040_arr[0]));
    ui->lineEdit_B9->setText(QString::number(threshold_B_SM1040_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_B_para.threshold_SM1040.push_back(threshold_B_SM1040_arr[var]);
    }

    ui->lineEdit_B10->setText(QString::number(threshold_B_121_arr[0]));
    ui->lineEdit_B11->setText(QString::number(threshold_B_121_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_B_para.threshold_121.push_back(threshold_B_121_arr[var]);
    }

    ui->lineEdit_B12->setText(QString::number(threshold_B_100_arr[0]));
    ui->lineEdit_B13->setText(QString::number(threshold_B_100_arr[1]));
    for (int var = 0; var < 2; ++var) {
        m_board_B_para.threshold_100.push_back(threshold_B_100_arr[var]);
    }
}

thresholdSetting::~thresholdSetting()
{
    delete ui;
}

void thresholdSetting::get_board_A_para(Board_A_detec::Board_A_area_item &para)
{
    para = m_board_A_para;
}

void thresholdSetting::get_board_B_para(Board_B_detec::Board_B_area_item &para)
{
    para = m_board_B_para;
}

void thresholdSetting::update_ui()
{
    ui->lineEdit_A1->setText(QString::number(m_board_A_para.threshold_JALVC164245[0]));
    ui->lineEdit_A2->setText(QString::number(m_board_A_para.threshold_JALVC164245[1]));
    ui->lineEdit_A3->setText(QString::number(m_board_A_para.threshold_JALVC164245[2]));
    ui->lineEdit_A4->setText(QString::number(m_board_A_para.threshold_JALVC164245[3]));
    ui->lineEdit_A5->setText(QString::number(m_board_A_para.threshold_JALVC164245[4]));

    ui->lineEdit_A6->setText(QString::number(m_board_A_para.threshold_GH137S[0]));
    ui->lineEdit_A7->setText(QString::number(m_board_A_para.threshold_GH137S[1]));
    ui->lineEdit_A8->setText(QString::number(m_board_A_para.threshold_GH137S[2]));
    ui->lineEdit_A9->setText(QString::number(m_board_A_para.threshold_GH137S[3]));
    ui->lineEdit_A10->setText(QString::number(m_board_A_para.threshold_GH137S[4]));

    ui->lineEdit_A11->setText(QString::number(m_board_A_para.threshold_SM1040[0]));
    ui->lineEdit_A12->setText(QString::number(m_board_A_para.threshold_SM1040[1]));

    ui->lineEdit_A13->setText(QString::number(m_board_A_para.threshold_J16H281[0]));
    ui->lineEdit_A14->setText(QString::number(m_board_A_para.threshold_J16H281[1]));

    ui->lineEdit_A15->setText(QString::number(m_board_A_para.threshold_SM490[0]));
    ui->lineEdit_A16->setText(QString::number(m_board_A_para.threshold_SM490[1]));

    ui->lineEdit_A17->setText(QString::number(m_board_A_para.threshold_100[0]));
    ui->lineEdit_A18->setText(QString::number(m_board_A_para.threshold_100[1]));

    ui->lineEdit_A19->setText(QString::number(m_board_A_para.threshold_JDSPF28335[0]));
    ui->lineEdit_A20->setText(QString::number(m_board_A_para.threshold_JDSPF28335[1]));
    ui->lineEdit_A21->setText(QString::number(m_board_A_para.threshold_JDSPF28335[2]));
    ui->lineEdit_A22->setText(QString::number(m_board_A_para.threshold_JDSPF28335[3]));
    ui->lineEdit_A23->setText(QString::number(m_board_A_para.threshold_JDSPF28335[4]));
    ui->lineEdit_A24->setText(QString::number(m_board_A_para.threshold_JDSPF28335[5]));
    ui->lineEdit_A25->setText(QString::number(m_board_A_para.threshold_JDSPF28335[6]));
    ui->lineEdit_A26->setText(QString::number(m_board_A_para.threshold_JDSPF28335[7]));

    ui->lineEdit_B1->setText(QString::number(m_board_B_para.threshold_GH137S[0]));
    ui->lineEdit_B2->setText(QString::number(m_board_B_para.threshold_GH137S[1]));
    ui->lineEdit_B3->setText(QString::number(m_board_B_para.threshold_GH137S[2]));
    ui->lineEdit_B4->setText(QString::number(m_board_B_para.threshold_GH137S[3]));
    ui->lineEdit_B5->setText(QString::number(m_board_B_para.threshold_GH137S[4]));

    ui->lineEdit_B6->setText(QString::number(m_board_B_para.threshold_JT6H281[0]));
    ui->lineEdit_B7->setText(QString::number(m_board_B_para.threshold_JT6H281[1]));

    ui->lineEdit_B8->setText(QString::number(m_board_B_para.threshold_SM1040[0]));
    ui->lineEdit_B9->setText(QString::number(m_board_B_para.threshold_SM1040[1]));

    ui->lineEdit_B10->setText(QString::number(m_board_B_para.threshold_121[0]));
    ui->lineEdit_B11->setText(QString::number(m_board_B_para.threshold_121[1]));

    ui->lineEdit_B12->setText(QString::number(m_board_B_para.threshold_100[0]));
    ui->lineEdit_B13->setText(QString::number(m_board_B_para.threshold_100[1]));
}


void thresholdSetting::on_pushButton_ok_clicked()
{
    m_board_A_para.threshold_JALVC164245[0] = ui->lineEdit_A1->text().toDouble();
    m_board_A_para.threshold_JALVC164245[1] = ui->lineEdit_A2->text().toDouble();
    m_board_A_para.threshold_JALVC164245[2] = ui->lineEdit_A3->text().toDouble();
    m_board_A_para.threshold_JALVC164245[3] = ui->lineEdit_A4->text().toDouble();
    m_board_A_para.threshold_JALVC164245[4] = ui->lineEdit_A5->text().toDouble();

    m_board_A_para.threshold_GH137S[0] = ui->lineEdit_A6->text().toDouble();
    m_board_A_para.threshold_GH137S[1] = ui->lineEdit_A7->text().toDouble();
    m_board_A_para.threshold_GH137S[2] = ui->lineEdit_A8->text().toDouble();
    m_board_A_para.threshold_GH137S[3] = ui->lineEdit_A9->text().toDouble();
    m_board_A_para.threshold_GH137S[4] = ui->lineEdit_A10->text().toDouble();

    m_board_A_para.threshold_SM1040[0] = ui->lineEdit_A11->text().toDouble();
    m_board_A_para.threshold_SM1040[1] = ui->lineEdit_A12->text().toDouble();

    m_board_A_para.threshold_J16H281[0] = ui->lineEdit_A13->text().toDouble();
    m_board_A_para.threshold_J16H281[1] = ui->lineEdit_A14->text().toDouble();

    m_board_A_para.threshold_SM490[0] = ui->lineEdit_A15->text().toDouble();
    m_board_A_para.threshold_SM490[1] = ui->lineEdit_A16->text().toDouble();

    m_board_A_para.threshold_100[0] = ui->lineEdit_A17->text().toDouble();
    m_board_A_para.threshold_100[1] = ui->lineEdit_A18->text().toDouble();

    m_board_A_para.threshold_JDSPF28335[0] = ui->lineEdit_A19->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[1] = ui->lineEdit_A20->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[2] = ui->lineEdit_A21->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[3] = ui->lineEdit_A22->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[4] = ui->lineEdit_A23->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[5] = ui->lineEdit_A24->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[6] = ui->lineEdit_A25->text().toDouble();
    m_board_A_para.threshold_JDSPF28335[7] = ui->lineEdit_A26->text().toDouble();

    m_board_B_para.threshold_GH137S[0] = ui->lineEdit_B1->text().toDouble();
    m_board_B_para.threshold_GH137S[1] = ui->lineEdit_B2->text().toDouble();
    m_board_B_para.threshold_GH137S[2] = ui->lineEdit_B3->text().toDouble();
    m_board_B_para.threshold_GH137S[3] = ui->lineEdit_B4->text().toDouble();
    m_board_B_para.threshold_GH137S[4] = ui->lineEdit_B5->text().toDouble();

    m_board_B_para.threshold_JT6H281[0] = ui->lineEdit_B6->text().toDouble();
    m_board_B_para.threshold_JT6H281[1] = ui->lineEdit_B7->text().toDouble();

    m_board_B_para.threshold_SM1040[0] = ui->lineEdit_B8->text().toDouble();
    m_board_B_para.threshold_SM1040[1] = ui->lineEdit_B9->text().toDouble();

    m_board_B_para.threshold_121[0] = ui->lineEdit_B10->text().toDouble();
    m_board_B_para.threshold_121[1] = ui->lineEdit_B11->text().toDouble();

    m_board_B_para.threshold_100[0] = ui->lineEdit_B12->text().toDouble();
    m_board_B_para.threshold_100[1] = ui->lineEdit_B13->text().toDouble();

    emit(update_para_signal());

    hide();
}

void thresholdSetting::on_pushButton_cancel_clicked()
{
    ui->lineEdit_A1->setText(QString::number(m_board_A_para.threshold_JALVC164245[0]));
    ui->lineEdit_A2->setText(QString::number(m_board_A_para.threshold_JALVC164245[1]));
    ui->lineEdit_A3->setText(QString::number(m_board_A_para.threshold_JALVC164245[2]));
    ui->lineEdit_A4->setText(QString::number(m_board_A_para.threshold_JALVC164245[3]));
    ui->lineEdit_A5->setText(QString::number(m_board_A_para.threshold_JALVC164245[4]));

    ui->lineEdit_A6->setText(QString::number(m_board_A_para.threshold_GH137S[0]));
    ui->lineEdit_A7->setText(QString::number(m_board_A_para.threshold_GH137S[1]));
    ui->lineEdit_A8->setText(QString::number(m_board_A_para.threshold_GH137S[2]));
    ui->lineEdit_A9->setText(QString::number(m_board_A_para.threshold_GH137S[3]));
    ui->lineEdit_A10->setText(QString::number(m_board_A_para.threshold_GH137S[4]));

    ui->lineEdit_A11->setText(QString::number(m_board_A_para.threshold_SM1040[0]));
    ui->lineEdit_A12->setText(QString::number(m_board_A_para.threshold_SM1040[1]));

    ui->lineEdit_A13->setText(QString::number(m_board_A_para.threshold_J16H281[0]));
    ui->lineEdit_A14->setText(QString::number(m_board_A_para.threshold_J16H281[1]));

    ui->lineEdit_A15->setText(QString::number(m_board_A_para.threshold_SM490[0]));
    ui->lineEdit_A16->setText(QString::number(m_board_A_para.threshold_SM490[1]));

    ui->lineEdit_A17->setText(QString::number(m_board_A_para.threshold_100[0]));
    ui->lineEdit_A18->setText(QString::number(m_board_A_para.threshold_100[1]));

    ui->lineEdit_A19->setText(QString::number(m_board_A_para.threshold_JDSPF28335[0]));
    ui->lineEdit_A20->setText(QString::number(m_board_A_para.threshold_JDSPF28335[1]));
    ui->lineEdit_A21->setText(QString::number(m_board_A_para.threshold_JDSPF28335[2]));
    ui->lineEdit_A22->setText(QString::number(m_board_A_para.threshold_JDSPF28335[3]));
    ui->lineEdit_A23->setText(QString::number(m_board_A_para.threshold_JDSPF28335[4]));
    ui->lineEdit_A24->setText(QString::number(m_board_A_para.threshold_JDSPF28335[5]));
    ui->lineEdit_A25->setText(QString::number(m_board_A_para.threshold_JDSPF28335[6]));
    ui->lineEdit_A26->setText(QString::number(m_board_A_para.threshold_JDSPF28335[7]));

    ui->lineEdit_B1->setText(QString::number(m_board_B_para.threshold_GH137S[0]));
    ui->lineEdit_B2->setText(QString::number(m_board_B_para.threshold_GH137S[1]));
    ui->lineEdit_B3->setText(QString::number(m_board_B_para.threshold_GH137S[2]));
    ui->lineEdit_B4->setText(QString::number(m_board_B_para.threshold_GH137S[3]));
    ui->lineEdit_B5->setText(QString::number(m_board_B_para.threshold_GH137S[4]));

    ui->lineEdit_B6->setText(QString::number(m_board_B_para.threshold_JT6H281[0]));
    ui->lineEdit_B7->setText(QString::number(m_board_B_para.threshold_JT6H281[1]));

    ui->lineEdit_B8->setText(QString::number(m_board_B_para.threshold_SM1040[0]));
    ui->lineEdit_B9->setText(QString::number(m_board_B_para.threshold_SM1040[1]));

    ui->lineEdit_B10->setText(QString::number(m_board_B_para.threshold_121[0]));
    ui->lineEdit_B11->setText(QString::number(m_board_B_para.threshold_121[1]));

    ui->lineEdit_B12->setText(QString::number(m_board_B_para.threshold_100[0]));
    ui->lineEdit_B13->setText(QString::number(m_board_B_para.threshold_100[1]));
    hide();
}

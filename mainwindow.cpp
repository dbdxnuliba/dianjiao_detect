#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QFileDialog"
#include <QVector>

#include <QDebug>
#include <QMessageBox>


static unsigned int board_NG_cnt;      //OK
static unsigned int board_OK_cnt;      //NG
static unsigned int board_ERR_cnt;     //无法识别


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_objThread = nullptr;
    m_file_read_thread = nullptr;
    m_template_setting_is_OK = false;
    m_settingsDialog = new thresholdSetting(this);
    m_templateSet = new DialogTemplateSetting(this);
    action_init();
    widget_init();

    //连接信号槽，当参数设置有变化时及时更新阈值
    connect(m_settingsDialog, &thresholdSetting::update_para_signal, this, &MainWindow::setting_dialog_update);
    connect(m_templateSet, &DialogTemplateSetting::update_para_signal, this, &MainWindow::template_para_changed);

    setting_dialog_update();    //使用默认阈值更新目标板检测阈值
    //template_para_changed();    //主动触发一次模板路径更新的槽函数，使用系统默认的路径更新模板

    startObjThread();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_settingsDialog;
    if(m_objThread){
        m_objThread->quit();
    }
    m_objThread->wait();
}


/**
 * @brief MainWindow::on_pushButton_clicked
 * //开始信号，收到该信号后系统开始进行检测
 */
void MainWindow::on_pushButton_clicked()
{
    QString board_num;
    Mat image_src;

    //检查文件模板是否正常设置，否则不进行后续操作
    if (false == m_template_setting_is_OK){
        QMessageBox::warning(this, "警告", "请设置模板图片路径后再进行检测，否则系统会崩溃");
        return;
    }

    if (ui->lineEdit_srcPath->text().isEmpty()){
        QMessageBox::warning(this, "警告", "请设置待检文件或者文件夹路径");
        return;
    }


    clear_detec_res();//清除上一轮检测数据，不包括统计数据

    //------------------------------根据检测模式来确定后续流程--------------------------------
    //０手动模式，１自动模式，２自检模式
    if (0 == ui->comboBox->currentIndex()){
        QString fileName = ui->lineEdit_srcPath->text();

        //------------------根据文件名更新编号---------------------
        QFileInfo file_info(fileName);
        QString num_str = file_info.fileName();
        num_str.resize(9);
        //---------------------------------------

        cv::Mat img_src = cv::imread(fileName.toStdString());
        m_board_ctrl.set_image_src(img_src);

        board_detect(num_str);
    }
    else{
        if (ui->lineEdit_srcPath_2->text().isEmpty()){
            QMessageBox::warning(this, "警告", "请设置LOG文件路径");
            return;
        }

        QString fileName = ui->lineEdit_srcPath_2->text();
        if (0 > m_log.set_file_path(fileName)){
            QMessageBox::warning(this, "警告", "LOG文件创建或打开失败，请检查");
            return;
        }

        QDir dir(ui->lineEdit_srcPath->text());
        m_file_read_thread->set_src_dir(dir);
        emit run_file_search_signal();
    }
    return;
}

/**
 * @brief MainWindow::image_disp
 * @param src: source image
 * @param label:
 * @param format
 */
void MainWindow::image_disp(const cv::Mat & src, QImage::Format format)
{
    const int label_width = 800;
    const int label_height = 600;
    Mat image_src = Mat(Size(label_width, label_height), CV_32FC3);
    cv::resize(src, image_src, Size(label_width, label_height));
    QImage Qtemp = QImage((const unsigned char*)(image_src.data), image_src.cols, image_src.rows, image_src.step, format);
    ui->label->clear();
    ui->label->setPixmap(QPixmap::fromImage(Qtemp));
    ui->label->show();
    this->repaint();
}

/**
 * @brief MainWindow::action_init
 * //初始化与action有关的控件
 */
void MainWindow::action_init()
{
    connect(ui->action_threshold, &QAction::triggered, m_settingsDialog, &thresholdSetting::show);
    connect(ui->action_threshold, &QAction::triggered, m_settingsDialog, &thresholdSetting::update_ui);
    connect(ui->action_temp_para, &QAction::triggered, m_templateSet, &thresholdSetting::show);
}

/**
 * @brief MainWindow::widget_init
 * //主界面上控件的初始化
 */
void MainWindow::widget_init()
{
    ui->lineEdit_srcPath->setDisabled(true);
    ui->lineEdit_detec_board->setDisabled(true);
    ui->lineEdit_num->setDisabled(true);
    ui->lineEdit_res->setDisabled(true);
    ui->lineEdit_OK_ratio->setDisabled(true);
    ui->lineEdit_detec_err_num->setDisabled(true);
    ui->lineEdit_detec_num->setDisabled(true);

    ui->label_disp_state->setText("");
    ui->label_disp_state_2->setText("");

    m_board_A_line_edit_vec.clear();
    m_board_A_line_edit_vec.append(ui->lineEdit_A1);
    m_board_A_line_edit_vec.append(ui->lineEdit_A2);
    m_board_A_line_edit_vec.append(ui->lineEdit_A3);
    m_board_A_line_edit_vec.append(ui->lineEdit_A4);
    m_board_A_line_edit_vec.append(ui->lineEdit_A5);
    m_board_A_line_edit_vec.append(ui->lineEdit_A6);
    m_board_A_line_edit_vec.append(ui->lineEdit_A7);
    m_board_A_line_edit_vec.append(ui->lineEdit_A8);
    m_board_A_line_edit_vec.append(ui->lineEdit_A9);
    m_board_A_line_edit_vec.append(ui->lineEdit_A10);
    m_board_A_line_edit_vec.append(ui->lineEdit_A11);
    m_board_A_line_edit_vec.append(ui->lineEdit_A12);
    m_board_A_line_edit_vec.append(ui->lineEdit_A13);
    m_board_A_line_edit_vec.append(ui->lineEdit_A14);
    m_board_A_line_edit_vec.append(ui->lineEdit_A15);
    m_board_A_line_edit_vec.append(ui->lineEdit_A16);
    m_board_A_line_edit_vec.append(ui->lineEdit_A17);
    m_board_A_line_edit_vec.append(ui->lineEdit_A18);
    m_board_A_line_edit_vec.append(ui->lineEdit_A19);
    m_board_A_line_edit_vec.append(ui->lineEdit_A20);
    m_board_A_line_edit_vec.append(ui->lineEdit_A21);
    m_board_A_line_edit_vec.append(ui->lineEdit_A22);
    m_board_A_line_edit_vec.append(ui->lineEdit_A23);
    m_board_A_line_edit_vec.append(ui->lineEdit_A24);
    m_board_A_line_edit_vec.append(ui->lineEdit_A25);
    m_board_A_line_edit_vec.append(ui->lineEdit_A26);

    m_board_B_line_edit_vec.clear();
    m_board_B_line_edit_vec.append(ui->lineEdit_B1);
    m_board_B_line_edit_vec.append(ui->lineEdit_B2);
    m_board_B_line_edit_vec.append(ui->lineEdit_B3);
    m_board_B_line_edit_vec.append(ui->lineEdit_B4);
    m_board_B_line_edit_vec.append(ui->lineEdit_B5);
    m_board_B_line_edit_vec.append(ui->lineEdit_B6);
    m_board_B_line_edit_vec.append(ui->lineEdit_B7);
    m_board_B_line_edit_vec.append(ui->lineEdit_B8);
    m_board_B_line_edit_vec.append(ui->lineEdit_B9);
    m_board_B_line_edit_vec.append(ui->lineEdit_B10);
    m_board_B_line_edit_vec.append(ui->lineEdit_B11);
    m_board_B_line_edit_vec.append(ui->lineEdit_B12);
    m_board_B_line_edit_vec.append(ui->lineEdit_B13);

    for (int var = 0; var < m_board_A_line_edit_vec.size(); ++var){
        m_board_A_line_edit_vec.at(var)->setDisabled(true);
    }
    for (int var = 0; var < m_board_B_line_edit_vec.size(); ++var){
        m_board_B_line_edit_vec.at(var)->setDisabled(true);
    }
}

void MainWindow::board_A_area_disp(const Board_A_detec::Board_A_area_item &area, unsigned int res)
{
    ui->lineEdit_A1->setText(QString::number(area.threshold_JALVC164245.at(0)));
    if (res & (1 << 0)) ui->lineEdit_A1->setStyleSheet("background-color:red");
    else ui->lineEdit_A1->setStyleSheet("background-color:green");
    ui->lineEdit_A2->setText(QString::number(area.threshold_JALVC164245.at(1)));
    if (res & (1 << 1)) ui->lineEdit_A2->setStyleSheet("background-color:red");
    else ui->lineEdit_A2->setStyleSheet("background-color:green");
    ui->lineEdit_A3->setText(QString::number(area.threshold_JALVC164245.at(2)));
    if (res & (1 << 2)) ui->lineEdit_A3->setStyleSheet("background-color:red");
    else ui->lineEdit_A3->setStyleSheet("background-color:green");
    ui->lineEdit_A4->setText(QString::number(area.threshold_JALVC164245.at(3)));
    if (res & (1 << 3)) ui->lineEdit_A4->setStyleSheet("background-color:red");
    else ui->lineEdit_A4->setStyleSheet("background-color:green");
    ui->lineEdit_A5->setText(QString::number(area.threshold_JALVC164245.at(4)));
    if (res & (1 << 4)) ui->lineEdit_A5->setStyleSheet("background-color:red");
    else ui->lineEdit_A5->setStyleSheet("background-color:green");

    ui->lineEdit_A6->setText(QString::number(area.threshold_GH137S.at(0)));
    if (res & (1 << 5)) ui->lineEdit_A6->setStyleSheet("background-color:red");
    else ui->lineEdit_A6->setStyleSheet("background-color:green");
    ui->lineEdit_A7->setText(QString::number(area.threshold_GH137S.at(1)));
    if (res & (1 << 6)) ui->lineEdit_A7->setStyleSheet("background-color:red");
    else ui->lineEdit_A7->setStyleSheet("background-color:green");
    ui->lineEdit_A8->setText(QString::number(area.threshold_GH137S.at(2)));
    if (res & (1 << 7)) ui->lineEdit_A8->setStyleSheet("background-color:red");
    else ui->lineEdit_A8->setStyleSheet("background-color:green");
    ui->lineEdit_A9->setText(QString::number(area.threshold_GH137S.at(3)));
    if (res & (1 << 8)) ui->lineEdit_A9->setStyleSheet("background-color:red");
    else ui->lineEdit_A9->setStyleSheet("background-color:green");
    ui->lineEdit_A10->setText(QString::number(area.threshold_GH137S.at(4)));
    if (res & (1 << 9)) ui->lineEdit_A10->setStyleSheet("background-color:red");
    else ui->lineEdit_A10->setStyleSheet("background-color:green");

    ui->lineEdit_A11->setText(QString::number(area.threshold_SM1040.at(0)));
    if (res & (1 << 18)) ui->lineEdit_A11->setStyleSheet("background-color:red");
    else ui->lineEdit_A11->setStyleSheet("background-color:green");
    ui->lineEdit_A12->setText(QString::number(area.threshold_SM1040.at(1)));
    if (res & (1 << 19)) ui->lineEdit_A12->setStyleSheet("background-color:red");
    else ui->lineEdit_A12->setStyleSheet("background-color:green");

    ui->lineEdit_A13->setText(QString::number(area.threshold_J16H281.at(0)));
    if (res & (1 << 20)) ui->lineEdit_A13->setStyleSheet("background-color:red");
    else ui->lineEdit_A13->setStyleSheet("background-color:green");
    ui->lineEdit_A14->setText(QString::number(area.threshold_J16H281.at(1)));
    if (res & (1 << 21)) ui->lineEdit_A14->setStyleSheet("background-color:red");
    else ui->lineEdit_A14->setStyleSheet("background-color:green");

    ui->lineEdit_A15->setText(QString::number(area.threshold_SM490.at(0)));
    if (res & (1 << 22)) ui->lineEdit_A15->setStyleSheet("background-color:red");
    else ui->lineEdit_A15->setStyleSheet("background-color:green");
    ui->lineEdit_A16->setText(QString::number(area.threshold_SM490.at(1)));
    if (res & (1 << 23)) ui->lineEdit_A16->setStyleSheet("background-color:red");
    else ui->lineEdit_A16->setStyleSheet("background-color:green");

    ui->lineEdit_A17->setText(QString::number(area.threshold_100.at(0)));
    if (res & (1 << 24)) ui->lineEdit_A17->setStyleSheet("background-color:red");
    else ui->lineEdit_A17->setStyleSheet("background-color:green");
    ui->lineEdit_A18->setText(QString::number(area.threshold_100.at(1)));
    if (res & (1 << 25)) ui->lineEdit_A18->setStyleSheet("background-color:red");
    else ui->lineEdit_A18->setStyleSheet("background-color:green");

    ui->lineEdit_A19->setText(QString::number(area.threshold_JDSPF28335.at(0)));
    if (res & (1 << 10)) ui->lineEdit_A19->setStyleSheet("background-color:red");
    else ui->lineEdit_A19->setStyleSheet("background-color:green");
    ui->lineEdit_A20->setText(QString::number(area.threshold_JDSPF28335.at(1)));
    if (res & (1 << 11)) ui->lineEdit_A20->setStyleSheet("background-color:red");
    else ui->lineEdit_A20->setStyleSheet("background-color:green");
    ui->lineEdit_A21->setText(QString::number(area.threshold_JDSPF28335.at(2)));
    if (res & (1 << 12)) ui->lineEdit_A21->setStyleSheet("background-color:red");
    else ui->lineEdit_A21->setStyleSheet("background-color:green");
    ui->lineEdit_A22->setText(QString::number(area.threshold_JDSPF28335.at(3)));
    if (res & (1 << 13)) ui->lineEdit_A22->setStyleSheet("background-color:red");
    else ui->lineEdit_A22->setStyleSheet("background-color:green");
    ui->lineEdit_A23->setText(QString::number(area.threshold_JDSPF28335.at(4)));
    if (res & (1 << 14)) ui->lineEdit_A23->setStyleSheet("background-color:red");
    else ui->lineEdit_A23->setStyleSheet("background-color:green");
    ui->lineEdit_A24->setText(QString::number(area.threshold_JDSPF28335.at(5)));
    if (res & (1 << 15)) ui->lineEdit_A24->setStyleSheet("background-color:red");
    else ui->lineEdit_A24->setStyleSheet("background-color:green");
    ui->lineEdit_A25->setText(QString::number(area.threshold_JDSPF28335.at(6)));
    if (res & (1 << 16)) ui->lineEdit_A25->setStyleSheet("background-color:red");
    else ui->lineEdit_A25->setStyleSheet("background-color:green");
    ui->lineEdit_A26->setText(QString::number(area.threshold_JDSPF28335.at(7)));
    if (res & (1 << 17)) ui->lineEdit_A26->setStyleSheet("background-color:red");
    else ui->lineEdit_A26->setStyleSheet("background-color:green");

}

void MainWindow::board_B_area_disp(const Board_B_detec::Board_B_area_item &area, unsigned int res)
{
    ui->lineEdit_B1->setText(QString::number(area.threshold_GH137S.at(0)));
    if (res & (1 << 0)) ui->lineEdit_B1->setStyleSheet("background-color:red");
    else ui->lineEdit_B1->setStyleSheet("background-color:green");
    ui->lineEdit_B2->setText(QString::number(area.threshold_GH137S.at(1)));
    if (res & (1 << 1)) ui->lineEdit_B2->setStyleSheet("background-color:red");
    else ui->lineEdit_B2->setStyleSheet("background-color:green");
    ui->lineEdit_B3->setText(QString::number(area.threshold_GH137S.at(2)));
    if (res & (1 << 2)) ui->lineEdit_B3->setStyleSheet("background-color:red");
    else ui->lineEdit_B3->setStyleSheet("background-color:green");
    ui->lineEdit_B4->setText(QString::number(area.threshold_GH137S.at(3)));
    if (res & (1 << 3)) ui->lineEdit_B4->setStyleSheet("background-color:red");
    else ui->lineEdit_B4->setStyleSheet("background-color:green");
    ui->lineEdit_B5->setText(QString::number(area.threshold_GH137S.at(4)));
    if (res & (1 << 4)) ui->lineEdit_B5->setStyleSheet("background-color:red");
    else ui->lineEdit_B5->setStyleSheet("background-color:green");

    ui->lineEdit_B6->setText(QString::number(area.threshold_JT6H281.at(0)));
    if (res & (1 << 5)) ui->lineEdit_B6->setStyleSheet("background-color:red");
    else ui->lineEdit_B6->setStyleSheet("background-color:green");
    ui->lineEdit_B7->setText(QString::number(area.threshold_JT6H281.at(1)));
    if (res & (1 << 6)) ui->lineEdit_B7->setStyleSheet("background-color:red");
    else ui->lineEdit_B7->setStyleSheet("background-color:green");

    ui->lineEdit_B8->setText(QString::number(area.threshold_SM1040.at(0)));
    if (res & (1 << 7)) ui->lineEdit_B8->setStyleSheet("background-color:red");
    else ui->lineEdit_B8->setStyleSheet("background-color:green");
    ui->lineEdit_B9->setText(QString::number(area.threshold_SM1040.at(1)));
    if (res & (1 << 8)) ui->lineEdit_B9->setStyleSheet("background-color:red");
    else ui->lineEdit_B9->setStyleSheet("background-color:green");

    ui->lineEdit_B10->setText(QString::number(area.threshold_121.at(0)));
    if (res & (1 << 9)) ui->lineEdit_B10->setStyleSheet("background-color:red");
    else ui->lineEdit_B10->setStyleSheet("background-color:green");
    ui->lineEdit_B11->setText(QString::number(area.threshold_121.at(1)));
    if (res & (1 << 10)) ui->lineEdit_B11->setStyleSheet("background-color:red");
    else ui->lineEdit_B11->setStyleSheet("background-color:green");

    ui->lineEdit_B12->setText(QString::number(area.threshold_100.at(0)));
    if (res & (1 << 11)) ui->lineEdit_B12->setStyleSheet("background-color:red");
    else ui->lineEdit_B12->setStyleSheet("background-color:green");
    ui->lineEdit_B13->setText(QString::number(area.threshold_100.at(1)));
    if (res & (1 << 12)) ui->lineEdit_B13->setStyleSheet("background-color:red");
    else ui->lineEdit_B13->setStyleSheet("background-color:green");
}

void MainWindow::board_detect(QString & num_str)
{
    QString board_num;
    Mat image_src;


    m_board_res = m_board_ctrl.get_board_res(m_board_info, board_num, image_src);
    ui->lineEdit_num->setText(num_str);

    ui->label_disp_state_2->setText("数据处理中...耗时较长");
    cout<<"boadr_info is:"<<m_board_info<<endl;
    //--------------------------通过获取图片的信息来确定是Ａ面还是Ｂ面---------------------------------
    if (0 == m_board_info){
        Board_A_detec::Board_A_area_item area_item;
        m_board_ctrl.get_board_A_area(area_item);
        board_A_area_disp(area_item, m_board_res);
        ui->tabWidget->setCurrentIndex(0);
        ui->lineEdit_detec_board->setText("A");
    }
    else if (1 == m_board_info){
        Board_B_detec::Board_B_area_item area_item;
        m_board_ctrl.get_board_B_area(area_item);
        board_B_area_disp(area_item, m_board_res);
        ui->tabWidget->setCurrentIndex(1);
        ui->lineEdit_detec_board->setText("B");
    }
    else{
        //error
    }

    ui->label_disp_state_2->setText("数据处理完毕，可进行操作");
    if ((! image_src.empty()) && (-1 != m_board_info)){
        image_disp(image_src, QImage::Format_BGR888);
        if (0 == m_board_res){
            ui->lineEdit_res->setText("OK");
            ui->lineEdit_res->setStyleSheet("background-color:green");
        }
        else{
            ui->lineEdit_res->setText("NG");
            ui->lineEdit_res->setStyleSheet("background-color:red");
        }
    }
    else{
        ui->lineEdit_res->setText("ERR");
        ui->lineEdit_res->setStyleSheet("background-color:blue");
    }
}

void MainWindow::startObjThread()
{
    if(m_objThread){
        return;
    }
    m_objThread= new QThread();
    m_file_read_thread = new FileReadThread();
    m_file_read_thread->moveToThread(m_objThread);
    connect(m_objThread, &QThread::finished, m_objThread, &QObject::deleteLater);
    connect(m_objThread, &QThread::finished, m_file_read_thread, &QObject::deleteLater);
    connect(this, &MainWindow::run_file_search_signal, m_file_read_thread, &FileReadThread::run_read_file_thread);
    connect(this, &MainWindow::stop_file_search_signal, m_file_read_thread, &FileReadThread::stop);
    connect(m_file_read_thread, &FileReadThread::find_file_path_signal, this, &MainWindow::recv_file_path);
    connect(m_file_read_thread, &FileReadThread::thread_is_stop, [&](){
        ui->label_disp_state->setText("自动检测已停止!");
    });
    connect(m_file_read_thread, &FileReadThread::thread_is_running, [&](){
        ui->label_disp_state->setText("自动检测中!");
    });

    m_objThread->start();
}


/**
 * @brief MainWindow::on_pushButton_src_clicked
 */
void MainWindow::on_pushButton_src_clicked()
{
    //根据所选择的检测模式来决定导入文件夹还是文件路径
    //0手动模式,导入文件;１自动模式,导入文件夹
    if (0 == ui->comboBox->currentIndex()){
        QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
        ui->lineEdit_srcPath->setText(fileName);
    }
    else{
        QString dirpath = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
        ui->lineEdit_srcPath->setText(dirpath);
    }

    //只要存在文件或者文件夹路径，则禁止再选择检测模式，防止后面处理的时候就出错
    if (! ui->lineEdit_srcPath->text().isEmpty()){
        ui->comboBox->setDisabled(true);
    }
    else{
        ui->comboBox->setEnabled(true);
    }
}

/**
 * @brief MainWindow::setting_dialog_update
 * //slots
 */
void MainWindow::setting_dialog_update()
{
    Board_A_detec::Board_A_area_item board_A_para;
    Board_B_detec::Board_B_area_item board_B_para;

    m_settingsDialog->get_board_A_para(board_A_para);
    m_board_ctrl.set_board_A_area(board_A_para);

    m_settingsDialog->get_board_B_para(board_B_para);
    m_board_ctrl.set_board_B_area(board_B_para);
}

/**
 * @brief MainWindow::on_pushButton_clear_clicked
 * //清除按钮,清除所有检测信息，包括统计数据
 */
void MainWindow::on_pushButton_clear_clicked()
{
    ui->comboBox->setEnabled(true);
    ui->lineEdit_srcPath->setText("");

    board_NG_cnt = 0;      //OK
    board_OK_cnt = 0;      //NG
    board_ERR_cnt = 0;     //无法识别

    ui->lineEdit_OK_ratio->setText("");
    ui->lineEdit_detec_num->setText("0");
    ui->lineEdit_detec_err_num->setText("0");

    ui->label_disp_state->setText("");
    ui->label_disp_state_2->setText("");
    clear_detec_res();
}

void MainWindow::on_pushButton_stop_clicked()
{
    cout<<"emit(stop_file_stop_file_search_signalsearch_signal());"<<endl;
    if(m_objThread){
        if(m_file_read_thread){
            m_file_read_thread->stop();
        }
    }
}


static vector<QString> s_image_path_vec;

/**
 * @brief MainWindow::recv_file_path
 * //接收到图片文件循环检测线程的信号，表示已经检测到一个板子的正反两面图片
 * @param file_path
 * @param num_str
 */
void MainWindow::recv_file_path(QString &file_path, QString & num_str)
{
    int board_info_1, board_info_2;
    int res_1,res_2;
    bool err_flag = false;
    bool ng_flag = false;
    s_image_path_vec.push_back(file_path);
    num_str.resize(9);
    if (2 == s_image_path_vec.size()){
        cv::Mat src_img = imread(s_image_path_vec.at(0).toStdString());
        m_board_ctrl.set_image_src(src_img);
        board_detect(num_str);

        if ((-1 != m_board_info)){
            board_info_1 = m_board_info;
            res_1 = m_board_res;

            src_img = imread(s_image_path_vec.at(1).toStdString());
            m_board_ctrl.set_image_src(src_img);
            board_detect(num_str);

            if (-1 != m_board_info){
                board_info_2 = m_board_info;
                res_2 = m_board_res;

                if ((0 == res_1)&&(0 == res_2)){
                    board_OK_cnt ++;
                }
                else{
                    board_NG_cnt ++;
                    ng_flag = true;
                }
            }
            else{
                board_ERR_cnt ++;
                err_flag = true;
            }//end if (-1 != m_board_info)
        }
        else{
            board_ERR_cnt ++;
            err_flag = true;
        }//end

        ui->lineEdit_detec_num->setText(QString::number(board_NG_cnt + board_OK_cnt + board_ERR_cnt));
        double ratio = board_OK_cnt * 1.0/(board_NG_cnt+board_ERR_cnt+board_OK_cnt);
        ui->lineEdit_OK_ratio->setText(QString::number(ratio));
        ui->lineEdit_detec_err_num->setText(QString::number(board_ERR_cnt));

        {
            int res_flag = 0;
            std::vector<double> area_vec;
            if (err_flag) res_flag = -1;
            if (ng_flag) res_flag = 1;

            for (int var = 0; var < m_board_A_line_edit_vec.size(); ++var){
                area_vec.push_back(m_board_A_line_edit_vec.at(var)->text().toDouble());
            }
            for (int var = 0; var < m_board_B_line_edit_vec.size(); ++var){
                area_vec.push_back(m_board_B_line_edit_vec.at(var)->text().toDouble());
            }
            m_log.res_write(num_str, res_flag, area_vec);
        }


        //当发生检测异常，且处于自动检测模式下时，将停止继续检测，等待工作人员进行处理
        if (((err_flag == true) || (ng_flag == true)) && (1 == ui->comboBox->currentIndex())){
            QMessageBox::warning(this, "警告", tr("电路板编号:%1",
                                                "检测异常，系统将停止检测请查明原因后再点击＂开始＂按钮，继续检测").arg(num_str));
            s_image_path_vec.clear();
            m_file_read_thread->stop();
            return;
        }


        QFile fileTemp(s_image_path_vec.at(0));
        fileTemp.remove();
        fileTemp.setFileName(s_image_path_vec.at(1));
        fileTemp.remove();

        s_image_path_vec.clear();
        m_file_read_thread->file_unlock();
    }
}


/**
 * @brief MainWindow::template_para_changed
 */
void MainWindow::template_para_changed()
{
    double template_ratio = m_templateSet->get_ratio();
    m_board_ctrl.set_best_radius(m_templateSet->get_radius());  //读取所设置的最佳半径并传递给检测控制类

    vector<QString> file_path_vec;

    m_templateSet->get_template(file_path_vec);

    if (5 != file_path_vec.size()){
        QMessageBox::warning(this, "警告", "请设置模板图片路径后再进行检测，否则系统会崩溃");
        m_template_setting_is_OK = false;
        return;
    }
    for (int i =0; i < file_path_vec.size(); i ++){
        if (file_path_vec.at(i).isEmpty()){
            QMessageBox::warning(this, "警告", "请设置模板图片路径后再进行检测，否则系统会崩溃");
            m_template_setting_is_OK = false;
            return;
        }
    }

    m_template_setting_is_OK = true;

    //------------------------------------读取模板信息---------------------------------
    cv::Mat rotate_template_img = imread(file_path_vec.at(0).toStdString(), 1);
    vector<cv::Mat> template_img_vec;
    cv::Mat temp;
    temp = imread(file_path_vec.at(1).toStdString(), 1);
    cv::resize(temp, temp, Size(temp.cols*template_ratio,temp.rows*template_ratio));
    template_img_vec.push_back(temp);
    temp = imread(file_path_vec.at(2).toStdString(), 1);
    cv::resize(temp, temp, Size(temp.cols*template_ratio,temp.rows*template_ratio));
    template_img_vec.push_back(temp);
    temp = imread(file_path_vec.at(3).toStdString(), 1);
    cv::resize(temp, temp, Size(temp.cols*template_ratio,temp.rows*template_ratio));
    template_img_vec.push_back(temp);
    temp = imread(file_path_vec.at(4).toStdString(), 1);
    cv::resize(temp, temp, Size(temp.cols*template_ratio,temp.rows*template_ratio));
    template_img_vec.push_back(temp);

    cv::resize(rotate_template_img, rotate_template_img, Size(rotate_template_img.cols*template_ratio,rotate_template_img.rows*template_ratio));
    m_board_ctrl.set_rotate_template(rotate_template_img);
    m_board_ctrl.set_match_template(template_img_vec);
}

void MainWindow::error_info_with_err_detect()
{

}

/**
 * @brief MainWindow::clear_detec_res
 * //清除检测结果，不包括累计统计数据
 */
void MainWindow::clear_detec_res()
{
    for (int var = 0; var < m_board_A_line_edit_vec.size(); ++var){
        m_board_A_line_edit_vec.at(var)->setText("");
        m_board_A_line_edit_vec.at(var)->setStyleSheet("background-color:gray");
    }
    for (int var = 0; var < m_board_B_line_edit_vec.size(); ++var){
        m_board_B_line_edit_vec.at(var)->setText("");
        m_board_B_line_edit_vec.at(var)->setStyleSheet("background-color:gray");
    }
    ui->lineEdit_detec_board->setText("");
    ui->lineEdit_num->setText("");
    ui->lineEdit_res->setText("");
    ui->lineEdit_res->setStyleSheet("background-color:gray");
    this->repaint();
}

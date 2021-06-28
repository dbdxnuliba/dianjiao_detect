#include "dialogtemplatesetting.h"
#include "ui_dialogtemplatesetting.h"

#include "QFileDialog"
#include "QSettings"

#include "iostream"

using namespace std;

const QString local_key_arr[7] = {"Template_B_path", "Template_1_path", "Template_2_path", "Template_3_path"\
                                 , "Template_4_path", "Ratio", "Radius"};

const QString deafult_template_path_arr[5] = {"/home/panhan/Desktop/template/std.jpg",\
                                              "/home/panhan/Desktop/template/temp1.jpg",\
                                              "/home/panhan/Desktop/template/temp2.jpg",\
                                              "/home/panhan/Desktop/template/temp3.jpg",\
                                              "/home/panhan/Desktop/template/temp4.jpg"};

const double default_ratio = 0.666667;
const int default_radius = 80;
DialogTemplateSetting::DialogTemplateSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTemplateSetting)
{
    ui->setupUi(this);

    ui->lineEdit_0->setDisabled(true);
    ui->lineEdit_1->setDisabled(true);
    ui->lineEdit_2->setDisabled(true);
    ui->lineEdit_3->setDisabled(true);
    ui->lineEdit_4->setDisabled(true);



//    m_ratio = 2.0/3.0;
//    m_radius = 80;
//    m_template_arr[0] = QString("/home/panhan/Desktop/template/std.jpg");
//    m_template_arr[1] = QString("/home/panhan/Desktop/template/temp1.jpg");
//    m_template_arr[2] = QString("/home/panhan/Desktop/template/temp2.jpg");
//    m_template_arr[3] = QString("/home/panhan/Desktop/template/temp3.jpg");
//    m_template_arr[4] = QString("/home/panhan/Desktop/template/temp4.jpg");


    QSettings* cfg = new QSettings("/home/panhan/Desktop/template/Config.ini", QSettings::IniFormat);
    cfg->beginGroup("TemplaetSetting");

    for (int var = 0; var < 5; ++var) {
        QString temp_path;
        temp_path = cfg->value(local_key_arr[var]).toString();
        if (temp_path.isEmpty()) cfg->setValue(local_key_arr[var], deafult_template_path_arr[var]);
        temp_path = cfg->value(local_key_arr[var]).toString();
        m_template_arr[var] = temp_path;
    }
    QString temp_str = cfg->value(local_key_arr[5]).toString();
    if (temp_str.isEmpty()){
        cfg->setValue(local_key_arr[5], QString::number(default_ratio));
    }
    m_ratio = cfg->value(local_key_arr[5]).toDouble();

    temp_str = cfg->value(local_key_arr[6]).toString();
    if (temp_str.isEmpty()){
        cfg->setValue(local_key_arr[6], QString::number(default_radius));
    }
    m_radius = cfg->value(local_key_arr[6]).toInt();

    cfg->endGroup();

    ui->lineEdit_5->setText(QString::number(m_ratio));
    ui->lineEdit_6->setText(QString::number(m_radius));
    ui->lineEdit_0->setText(m_template_arr[0]);
    ui->lineEdit_1->setText(m_template_arr[1]);
    ui->lineEdit_2->setText(m_template_arr[2]);
    ui->lineEdit_3->setText(m_template_arr[3]);
    ui->lineEdit_4->setText(m_template_arr[4]);

    delete cfg;
}

DialogTemplateSetting::~DialogTemplateSetting()
{
    delete ui;
}

void DialogTemplateSetting::get_template(std::vector<QString> &temp_file)
{
    temp_file.clear();
    for (int var = 0; var < 5; ++var) {
        temp_file.push_back(m_template_arr[var]);
    }
}

double DialogTemplateSetting::get_ratio()
{
    return m_ratio;
}

int DialogTemplateSetting::get_radius()
{
    return m_radius;
}

void DialogTemplateSetting::on_pushButton_0_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
    if (! fileName.isEmpty()){
        ui->lineEdit_0->setText(fileName);
    }
}

void DialogTemplateSetting::on_pushButton_1_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
    if (! fileName.isEmpty()){

        ui->lineEdit_1->setText(fileName);
    }
}

void DialogTemplateSetting::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
    if (! fileName.isEmpty()){

        ui->lineEdit_2->setText(fileName);
    }
}

void DialogTemplateSetting::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
    if (! fileName.isEmpty()){

        ui->lineEdit_3->setText(fileName);
    }
}

void DialogTemplateSetting::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文件");
    if (! fileName.isEmpty()){

        ui->lineEdit_4->setText(fileName);
    }
}

void DialogTemplateSetting::on_pushButton_OK_clicked()
{
    m_template_arr[0] = ui->lineEdit_0->text();
    m_template_arr[1] = ui->lineEdit_1->text();
    m_template_arr[2] = ui->lineEdit_2->text();
    m_template_arr[3] = ui->lineEdit_3->text();
    m_template_arr[4] = ui->lineEdit_4->text();

    m_ratio = ui->lineEdit_5->text().toDouble();
    m_radius = ui->lineEdit_6->text().toInt();


    QSettings* cfg = new QSettings("/home/panhan/Desktop/template/Config.ini", QSettings::IniFormat);
    cfg->beginGroup("TemplaetSetting");

    for (int var = 0; var < 5; ++var) {
        cfg->setValue(local_key_arr[var], m_template_arr[var]);
    }
    cfg->setValue(local_key_arr[5], QString::number(m_ratio));
    cfg->setValue(local_key_arr[6], QString::number(m_radius));

    cfg->endGroup();

    emit update_para_signal();

    hide();

    delete cfg;
}

void DialogTemplateSetting::on_pushButton_CANCEL_clicked()
{
    ui->lineEdit_0->setText(m_template_arr[0]);
    ui->lineEdit_1->setText(m_template_arr[1]);
    ui->lineEdit_2->setText(m_template_arr[2]);
    ui->lineEdit_3->setText(m_template_arr[3]);
    ui->lineEdit_4->setText(m_template_arr[4]);

    ui->lineEdit_5->setText(QString::number(m_ratio));
    ui->lineEdit_6->setText(QString::number(m_radius));

    hide();
}

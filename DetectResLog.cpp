#include "DetectResLog.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDevice>
#include <QTextStream>

DetectResLog::DetectResLog(QObject *parent):QObject(parent)
{
    m_write_flag = 0;
}

DetectResLog::~DetectResLog()
{

}

/**
 * @brief DetectResLog::set_file_path setting txt file path
 * @param file_path
 * @return
 */
int DetectResLog::set_file_path(QString &file_path)
{
    m_abs_file_name = file_path;
    QFile file(m_abs_file_name);
    if (! file.open(QIODevice::Append|QIODevice::Text|QIODevice::ReadWrite)){
        return -1;
    }
    file.close();
    return 0;
}

int DetectResLog::res_write_num_str(QString &num_str)
{
    m_write_str[0] = num_str;
    m_write_flag |= (1 << 0);

    return write_finished();
}

int DetectResLog::res_write_board_res(int board_res)
{
    QString res_str;
    if (0 == board_res) m_write_str[1] = QString("OK");
    else if (1 == board_res) m_write_str[1] = QString("NG");
    else m_write_str[1] = QString("ERR");

    m_write_flag |= (1 << 1);
    return write_finished();
}

int DetectResLog::res_write_A_area(std::vector<double> &area)
{
    QString area_str;
    for (unsigned int var = 0; var < area.size(); ++var){
        area_str += QString::number(area.at(var));
        area_str += QString(" ");
    }
    m_write_str[2] = area_str;
    m_write_flag |= (1 << 2);

    return write_finished();

}

int DetectResLog::res_write_B_area(std::vector<double> &area)
{
    QString area_str;
    for (unsigned int var = 0; var < area.size(); ++var){
        area_str += QString::number(area.at(var));
        area_str += QString(" ");
    }
    m_write_str[3] = area_str;
    m_write_flag |= (1 << 3);

    return write_finished();
}

int DetectResLog::res_write_A_area_res(vector<bool> &area_res)
{
    QString area_res_str;
    for (unsigned int var = 0; var < area_res.size(); ++var){
        QString str_temp;
        if (true == area_res.at(var)){
            str_temp = QString("A_%1_NG").arg(var+1);
            area_res_str += str_temp;
            area_res_str += QString(" ");
        }
    }
    m_write_str[4] = area_res_str;
    m_write_flag |= (1 << 4);

    return write_finished();
}

int DetectResLog::res_write_B_area_res(vector<bool> &area_res)
{
    QString area_res_str;
    for (unsigned int var = 0; var < area_res.size(); ++var){
        QString str_temp;
        if (true == area_res.at(var)){
            str_temp = QString("B_%1_NG").arg(var+1);
            area_res_str += str_temp;
            area_res_str += QString(" ");
        }
    }
    m_write_str[5] = area_res_str;
    m_write_flag |= (1 << 5);

    return write_finished();
}

void DetectResLog::clear_all_flag()
{
    m_write_flag = 0;
}

int DetectResLog::write_finished()
{
    if (m_write_flag == 0x3f){
        QFile file(m_abs_file_name);
        if (! file.open(QIODevice::Append|QIODevice::Text)){
            return -1;
        }
        QTextStream textStream(&file);       //读取文件，使用QTextStream
        m_write_flag = 0;
        textStream << m_write_str[0] <<'\t'<< m_write_str[1]<<'\t'<<m_write_str[4]<<'\t'<<m_write_str[5]<<'\t'<<m_write_str[2]<<'\t'<<m_write_str[3]<<"\r\n";

        emit write_is_finished();
    }
    return 0;
}

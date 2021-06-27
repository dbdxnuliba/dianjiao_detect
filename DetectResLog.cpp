#include "DetectResLog.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDevice>
#include <QTextStream>


using namespace std;
DetectResLog::DetectResLog(QObject *parent):QObject(parent)
{

}

DetectResLog::~DetectResLog()
{

}

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

int DetectResLog::res_write(QString &num_str, int board_res, std::vector<double> &area)
{
    QFile file(m_abs_file_name);
    if (! file.open(QIODevice::Append|QIODevice::Text)){
        return -1;
    }
    QTextStream textStream(&file);       //读取文件，使用QTextStream

    QString res_str;
    if (0 == board_res){
        res_str = QString("OK");
    }
    else if (1 == board_res){
        res_str = QString("NG");
    }
    else{
        res_str = QString("ERR");
    }

    QString area_str;
    for (unsigned int var = 0; var < area.size(); ++var){
        area_str += QString::number(area.at(var));
        area_str += QString(" ");
    }
    textStream << num_str <<'\t'<< res_str<<'\t'<<area_str<<"\r\n";
    file.close();

    return 0;
}

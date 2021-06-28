/********************************************************************************
  **模块名称:DetectResLog,检测结果记录模块
  **模块功能:将检测结果写入指定的txt文档
  **作者:panhan
  **修改记录:
  *******************************************************************************/
#ifndef DETECTRESLOG_H
#define DETECTRESLOG_H

#include "QFileDialog"


using namespace std;
class DetectResLog : public QObject
{
    Q_OBJECT
public:
    explicit DetectResLog(QObject *parent = 0);
    ~DetectResLog();
    int set_file_path(QString & file_path);
    int res_write_num_str(QString & num_str);
    int res_write_board_res(int board_res);
    int res_write_A_area(std::vector<double> & area);
    int res_write_B_area(std::vector<double> & area);
    int res_write_A_area_res(vector<bool> & area_res);
    int res_write_B_area_res(vector<bool> & area_res);
    void clear_all_flag();
signals:
    void write_is_finished();
private:
    int write_finished();
private:
    QString m_abs_file_name;
    unsigned char m_write_flag;
    QString m_write_str[6];
};

#endif // DETECTRESLOG_H

#ifndef DETECTRESLOG_H
#define DETECTRESLOG_H

#include "QFileDialog"

class DetectResLog : public QObject
{
    Q_OBJECT
public:
    explicit DetectResLog(QObject *parent = 0);
    ~DetectResLog();
    int set_file_path(QString & file_path);
    int res_write(QString & num_str, int board_res, std::vector<double> & area);
private:
    QString m_abs_file_name;
};

#endif // DETECTRESLOG_H

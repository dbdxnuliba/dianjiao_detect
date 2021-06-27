#ifndef DIALOGTEMPLATESETTING_H
#define DIALOGTEMPLATESETTING_H

#include <QDialog>

namespace Ui {
class DialogTemplateSetting;
}

class DialogTemplateSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTemplateSetting(QWidget *parent = nullptr);
    ~DialogTemplateSetting();
    void get_template(std::vector<QString> & temp_file);
    double get_ratio();
    int get_radius();
private slots:
    void on_pushButton_0_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_OK_clicked();

    void on_pushButton_CANCEL_clicked();
signals:
    void update_para_signal();
private:
    Ui::DialogTemplateSetting *ui;
    QString m_template_arr[5];
    double m_ratio;
    int m_radius;
};

#endif // DIALOGTEMPLATESETTING_H

#ifndef FILEREADTHREAD_H
#define FILEREADTHREAD_H

#include <QThread>
#include "QFileDialog"
#include <QMutex>
#include <QMetaType>

class FileReadThread : public QObject
{
    Q_OBJECT
public:
    explicit FileReadThread(QObject *parent = 0);
    ~FileReadThread();
    void set_src_dir(QDir & dir);
    void file_unlock();
public slots:
    void run_read_file_thread();
    void stop();

signals:
    void find_file_path_signal(QString & file_path, QString & num_str);
    void thread_is_running();
    void thread_is_stop();
private:
    bool m_is_stop;
    QDir m_dir;
    bool m_file_lock;
};

#endif // FILEREADTHREAD_H

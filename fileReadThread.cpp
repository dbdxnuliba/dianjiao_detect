#include "fileReadThread.h"

#include <iostream>


FileReadThread::FileReadThread(QObject *parent):QObject(parent)
{
    m_is_stop = true;
    m_file_lock = false;
    qRegisterMetaType<QString>("QString &");
}

FileReadThread::~FileReadThread()
{

}

void FileReadThread::set_src_dir(QDir &dir)
{
    m_dir = dir;
}

void FileReadThread::file_unlock()
{
    m_file_lock = false;
}

void FileReadThread::run_read_file_thread()
{
    m_is_stop = false;
    m_file_lock = false;

    m_dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    m_dir.setSorting(QDir::Time);
    QFileInfoList file_list = m_dir.entryInfoList();
    QString board_num;

    int start_index = 0;
    int end_index = 0;
    while (1) {
        if (m_is_stop){
            std::cout<<"m_is_stop"<<std::endl;
            emit thread_is_stop();
            return;
        }

        if (false == m_file_lock){
            m_dir.refresh();
            file_list = m_dir.entryInfoList();  //更新文件列表

            if (! file_list.isEmpty()){

                for (start_index = 0; start_index < (file_list.size() - 1); ++ start_index){
                    board_num = file_list.at(start_index).fileName();   //获取起始文件名
                    board_num.resize(9);                                //提取其编号字段，总长９位
                    end_index = 0;
                    //遍历其后的文件名，寻找同编号文件
                    for (int i = start_index+1; i < file_list.size(); i++){
                        if (file_list.at(i).fileName().contains(board_num)){
                            end_index = i;
                            break;
                        }
                    }
                    if (0 != end_index){
                        QString file_path = file_list.at(start_index).absoluteFilePath();
                        emit find_file_path_signal(file_path, board_num);
                        file_path = file_list.at(end_index).absoluteFilePath();
                        emit find_file_path_signal(file_path, board_num);

                        m_file_lock = true; //lock
                        break;
                    }
                }

            }
        }
        emit thread_is_running();
        QThread::sleep(1);
    }
}

void FileReadThread::stop()
{
    m_is_stop = true;
}


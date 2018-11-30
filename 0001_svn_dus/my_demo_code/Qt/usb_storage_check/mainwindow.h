#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <QMainWindow>
#include <dbt.h>
#include<devguid.h>
#include<SetupAPI.h>
#include<InitGuid.h>
#include <QString>
#include <QWidget>
#include <QListWidgetItem>
#include<QMessageBox>
#include<QDebug>
#include<QFile>
#include<QFileDialog>
#include<QDir>
#include<QTextStream>
#include<QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow// ,QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //---双击浏览器中显示的目录进入下级，或者返回上一级
    void showFileInfoList(QFileInfoList list);
    QString usb_tag;
    ~MainWindow();
public slots:
    //显示当前目录下的所有文件
    void slotShow(QDir dir);
    //根据选择显示下一级目录的文件
    void slotDirShow(QListWidgetItem *Item);


private:
    QLineEdit *fileLineEdit;
    QListWidget *fileListWidget;
    QVBoxLayout *vLayout;
    QFileInfoList list;
    Ui::MainWindow *ui;
    char FirstDriveFromMask (ULONG unitmask);
protected:
    bool nativeEvent(const QByteArray & eventType, void * message, long*result);
};

#endif // MAINWINDOW_H

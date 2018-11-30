#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
//#include <locale>
#include <QFile>
#include <string>
#include <QLineEdit>
#include <QTextCodec>
#include <QListWidgetItem>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/ossl_typ.h>

#define EXIT_IF_TRUE(x) if (x)                              \
do{                                                    \
    fprintf(stderr, "Check '%s' is true\n", #x);    \
    ERR_print_errors_fp(stderr);                    \
    exit(2);                                        \
}while(0)

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)/*,QWidget(parent)*/,ui(new Ui::MainWindow){
    ui->setupUi(this);
    //实例化对象
    fileLineEdit = new QLineEdit("\\",this);
    fileListWidget = new QListWidget(this);
    fileLineEdit->setFixedSize(500,300); //设置控件大小
    fileListWidget->setFixedSize(500,300);//设置控件大小
    //布局
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(fileLineEdit);
    vLayout->addWidget(fileListWidget);
    this->setLayout(vLayout);//使用控件填充当前的控件
    vLayout->setMargin(15);
    //设置对应信号与槽
    connect(fileLineEdit,SIGNAL(returnPressed()),this,SLOT(slotShow(QDir)));
    connect(fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slotDirShow(QListWidgetItem*)));
#if 1
    static const GUID GUID_DEVINTERFACE_LIST[] ={
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
    // GUID_DEVINTERFACE_DISK
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
    // GUID_DEVINTERFACE_HID,
    { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
    // GUID_NDIS_LAN_CLASS
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
    //// GUID_DEVINTERFACE_COMPORT
    //{ 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },
    //// GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
    //{ 0x4D36E978, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } },
    //// GUID_DEVINTERFACE_PARALLEL
    //{ 0x97F76EF0, 0xF883, 0x11D0, { 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C } },
    //// GUID_DEVINTERFACE_PARCLASS
    //{ 0x811FC6A5, 0xF728, 0x11D0, { 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1 } }
    };
    //注册插拔事件
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    for(int i=0;i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID);i++){
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];//GetCurrentUSBGUID();//m_usb->GetDriverGUID();
        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(),&NotifacationFiler,DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify){
            GetLastError();
            qDebug() << "注册失败" <<endl;
        }
    }
#endif
}
MainWindow::~MainWindow(){
    delete ui;
}
char MainWindow::FirstDriveFromMask(ULONG unitmask){
    char i;
    for (i = 0; i < 26; ++i){
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result){
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType==WM_DEVICECHANGE){
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME){
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags ==0){
                    //插入u盘
                    QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    qDebug() <<"---------------------------------------------";
                    qDebug() << "USB_Arrived and The USBDisk is: "<<USBDisk ;
                    //ui->textBrowser->append("USB_Arrived and The USBDisk is " + USBDisk);
                    QString lpRootPathName = USBDisk+":\\";
                    this->usb_tag = lpRootPathName;
                    qDebug() <<" lpRootPathName = "<< lpRootPathName;
                    LPTSTR lpVolumeNameBuffer=new TCHAR[12];//磁盘卷标
                    DWORD nVolumeNameSize=12;// 卷标的字符串长度
                    DWORD VolumeSerialNumber;//硬盘序列号
                    DWORD MaximumComponentLength;// 最大的文件长度
                    LPTSTR lpFileSystemNameBuffer=new TCHAR[10];// 存储所在盘符的分区类型的长指针变量
                    DWORD nFileSystemNameSize=10;// 分区类型的长指针变量所指向的字符串长度
                    DWORD FileSystemFlags;// 文件系统的一此标志
                    GetVolumeInformation((LPTSTR)lpRootPathName.utf16(),lpVolumeNameBuffer,
                                         nVolumeNameSize,&VolumeSerialNumber, &MaximumComponentLength,
                                         &FileSystemFlags,lpFileSystemNameBuffer, nFileSystemNameSize);
                    qDebug() <<" VolumeSerialNumber = "<< VolumeSerialNumber;
                }
                //read file dzb.txt
                QString fileName = this->usb_tag+"\dzb.txt";
                //fileLineEdit = new QLineEdit(this->usb_tag,this);
                fileLineEdit->setText(this->usb_tag);
                //布局
                vLayout->addWidget(fileLineEdit);
                vLayout->addWidget(fileListWidget);
                QDir rootDir(this->usb_tag);
                QStringList stringlist;
                stringlist << "*";
                list = rootDir.entryInfoList(stringlist);
                showFileInfoList(list);
                //显示布局
                setLayout(vLayout);
                //设置窗口属性
                setWindowTitle("File View");
                if(fileName.isEmpty()){
                    qDebug() <<"file is Empty";
                }else{
                     QFile *file = new QFile;
                     qDebug()<<"filename:"<<fileName;
                     file->setFileName(fileName);//新建一个文件对象，并且把它设置为上面获取的文件
                     if(file->exists()!=true){

                     }
                     bool ok=file->open(QIODevice::ReadOnly);//设置打开模式
                     if(ok){//如果可以打开
                          //将文件与文本流相关联
                          QTextStream in(file);
                          //ui->textEdit->setText(in.readAll());//读取该文件的所有内容
                          QString buff = in.readAll();
                          qDebug()<<"buff:"<<buff;
                          file->close();//关闭文件
                          //delete file();//释放文件进程
                          delete(file);
                      }else{
                         qDebug() <<"file is fail";
                      }
                 }
                //生成公钥私钥
                QString strRSAPath = this->usb_tag+"./key";
                QDir dir;
                if(dir.exists(strRSAPath) == true){//文件夹存在，删除文件夹
                    dir.rmpath(strRSAPath);
                }
                dir.mkpath(strRSAPath);//创建文件夹
                BIGNUM *bne = BN_new();
                int ret = BN_set_word(bne,RSA_F4);//设置随机数长度
                RSA *rsa = RSA_new();
                ret = RSA_generate_key_ex(rsa,1024,bne,NULL);
                if(ret != 1){
                    qDebug()<<"create RSA ERROR.";
                    return false;
                }
                /*提取私钥*/
                QString strPrivateKeyPath = strRSAPath+"/privateKey.pem";
                QByteArray qbaPrivatekeyPath = strPrivateKeyPath.toLatin1();
                FILE *filename = NULL;
                filename = fopen(qbaPrivatekeyPath.data(),"wb");
                PEM_write_RSAPrivateKey(filename, rsa, NULL, NULL, 0, NULL, NULL);
                fclose(filename);
                filename = NULL;
                /* 提取公钥 */
                unsigned char *n_b = (unsigned char *)calloc(RSA_size(rsa), sizeof(unsigned char));
                unsigned char *e_b = (unsigned char *)calloc(RSA_size(rsa), sizeof(unsigned char));
                int n_size = BN_bn2bin(rsa->n, n_b);
                int b_size = BN_bn2bin(rsa->e, e_b);
                RSA *pubrsa = RSA_new();
                pubrsa->n = BN_bin2bn(n_b, n_size, NULL);
                pubrsa->e = BN_bin2bn(e_b, b_size, NULL);
            //    //另一种格式的公钥输出
            //    filename = fopen("./key/publicKeyRSA.pem", "wb");
            //    PEM_write_RSAPublicKey(filename, pubrsa);
            //    fclose(filename);
            //    filename = NULL;
                QString strPublicKeyPath = strRSAPath+"/publicKey.pem";
                QByteArray qbaPublickeyPath = strPublicKeyPath.toLatin1();
                filename = fopen(qbaPublickeyPath.data(), "wb");
                PEM_write_RSA_PUBKEY(filename, pubrsa);
                fclose(filename);
                filename = NULL;
                RSA_free(pubrsa);
                RSA_free(rsa);
                //公钥私钥生成完毕
                //生成证书

            }
            if(lpdb->dbch_devicetype = DBT_DEVTYP_DEVICEINTERFACE){
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
               // ui->textBrowser->append("插入设备 ：" + strname);
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            qDebug() << "设备移除" <<endl;
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME){
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0){
                    qDebug() << "USB_Removed";
                    //ui->textBrowser->append("USB_Remove");
                }
            }
            if(lpdb->dbch_devicetype = DBT_DEVTYP_DEVICEINTERFACE){
                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                qDebug()<< "移除设备(name)：" << pDevInf->dbcc_name;
                //qDebug()<< "移除设备(guid)：" << pDevInf->dbcc_classguid;
                qDebug()<< "移除设备(size)：" << pDevInf->dbcc_size;
                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
                //ui->textBrowser->append("移除设备 " + strname);
                //qDebug()<< "USB_Removed";
            }
            break;
        }
    }
    return false;
}
//显示当前目录下的所有文件
void MainWindow::slotShow(QDir dir){
    QStringList stringlist;
    stringlist <<"*";
    QFileInfoList InfoList = dir.entryInfoList(stringlist,QDir::AllEntries,QDir::DirsFirst);
    showFileInfoList(InfoList);
}
void MainWindow::showFileInfoList(QFileInfoList list){
    //清空列表控件
    fileListWidget->clear();
    //取出所有项，按照目录，文件方式添加到控件内
    for(unsigned int i = 0;i<list.count();i++){
        QFileInfo tmpFileInfo = list.at(i);
        if(tmpFileInfo.isDir()){
            QIcon icon("pic/dir.ico");
            QString filename = tmpFileInfo.fileName();
            QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
            //不需要填充颜色
            tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明

            //tmpListWidgetItem->setTextColor(QColor(255,255,255,255));
            fileListWidget->addItem(tmpListWidgetItem);
        }else{
            QString filename = tmpFileInfo.fileName();
            int first = filename.lastIndexOf(".");
            QString suffix = filename.right(filename.length()-first-1);//从右边截取 获取后缀名
            suffix = suffix.toLower();//将后缀名转为全小写
            if(suffix.compare(QString::fromLocal8Bit("doc"))==0){
                QIcon icon("pic/doc.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else if((suffix.compare(QString::fromLocal8Bit("jpg"))==0)||(suffix.compare(QString::fromLocal8Bit("png"))==0)){
                QIcon icon("pic/jpg.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else if((suffix.compare(QString::fromLocal8Bit("exe"))==0)||(suffix.compare(QString::fromLocal8Bit("bat"))==0)){
                QIcon icon("pic/exe.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else if((suffix.compare(QString::fromLocal8Bit("iso"))==0)||(suffix.compare(QString::fromLocal8Bit("img"))==0)){
                QIcon icon("pic/iso.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else if((suffix.compare(QString::fromLocal8Bit("mp3"))==0)||(suffix.compare(QString::fromLocal8Bit("amr"))==0)){
                QIcon icon("pic/mp3.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else if((suffix.compare(QString::fromLocal8Bit("mp4"))==0)||(suffix.compare(QString::fromLocal8Bit("mkv"))==0)){
                QIcon icon("pic/mp4.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else if((suffix.compare(QString::fromLocal8Bit("zip"))==0)||(suffix.compare(QString::fromLocal8Bit("rar"))==0)){
                QIcon icon("pic/zip.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }else{
                QIcon icon("pic/file.ico");
                QListWidgetItem *tmpListWidgetItem = new QListWidgetItem(icon,filename);
                tmpListWidgetItem->setBackgroundColor(QColor(127,127,127,127));//rgb 和灰度，0 透明，255 完全不透明
                fileListWidget->addItem(tmpListWidgetItem);
            }
            //qDebug()<<"file_suffix:"<<suffix;
        }
    }
}
//根据用户的选择显示下一级目录文件
void MainWindow::slotDirShow(QListWidgetItem *Item){
    //保存下一级目录名称
    QString string = Item->text();
    QDir dir;
    //设置路径为当前目录路径
    dir.setPath(fileLineEdit->text());
    //重新设置路径
    dir.cd(string);
    //更新当前显示路径，这里是获取的绝对路径
    fileLineEdit->setText(dir.absolutePath());
    //显示当前文件目录下的所有文件
    slotShow(dir);
}


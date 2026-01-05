#include "inc/prosetpage.h"
#include "ui_prosetpage.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath", ui->lineEdit_2);
    registerField("proName*", ui->lineEdit);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);

    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText( curPath );
    ui->lineEdit_2->setCursorPosition( ui->lineEdit_2->text().size());
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit_2->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

bool ProSetPage::isComplete() const
{
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == ""){
         return false;
     }

     // 判断是否文件夹是否合理
     QDir dir(ui->lineEdit_2->text());
     if(!dir.exists())
     {
        //qDebug()<<"file path is not exists" << endl;
        ui->tips->setText("project path is not exists");
        return false;
     }

     // 判断路径是否存在
     QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
     // qDebug() << "absFilePath is " << absFilePath;

     QDir dist_dir(absFilePath);
     if(dist_dir.exists()){
         ui->tips->setText("project has exists, change path or name!");
         return false;
     }

     ui->tips->setText("");
     return QWizardPage::isComplete();
}

/**
 * @brief 添加浏览按钮点击后选择文件夹的操作
**/
void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    // 仅在Linux上强制使用Qt对话框
    #if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
        // Linux下默认的对话框模式设置QFileDialog::ShowDirsOnly不生效
        file_dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    #endif
    file_dialog.setOption(QFileDialog::ShowDirsOnly);

    file_dialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    file_dialog.setDirectory(path);
    // Displays an icon, a name, and details for each item in the directory.
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec() == QDialog::Accepted){
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.isEmpty()){
         return;
    }

    QString import_path = fileNames.at(0);
    qDebug() << "import_path is " << import_path << endl;
    ui->lineEdit_2->setText(import_path);
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

#include "widget.h"
#include "ui_widget.h"
bool copyDir(const QString &source, const QString &destination, bool override)
{
    QDir directory(source);
    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(destination);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();
    bool error = false;
    QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i=0; i != fileNames.size(); ++i)
    {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink())
        {
            if (override)
            {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }
            QFile::copy(srcFilePath, dstFilePath);
        }
        else if (fileInfo.isDir())
        {
            QDir dstDir(dstFilePath);
            dstDir.mkpath(dstFilePath);
            if (!copyDir(srcFilePath, dstFilePath, override))
            {
                error = true;
            }
        }
    }
    return !error;
}
bool copyFile(const QString &source, const QString &destination, const QString &replacestr){
    QFile src(source);
    QFile des(destination);
    if(src.open(QFile::ReadOnly)){
        QTextStream in(&src);
        in.setCodec("UTF-8");
        QString context=in.readAll();
        context.replace("Template",replacestr);
        if(!des.exists())
            des.open(QIODevice::ReadWrite|QIODevice::Text);
        QTextStream out(&des);
        out.setCodec("UTF-8");
        out<<context;
        out.flush();
    } else{
        return false;
    }
    return true;
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    settings.beginGroup("CreateProject");
    for(QString key:settings.allKeys()){
        if(key=="projectPath"){
            config.setProjectPath(settings.value("projectPath").toString());
        }else{
            config.setStringlist(settings.value("list").toStringList());
        }
    }
    //    settings.clear();
    settings.endGroup();
    ui->setupUi(this);
    this->setWindowTitle("Create Stm32 Project");
    this->setWindowIcon(QIcon (":/e.png"));
    ui->pathComboBox->addItems(config.getStringlist());
    if(config.getProjectPath()!=""){
        ui->pathComboBox->setCurrentText(config.getProjectPath());
        ui->pathCheckBox->setChecked(true);
    }
    ui->imageLabel->setPixmap(QPixmap(":/e.png"));
    ui->imageLabel->setScaledContents(true);
    ui->createButton->setEnabled(false);
    this->setPalette(QPalette(QColor(174, 174, 174)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_createButton_clicked()
{
    QString path=ui->pathComboBox->currentText();
    QDir projects{path};
    if(projects.exists()){
        toCopy("template",path);
    }else{
        if(QMessageBox::question(this,"Question",
                                 "The folder does not exist, whether to create a new one?",
                                 QMessageBox::Ok|QMessageBox::Cancel)
                ==QMessageBox::Ok){
            projects.mkpath(path);
            toCopy("template",path);
        }
    }
    settings.beginGroup("CreateProject");
    if(!config.indexOf(path)){
        config.addString(path);
        settings.setValue("list",config.getStringlist());
    }
    if(ui->pathCheckBox->isChecked()){
        settings.setValue("projectPath",path);
    }
    settings.endGroup();
    this->close();
}

void MainWidget::on_pathButton_clicked()
{
    QString path=config.getProjectPath();
    if(path==""){
        path=QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0);
    }
    QString dir=QFileDialog::getExistingDirectory(this,tr("Open Directory"),path,
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir!=""){
        ui->pathComboBox->clear();
        ui->pathComboBox->addItems(config.getStringlist());
        ui->pathComboBox->addItem(dir);
        ui->pathComboBox->setCurrentText(dir);
    }
}

void MainWidget::on_nameLineEdit_textChanged(const QString &arg1)
{
    if(arg1==""){
        ui->createButton->setEnabled(false);
    }else{
        ui->createButton->setEnabled(true);
    }
}

void MainWidget::on_pathCheckBox_stateChanged(int arg1)
{
    if(arg1){
        config.setProjectPath(ui->pathComboBox->currentText());
        ui->pathButton->setEnabled(false);
        ui->pathComboBox->setEnabled(false);
    }else{
        ui->pathButton->setEnabled(true);
        ui->pathComboBox->setEnabled(true);
    }
}
void MainWidget::toCopy(const QString &source, const QString &destination)
{
    QString name=ui->nameLineEdit->text();
    QDir file(source);
    if(file.exists()){
        copyDir(source+"/file",destination+"/"+name,true);
        copyFile(source+"/Template.uvguix.Moon",destination+"/"+name+"/USER/"+name+".uvguix.Moon",name);
        copyFile(source+"/Template.map",destination+"/"+name+"/USER/Listings/"+name+".map",name);
        copyFile(source+"Template.uvoptx",destination+"/"+name+"/USER/"+name+".uvoptx",name);
        copyFile(source+"/Template.uvprojx",destination+"/"+name+"/USER/"+name+".uvprojx",name);
        if(QMessageBox::question(this,"","A new project has been created. Do you want to open the project?",QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Ok){
            system(QString{"start "+destination+"/"+name+"/USER/"+name+".uvprojx"}.toStdString().data());
        }
    }else{
        QMessageBox::warning(this,"Warning","The project template does not exist.",QMessageBox::Ok);
    }
}

void MainWidget::on_nameLineEdit_returnPressed()
{
    ui->createButton->click();
}

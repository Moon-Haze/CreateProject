#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<Qt>
#include<QCheckBox>
#include"config.h"
#include<QDebug>
#include<QFileDialog>
#include<QString>
#include<QLineEdit>
#include<QComboBox>
#include<QStringList>
#include<QSettings>
#include<QCloseEvent>
#include<QFile>
#include<QMessageBox>
#include<QDir>
#include<QFileInfo>
#include<QTextStream>
#include<QStandardPaths>
#include<iostream>
#include<string>
#include<QIcon>
#include<QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void toCopy(const QString &source, const QString &destination);
private slots:
    void on_createButton_clicked();
    void on_pathButton_clicked();
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_pathCheckBox_stateChanged(int arg1);

    void on_nameLineEdit_returnPressed();

private:
    Ui::Widget *ui;
    Config config;
    QSettings settings{"Swx","CreateProject"};
};
#endif // WIDGET_H

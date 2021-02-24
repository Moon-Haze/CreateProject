//
// Created by Moon on 2021/2/1.
//

#include <QTextStream>
#include <QFile>
#include <utility>
#include "ProjectConfig.h"
#include <QDebug>

QString ProjectConfig::readFile(const QString &source) {
    QFile src(source);
    QString context;
    if (src.open(QFile::ReadOnly)) {
        QTextStream in(&src);
        in.setCodec("utf-8");
        context.append(in.readAll());
    }
    return context;
}

ProjectConfig::ProjectConfig(QObject *object) : QObject(object) {}


ProjectConfig::ProjectConfig(QString value) : name(std::move(value)) {}

void ProjectConfig::addFile(const QString &string, bool isOpen) {
    openFiles.append(string.toLower());
    if (isOpen) {
        files.append(string.toLower());
    }
}

void ProjectConfig::removeFile(const QString &string) {
    openFiles.removeOne(string.toLower());
    files.append(string.toLower());
}


QString ProjectConfig::mainToQString() const {
    QString context{"#include \"stm32f10x.h\"\n"
                    "#include \"delay.h\"\n"
                    "#include \"sys.h\"\n"};
    for (const QString &item:files) {
        context.append("#include \"" + item.toLower() + ".h\"\n");
    }
    context.append("\nint main(void){\n\n\twhile(1){\n\n\t}\n}\n");

    return context;
}

QString ProjectConfig::uvprojxToQString(const QString &source) const {
    QString context = readFile(source);
    context.replace("TEMPLATE", name.toUpper());
    context.replace("Template", name);
    QString uvprojxStr = "<Group><GroupName>HARDWARE</GroupName><Files>";
    for (const auto &item :files) {
        uvprojxStr.append("<File><FileName>");
        uvprojxStr.append(item);
        uvprojxStr.append(".c</FileName><FileType>1</FileType><FilePath>..\\HARDWARE\\");
        uvprojxStr.append(item);
        uvprojxStr.append(".c</FilePath></File>");
    }
    uvprojxStr.append("</Files></Group>");
    context.replace("<Group><GroupName>HARDWARE</GroupName></Group>", uvprojxStr);

    return context;
}

QString ProjectConfig::uvoptxToQString(const QString &source) const {
    QString context = readFile(source);
    context.replace("TEMPLATE", name.toUpper());
    context.replace("Template", name);
    QString moonStr = "<Group>\n"
                      "<GroupName>HARDWARE</GroupName>\n"
                      "<tvExp>1</tvExp>\n"
                      "<tvExpOptDlg>0</tvExpOptDlg>\n"
                      "<cbSel>0</cbSel>\n"
                      "<RteFlg>0</RteFlg>\n";
    for (const QString &item : files) {
        int i = 32;
        if (item != "main") {
            moonStr.append("<File>\n<GroupNumber>5</GroupNumber>\n<FileNumber>");
            moonStr.append(48 + (i++));
            moonStr.append("</FileNumber>\n"
                           "<FileType>1</FileType>\n"
                           "<tvExp>0</tvExp>\n"
                           "<Focus>0</Focus>\n"
                           "<tvExpOptDlg>0</tvExpOptDlg>\n"
                           "<bDave2>0</bDave2>\n<PathWithFileName>..\\HARDWARE\\");
            moonStr.append(item);
            moonStr.append(".c</PathWithFileName>\n"
                           "<FilenameWithoutPath>LED.c</FilenameWithoutPath>\n"
                           "<RteFlg>0</RteFlg>\n"
                           "<bShared>0</bShared>\n"
                           "</File>");
        }
    }
    moonStr.append("</Group>");
    context.replace("<Group><GroupName>HARDWARE</GroupName></Group>", moonStr);

    return context;
}

QString ProjectConfig::moonToQString(const QString &source) const {
    QString context = readFile(source);
    context.replace("TEMPLATE", name.toUpper());
    context.replace("Template", name);

    /*
       <MDIGroups>
    <Orientation>1</Orientation>
    <ActiveMDIGroup>0</ActiveMDIGroup>
    <MDIGroup>
      <Size>100</Size>
      <ActiveTab>0</ActiveTab>
      <Doc>
        <Name>..\SYSTEM\usart\usart.c</Name>
        <ColumnNumber>0</ColumnNumber>
        <TopLine>1</TopLine>
        <CurrentLine>1</CurrentLine>
        <Folding>1</Folding>
        <ContractedFolders></ContractedFolders>
        <PaneID>0</PaneID>
      </Doc>
      <Doc>
        <Name>..\SYSTEM\delay\delay.c</Name>
        <ColumnNumber>0</ColumnNumber>
        <TopLine>1</TopLine>
        <CurrentLine>1</CurrentLine>
        <Folding>1</Folding>
        <ContractedFolders></ContractedFolders>
        <PaneID>0</PaneID>
      </Doc>
    </MDIGroup>
  </MDIGroups>
     * */
    QString uvoptxStr =
            "<MDIGroups>\n"
            "<Orientation>1</Orientation>\n"
            "<ActiveMDIGroup>0</ActiveMDIGroup>"
            "<MDIGroup>\n"
            "<Size>100</Size>\n"
            "<ActiveTab>0</ActiveTab>\n"
            "<Doc>\n"
            "<Name>.\\main.c</Name>\n"
            "<ColumnNumber>0</ColumnNumber>\n"
            "<TopLine>1</TopLine>\n"
            "<CurrentLine>1</CurrentLine>\n"
            "<Folding>1</Folding>\n"
            "<ContractedFolders></ContractedFolders>\n"
            "<PaneID>0</PaneID>\n"
            "</Doc>\n";
    for (const QString &item : openFiles) {
        int i = 1;
        if (item != "main") {
            uvoptxStr.append("<Doc>\n"
                             "<Name>..\\HARDWARE\\" + item + ".h</Name>\n" +
                             "<ColumnNumber>");
            uvoptxStr.append(48 + (i++));
            uvoptxStr.append("</ColumnNumber>\n"
                             "<TopLine>1</TopLine>\n"
                             "<CurrentLine>1</CurrentLine>\n"
                             "<Folding>1</Folding>\n"
                             "<ContractedFolders></ContractedFolders>\n"
                             "<PaneID>0</PaneID>\n"
                             "</Doc>\n");
            uvoptxStr.append("<Doc>\n"
                             "<Name>..\\HARDWARE\\" + item + ".c</Name>\n" +
                             "<ColumnNumber>");
            uvoptxStr.append(48 + (i++));
            uvoptxStr.append("</ColumnNumber>\n"
                             "<TopLine>1</TopLine>\n"
                             "<CurrentLine>1</CurrentLine>\n"
                             "<Folding>1</Folding>\n"
                             "<ContractedFolders></ContractedFolders>\n"
                             "<PaneID>0</PaneID>\n"
                             "</Doc>\n");
        }
    }
    uvoptxStr.append("</MDIGroup></MDIGroups>");
    context.replace("<MDIGroups><MDIGroup><Size>100</Size><ActiveTab>2</ActiveTab></MDIGroup></MDIGroups>",
                    uvoptxStr);

    return context;
}

const QString &ProjectConfig::getName() const {
    return name;
}

void ProjectConfig::setName(const QString &value) {
    ProjectConfig::name = value;
}


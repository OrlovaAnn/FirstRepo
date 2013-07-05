#include "dialog.h"
#include "ui_dialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QPushButton>

#include <QTextCodec>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    isEncrypt = true;

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    fileDialog = new QFileDialog(this);
    statusBar = new QStatusBar(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* barLayout = new QHBoxLayout;
    QHBoxLayout* textLayout = new QHBoxLayout;
    labelForPixmap = new QLabel(this);
    QPixmap pixmap(":images/arrow");
    labelForPixmap->setPixmap(pixmap);

    createActions();
    createMenus();
    createConnections();

    barLayout->addWidget(bar);
    textLayout->addWidget(ui->textFrom);
    textLayout->addWidget(labelForPixmap);
    textLayout->addWidget(ui->textTo);
    mainLayout->addLayout(barLayout);
    mainLayout->addLayout(textLayout);
    mainLayout->addWidget(statusBar);
    this->setLayout(mainLayout);

    secDialog = new SecondaryDialog(this);

    statusBar->showMessage("...");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::openF()
{
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open text file"), ".", tr("Text files (*.txt)"));

    if(!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}

bool Dialog::save()
{
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save encrypted text"), ".", tr("Text files (*.txt)"));
    QFile mFile(fileName);
    if(!mFile.open(QFile::ReadWrite))
    {
        QMessageBox msgBox;
        msgBox.setText("Can not open file!");
        msgBox.exec();
        return false;
    }
    QTextStream out(&mFile);
    out << ui->textTo->toPlainText();
    mFile.close();
    return true;
}

void Dialog::encrypt()
{
    secDialog->show();
    isEncrypt = true;
    connect(secDialog, SIGNAL(closeDialog()), this, SLOT(translateText()));
}

void Dialog::decrypt()
{
    secDialog->show();
    isEncrypt = false;
    connect(secDialog, SIGNAL(closeDialog()), this, SLOT(translateText()));
}

void Dialog::createActions()
{
    actionOpen = new QAction("Open text for encryption..", this);
    actionOpen->setIcon(QIcon(":images/open"));
    actionSave = new QAction("Save encrypted text...", this);
    actionSave->setIcon(QIcon(":images/save"));
    actionEncrypt = new QAction("Encrypt...", this);
    actionEncrypt->setIcon(QIcon(":images/encrypt"));
    actionDecrypt = new QAction("Decrypt...", this);
    actionDecrypt->setIcon(QIcon(":images/decrypt"));
    actionExit = new QAction("Exit", this);
    actionExit->setIcon(QIcon(":images/exit"));
}


void Dialog::createMenus()
{
    bar = new QMenuBar(this);
    file = new QMenu("File");
    bar->addMenu(file);
    file->addAction(actionOpen);
    file->addAction(actionSave);
    file->addAction(actionEncrypt);
    file->addAction(actionDecrypt);
    file->addSeparator();
    file->addAction(actionExit);
}

void Dialog::createConnections()
{
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openF()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionEncrypt, SIGNAL(triggered()), this, SLOT(encrypt()));
    connect(actionDecrypt, SIGNAL(triggered()), this, SLOT(decrypt()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

bool Dialog::loadFile(const QString &fileName)
{
    QFile mFile(fileName);

    if(!mFile.open(QFile::ReadWrite))
    {
        QMessageBox msgBox;
        msgBox.setText("Can not open file for reading!");
        msgBox.exec();
        return false;
    }
    QTextStream in(&mFile);

    QString mText = in.readAll();
    ui->textFrom->clear();
    ui->textFrom->setText(mText);
    mFile.close();
    return true;
}

void Dialog::translateText()
{
    statusBar->showMessage(secDialog->getMessageForStatusBar());
    textFromStr.clear();
    textToStr.clear();
    ui->textTo->clear();

    textFromStr = ui->textFrom->toPlainText();

    if(textFromStr.isEmpty())
        return;

    int encrLength = secDialog->getEncryptText().length();

    if ( encrLength == 0 )
        return;

    for( int i = 0; i < textFromStr.length(); ++i)
    {
        ushort chCr = secDialog->getEncryptText().at(i % encrLength).unicode();
        ushort ch = textFromStr.at(i).unicode();
        if ( isEncrypt )
            ch = ch + chCr;
        else
            ch = ch - chCr;
        textToStr.push_back(QChar(ch));
    }
    ui->textTo->setText(textToStr);

}

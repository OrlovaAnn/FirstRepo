#include "secondarydialog.h"
#include "ui_secondarydialog.h"


#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextStream>
#include <QPushButton>
#include <QTextCodec>
#include <QFileDialog>

SecondaryDialog::SecondaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryDialog)
{
    ui->setupUi(this);
    initLayer();
    createConnections();
    isText = true;
    isImage = false;
    okToContinue = false;
}

SecondaryDialog::~SecondaryDialog()
{
    delete ui;
}


void SecondaryDialog::initLayer()
{

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* firstLineLayout = new QHBoxLayout;
    QHBoxLayout* secondLineLayout = new QHBoxLayout;
    QHBoxLayout* thirdLineLayout = new QHBoxLayout;

    ok = new QPushButton("OK");
    cancel = new QPushButton("Cancel");
    loadImageButton = new QPushButton("Загрузить..");
    textRadioButton = new QRadioButton("Введите кодовую фразу : ");
    imageRadioButton = new QRadioButton("Выберите изображение : ");

    lineEdit = new QLineEdit();

    mainLayout->addLayout(firstLineLayout);
    firstLineLayout->addWidget(textRadioButton);
    firstLineLayout->addWidget(lineEdit);

    mainLayout->addLayout(secondLineLayout);

    secondLineLayout->addWidget(imageRadioButton);
    secondLineLayout->addWidget(loadImageButton);

    thirdLineLayout->addWidget(ok);
    thirdLineLayout->addWidget(cancel);

    mainLayout->addLayout(thirdLineLayout);
    this->setLayout(mainLayout);

    textRadioButton->setChecked(true);
    imageRadioButton->setChecked(false);
    loadImageButton->setEnabled(false);
}

void SecondaryDialog::createConnections()
{
    connect(ok, SIGNAL(clicked()), this, SLOT(saveInfo()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(textRadioButton, SIGNAL(clicked(bool)), this, SLOT(disableImage(bool)));
    connect(imageRadioButton, SIGNAL(clicked(bool)), this, SLOT(disableText(bool)));
    connect(loadImageButton, SIGNAL(clicked()), this, SLOT(loadImage()));
}


void SecondaryDialog::saveInfo()
{
    if(isImage)
    {
        if(encryptText.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("No image was choosen!");
            msgBox.exec();
            return;
        }
    }

    if(isText)
    {
        encryptText = lineEdit->text();
        messageForStatusBar = "Code phrase : " + encryptText;
        if(encryptText.isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("Empty edit line word!");
            msgBox.exec();
            return;
        }
    }

    okToContinue = true;
    this->close();
    emit closeDialog();
}


void SecondaryDialog::disableImage(bool flag)
{
    isImage = false;
    isText = true;
    loadImageButton->setDisabled(flag);
    lineEdit->setEnabled(flag);
}

void SecondaryDialog::disableText(bool flag)
{
    isImage = true;
    isText = false;
    lineEdit->setDisabled(flag);
    loadImageButton->setEnabled(flag);
}

void SecondaryDialog::loadImage()
{
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), ".", tr("Images (*.jpeg)"));
    QImage image;
    encryptText.clear();
    image.load(fileName);
    messageForStatusBar = "Image path : " + fileName;

    if(!fileName.isEmpty() || image.isNull())
    {
        for (int i = 0; i < image.width(); ++i)
        {
            for (int j = 0; j < image.height(); ++j)
            {
                QRgb pixel = image.pixel(i,j);
                ushort value = pixel % 10;
                encryptText.push_back(QChar(value));
            }
        }
    }
}


QString SecondaryDialog::getMessageForStatusBar() const
{
    return this->messageForStatusBar;
}


QString SecondaryDialog::getEncryptText() const
{
    return this->encryptText;
}



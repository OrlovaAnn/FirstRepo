#ifndef SECONDARYDIALOG_H
#define SECONDARYDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QLabel>
#include <QImage>
#include <QLineEdit>

namespace Ui {
class SecondaryDialog;
}

class SecondaryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SecondaryDialog(QWidget *parent = 0);
    ~SecondaryDialog();

    bool okToContinue;

    QString getMessageForStatusBar() const;
    QString getEncryptText() const;

signals:
    void closeDialog();

private slots:
    void saveInfo();
    void disableImage(bool);
    void disableText(bool);
    void loadImage();

private:
    Ui::SecondaryDialog *ui;

    QPushButton* ok;
    QPushButton* cancel;
    QRadioButton* textRadioButton;
    QRadioButton* imageRadioButton;
    QLabel* textLabel;
    QLabel* imageLabel;
    QLineEdit* lineEdit;
    QPushButton* loadImageButton;

    QString encryptText;
    QString messageForStatusBar;
    bool isText;
    bool isImage;


    void createConnections();
    void initLayer();


};

#endif // SECONDARYDIALOG_H

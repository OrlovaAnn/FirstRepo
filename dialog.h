#ifndef DIALOG_H
#define DIALOG_H

#include "secondarydialog.h"
#include <QDialog>

#include <QFileDialog>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QPixmap>

namespace Ui {
class Dialog;
}


class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    SecondaryDialog *secDialog;

private slots:
    void openF();
    bool save();
    void encrypt();
    void decrypt();
    void translateText();

private:
    Ui::Dialog *ui;

    void createActions();
    void createMenus();
    void createConnections();
    bool loadFile(const QString &fileName);

    QFileDialog* fileDialog;
    QAction* actionNew;
    QAction* actionOpen;
    QAction* actionSave;
    QAction* actionEncrypt;
    QAction* actionExit;
    QAction* actionDecrypt;
    QMenuBar* bar;
    QMenu* file;
    QStatusBar* statusBar;
    //QPixmap pixmap;
    QLabel* labelForPixmap;

    bool isEncrypt;

    QString textFromStr;
    QString textToStr;


};


#endif // DIALOG_H

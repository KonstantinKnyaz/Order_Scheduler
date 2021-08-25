#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QTableView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Заказы");

    ui->btnAdd->connect(ui->btnAdd, &QPushButton::clicked, [this]() {
       addNewOrder();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
    this->deleteLater();
}

void MainWindow::addNewOrder()
{
    Ui::formEdit* formUi = new Ui::formEdit;
    QWidget* form = new QWidget();
    formUi->setupUi(form);
    form->setWindowTitle("Добавить");
    formUi->dateEdit->setDateTime(QDateTime::currentDateTime());

    QLineEdit* nameLine = formUi->nameEdit;
    QLineEdit* phoneLine = formUi->phoneEdit;
    QLineEdit* productLine = formUi->productEdit;
    QTextEdit* descripLine = formUi->orderDesc;
    QDateEdit* dateLine = formUi->dateEdit;

    QString name = nameLine->text();
    QString phone = phoneLine->text();
    QString product = productLine->text();
    QString desc = descripLine->toPlainText();
    //formUi->orderDesc->toPlainText();
    QString date = dateLine->text();
    formUi->editBtn->connect(formUi->canselBtn, &QPushButton::clicked, [=]() {

    });


    formUi->canselBtn->connect(formUi->canselBtn, &QPushButton::clicked, [=]() {
        form->close();
    });

    form->show();
}


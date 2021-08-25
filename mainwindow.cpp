#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QTableView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      model(Q_NULLPTR),
      proxyModel(Q_NULLPTR)
{
    ui->setupUi(this);
    setWindowTitle("Планировщик заказов");

    model = new tableModel(this);
    proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->btnAdd->connect(ui->btnAdd, &QPushButton::clicked, [this]() {
       addNewOrder();
    });

    ui->btnEdit->connect(ui->btnEdit, &QPushButton::clicked, [=]() {

    });

    ui->btnDel->connect(ui->btnDel, &QPushButton::clicked, [=]() {
        if(!ui->tableView->currentIndex().isValid()) return;

        QString data = model->index(ui->tableView->currentIndex().row(), 0, ui->tableView->currentIndex()).data().toString();

        QMessageBox msgBox(
                                QMessageBox::Question,
                                QString::fromUtf8("Удаление"),
                                QString::fromUtf8("Удалить %1?").arg(data),
                                QMessageBox::Yes | QMessageBox::No
                                );
                    QApplication::setApplicationDisplayName(msgBox.windowTitle());
                    msgBox.setButtonText(QMessageBox::Yes,"Да");
                    msgBox.setButtonText(QMessageBox::No, "Отмена");
                    msgBox.setIcon(QMessageBox::Question);
                    int r = msgBox.exec();
                    if(r == QMessageBox::Yes ){
                        model->remove(ui->tableView->currentIndex());
                    }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
    this->deleteLater();
    delete model;
    delete proxyModel;
}

void MainWindow::addNewOrder()
{
    Ui::formEdit* formUi = new Ui::formEdit;
    QDialog* form = new QDialog();
    formUi->setupUi(form);
    form->setWindowTitle("Добавление");
    formUi->dateEdit->setDateTime(QDateTime::currentDateTime());

    formUi->editBtn->connect(formUi->editBtn, &QPushButton::clicked, [this, form, formUi]() {

        QString name = formUi->nameEdit->text();
        QString phone = formUi->phoneEdit->text();
        QString product = formUi->phoneEdit->text();
        QString desc = formUi->orderDesc->toPlainText();
        QString date = formUi->dateEdit->text();

        model->add(dataModel(name, phone, product, desc, date));

        form->accept();
    });

    formUi->canselBtn->connect(formUi->canselBtn, &QPushButton::clicked, [=]() {
        form->reject();
    });

    form->exec();
    delete formUi;
    form->deleteLater();

}

void MainWindow::editOrder(const QModelIndex &index)
{

}



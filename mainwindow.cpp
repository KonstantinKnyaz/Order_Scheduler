#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDialog>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QTableView>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>

const QString MainWindow::fileName("ordersData.json");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      model(Q_NULLPTR),
      proxyModel(Q_NULLPTR)
{
    ui->setupUi(this);
    setWindowTitle("Планировщик заказов Beta 1.0.0");

    model = new tableModel(this);
    //model->setRowCount(values.count());
    model->setColumnCount(5);
    proxyModel = new QSortFilterProxyModel(this);

    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    if (loadFile()) {

        QString date = QDate::currentDate().toString(Qt::ISODate);

        for(int i = 0; i < model->values.count(); i++){

            qDebug() << date;

            QString dateTb = model->index(i, 4, ui->tableView->currentIndex()).data().toString();

            QStringList dt = date.split("-");
            QStringList dtb = dateTb.split("-");

            if (dt.at(1) == dtb.at(1)) {
                int dateLess = dtb.at(2).toInt() - dt.at(2).toInt();
                if (dateLess <= 3) {
                QMessageBox msgBox(
                    QMessageBox::Information,
                    QString::fromUtf8("Уведомление"),
                    QString::fromUtf8("У %1 осталось %2 дня").arg(model->index(i, 0, ui->tableView->currentIndex()).data().toString()).arg(dateLess)
                );
                msgBox.setIcon(QMessageBox::Information);
                msgBox.exec();
                }
            }

            qDebug() << dateTb;

        }
    }

    ui->btnAdd->connect(ui->btnAdd, &QPushButton::clicked, [this]() {
       addNewOrder(); 
    });

    ui->btnEdit->connect(ui->btnEdit, &QPushButton::clicked, [this]() {
        editOrder(ui->tableView->currentIndex());
    });

    ui->btnDel->connect(ui->btnDel, &QPushButton::clicked, [this]() {
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

    ui->searchEdit->connect(ui->searchEdit, &QLineEdit::textChanged, [=]() {
        if (ui->nameRadioBtn->isChecked())
            proxyModel->setFilterKeyColumn(0);
        else if (ui->phoneRadioBtn->isChecked())
            proxyModel->setFilterKeyColumn(1);

        proxyModel->setFilterWildcard(ui->searchEdit->text());
    });

    ui->info->connect(ui->info, &QAction::triggered, [](){
        QMessageBox msgBox(
            QMessageBox::Information,
            QString::fromUtf8("Информация"),
            QString::fromUtf8("Автор и исполнитель программы \n Константин Князев")
        );
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
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
        QString product = formUi->productEdit->text();
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
    if(!index.isValid()) return;
    Ui::formEdit* formUi = new Ui::formEdit;
    QDialog* form = new QDialog();
    formUi->setupUi(form);
    form->setWindowTitle("Редактирование");
    formUi->editBtn->setText("Изменить");
    formUi->dateEdit->setDateTime(QDateTime::currentDateTime());

    formUi->nameEdit->setText(model->index(ui->tableView->currentIndex().row(), 0, ui->tableView->currentIndex()).data().toString());
    formUi->phoneEdit->setText(model->index(ui->tableView->currentIndex().row(), 1, ui->tableView->currentIndex()).data().toString());
    formUi->productEdit->setText(model->index(ui->tableView->currentIndex().row(), 2, ui->tableView->currentIndex()).data().toString());
    formUi->orderDesc->setText(model->index(ui->tableView->currentIndex().row(), 3, ui->tableView->currentIndex()).data().toString());
    formUi->dateEdit->setDate(model->index(ui->tableView->currentIndex().row(), 4, ui->tableView->currentIndex()).data().toDate());

    formUi->editBtn->connect(formUi->editBtn, &QPushButton::clicked, [this, form, formUi, index]() {

        QString name = formUi->nameEdit->text();
        QString phone = formUi->phoneEdit->text();
        QString product = formUi->productEdit->text();
        QString desc = formUi->orderDesc->toPlainText();
        QString date = formUi->dateEdit->text();

        model->update(index, dataModel(name, phone, product, desc, date));

        form->accept();
    });

    formUi->canselBtn->connect(formUi->canselBtn, &QPushButton::clicked, [=]() {
        form->reject();
    });

    form->exec();
    delete formUi;
    form->deleteLater();
}

void MainWindow::saveToFile()
{
    QFile saveFile(fileName);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            qWarning("Couldn't open save file.");
            return;
        }

        QJsonObject textObject;
        for (int i = 0; i < model->values.count(); i++) {
        textObject["name"] = model->index(i,0,ui->tableView->currentIndex()).data().toString();
        textObject["phone"] = model->index(i,1,ui->tableView->currentIndex()).data().toString();
        textObject["order"] = model->index(i,2,ui->tableView->currentIndex()).data().toString();
        textObject["desc"] = model->index(i,3,ui->tableView->currentIndex()).data().toString();
        textObject["date"] = model->index(i,4,ui->tableView->currentIndex()).data().toString();
        QJsonArray textsArray = m_currentJsonObject["data"].toArray();
        textsArray.append(textObject);
        m_currentJsonObject["data"] = textsArray;
        }

        saveFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
        saveFile.close();
}

bool MainWindow::loadFile()
{
    QFile loadFile(fileName);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return false;
        }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument jsnDoc(QJsonDocument::fromJson(saveData));
    QJsonObject root = jsnDoc.object();
    //qDebug() << root.keys().at(0);
    QJsonValue jv = root.value("data");
    if(jv.isArray()) {
        QJsonArray js = jv.toArray();
        for(int i = 0; i < js.count(); i++) {
            QJsonObject subtree = js.at(i).toObject();
            QString name = subtree.value("name").toString();
            QString phone = subtree.value("phone").toString();
            QString order = subtree.value("order").toString();
            QString desk = subtree.value("desc").toString();
            QString date = subtree.value("date").toString();
            model->add(dataModel(name, phone, order, desk, date));
        }
    }
   // qDebug() << m_currentJsonObject;
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox(
        QMessageBox::Question,
        QString::fromUtf8("Завершение работы"),
        QString::fromUtf8("Выход из приложения"),
        QMessageBox::Yes | QMessageBox::No
    );
    msgBox.setButtonText(QMessageBox::Yes, ("Выйти"));
    msgBox.setButtonText(QMessageBox::No, ("Отмена"));
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    int r = msgBox.exec();
    if(r == QMessageBox::No )
    {
        event->ignore();
        return;
    } else if (r == QMessageBox::Yes) {
        saveToFile();
        qApp->processEvents(QEventLoop::AllEvents, 5000);
        event->accept();
    }
}



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
#include <QRegularExpressionValidator>
#include <QApplication>
#include <QScreen>
#include <QTextStream>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>

const QString MainWindow::fileName("ordersData.json");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      model(Q_NULLPTR),
      proxyModel(Q_NULLPTR),
      m_about(Q_NULLPTR),
      aboutFlag(false)
{
    ui->setupUi(this);
    setWindowTitle("Планировщик заказов Build 1.0.1");

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
        for(int i = 0; i < model->values.count(); i++){

            QDate date = QDate::currentDate();
            QDate dateTable = model->index(i, 4, ui->tableView->currentIndex()).data().toDate();
            quint64 days = date.daysTo(dateTable);

            if (days <= 3) {
            QMessageBox msgBox(
                QMessageBox::Information,
                QString::fromUtf8("Уведомление"),
                QString::fromUtf8("У %1 осталось %2 дня").arg(model->index(i, 0, ui->tableView->currentIndex()).data().toString()).arg(days)
            );
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
            ui->tableView->setItemDelegateForRow(i, new CustomDelegate(ui->tableView));
            }
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
                        lastDays();
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

    ui->tableView->connect(ui->tableView, &QTableView::doubleClicked, [this](){
        getAbout();
    });

    ui->printBtn->connect(ui->printBtn, &QAction::triggered, [this](){
        printFile();
    });

    ui->pdfBtn->connect(ui->pdfBtn, &QAction::triggered, [this](){
        printPdfFile();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    this->deleteLater();
    delete model;
    delete proxyModel;
    m_about->deleteLater();
}

void MainWindow::addNewOrder()
{
    Ui::formEdit* formUi = new Ui::formEdit;
    QDialog* form = new QDialog();
    formUi->setupUi(form);
    form->setWindowTitle("Добавление");
    formUi->dateEdit->setDateTime(QDateTime::currentDateTime());
    QRegularExpression rx("[0-9+]+");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    formUi->phoneEdit->setValidator(validator);

    formUi->editBtn->connect(formUi->editBtn, &QPushButton::clicked, [this, form, formUi]() {

        QString name = formUi->nameEdit->text();
        QString phone = formUi->phoneEdit->text();
        QString product = formUi->productEdit->text();
        QString desc = formUi->orderDesc->toPlainText();
        QString date = formUi->dateEdit->text();

        model->add(dataModel(name, phone, product, desc, date));
        lastDays();
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
    QRegularExpression rx("[0-9+]+");
    QValidator *validator = new QRegularExpressionValidator(rx, this);
    formUi->phoneEdit->setValidator(validator);

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
        lastDays();
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
        QJsonObject m_currentJsonObject;
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
    loadFile.close();
    return true;
}

void MainWindow::lastDays()
{
    for(int i = 0; i < model->values.count(); i++){

        QDate date = QDate::currentDate();
        QDate dateTable = model->index(i, 4, ui->tableView->currentIndex()).data().toDate();
        quint64 days =  date.daysTo(dateTable);

        if (days <= 3)
            ui->tableView->setItemDelegateForRow(i, new CustomDelegate(ui->tableView));
        else
            ui->tableView->setItemDelegateForRow(i, new CustomDelegateWhite(ui->tableView));
    }
}

void MainWindow::getAbout()
{
    if(aboutFlag == true) return;

    m_about = new QFrame(ui->tableView);
    m_about->setObjectName("aboutUs");

QString data = model->index(ui->tableView->currentIndex().row(), 3, ui->tableView->currentIndex()).data().toString();

    m_about->setWindowTitle("Данные о заказе");
    m_about->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    m_about->setFrameShape(QFrame::Box);
    m_about->installEventFilter(this);

    m_about->resize(500,300);
    m_about->setLayout(new QGridLayout(m_about));
    QLabel *lbl = new QLabel(QString("Информация: %1").arg(data),m_about);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_about->layout()->addWidget(lbl);

    m_about->show();
    aboutFlag = true;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_about) {
        if (event->type() == QEvent::MouseButtonPress) {
            m_about->setParent(Q_NULLPTR);
            m_about->close();
            m_about->removeEventFilter(this);
            aboutFlag = false;
            return true;
        } else
            return false;
    }
    return MainWindow::eventFilter(watched, event);
}

void MainWindow::printFile()
{
    saveToFile();

    QFile file("printForm.html");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QString date = QDate::currentDate().toString(Qt::ISODate);

    QTextStream stream(&file);
    stream << "<html><head></head><body><center>"+QString("Список заказов на %1").arg(date);
    stream << "<table border=1><tr>";
    stream << "<td>"+QString("ФИО")+"</td>";
    stream << "<td>"+QString("Телефон")+"</td>";
    stream << "<td>"+QString("Заказ")+"</td>";
    stream << "<td>"+QString("Описание")+"</td>";
    stream << "<td>"+QString("Дата")+"</td></tr>";

    QFile loadFile(fileName);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return;
        }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument jsnDoc(QJsonDocument::fromJson(saveData));
    QJsonObject root = jsnDoc.object();
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
            stream << "<tr><td>";
            stream << name;
            stream << "</td><td>";
            stream << phone;
            stream << "</td><td>";
            stream << order;
            stream << "</td><td>";
            stream << desk;
            stream << "</td><td>";
            stream << date;
            stream << "</td></tr>";
        }
        stream << "</table></center></body></html>";
    }
    loadFile.close();
    file.close();
}

void MainWindow::printPdfFile()
{
    saveToFile();

    QString str;

    QString date = QDate::currentDate().toString(Qt::ISODate);

    str.append( "<html><head></head><body><center>"+QString("Список заказов на %1").arg(date) );
    str.append( "<table border=1><tr>");
    str.append( "<td>"+QString("ФИО")+"</td>");
    str.append( "<td>"+QString("Телефон")+"</td>" );
    str.append( "<td>"+QString("Заказ")+"</td>" );
    str.append( "<td>"+QString("Описание")+"</td>" );
    str.append( "<td>"+QString("Дата")+"</td></tr>" );

    QFile loadFile(fileName);
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return;
        }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument jsnDoc(QJsonDocument::fromJson(saveData));
    QJsonObject root = jsnDoc.object();
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
            str.append( "<tr><td>");
            str.append( name );
            str.append( "</td><td>");
            str.append( phone );
            str.append( "</td><td>" );
            str.append( order );
            str.append( "</td><td>");
            str.append( desk );
            str.append( "</td><td>" );
            str.append( date );
            str.append( "</td></tr>" );
        }
        str.append( "</table></center></body></html>" );
    }
    loadFile.close();

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    //printer.setResolution();
    QString pdfName = QString("Заказы на %1").arg(QDate::currentDate().toString());
    QString path = QFileDialog::getSaveFileName(NULL, "Сохранить в PDF", pdfName, "PDF(*.pdf)");
    if(path.isEmpty()) return;
    printer.setOutputFileName(path);

    QTextDocument doc;
    doc.setHtml(str);
    doc.print(&printer);
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



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QFile>
#include <QSortFilterProxyModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "datamodel.h"
#include "tablemodel.h"
#include "ui_formEdit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
namespace formUi { class formEdit; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addNewOrder();

    void editOrder(const QModelIndex &index);

    void saveToFile();

protected:

private:
    Ui::MainWindow *ui;
    tableModel * model;
    QJsonObject newObj;
    QSortFilterProxyModel *proxyModel;
    void closeEvent(QCloseEvent *event);
    QList<dataModel> values;
    static const QString fileName;
};
#endif // MAINWINDOW_H

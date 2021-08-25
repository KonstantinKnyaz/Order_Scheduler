#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QFile>

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

protected:

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H

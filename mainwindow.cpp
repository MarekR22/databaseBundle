#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbmanager.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbManager = new DbManager(this);
    ui->tableView->setModel(dbManager->imageTableModel());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_insertButton_clicked()
{
    auto path = QFileDialog::getOpenFileName(this, "Image");
    if (path.isEmpty()) return;
    dbManager->addImage(ui->titleEdit->text(), path);
}

void MainWindow::on_deleteButton_clicked()
{
    auto selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.empty()) return;
    ui->tableView->model()->removeRow(selected.first().row());
}

#include "viewer_mainwindow.h"
#include "ui_viewer_mainwindow.h"

Viewer_MainWindow::Viewer_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer_MainWindow)
{
    ui->setupUi(this);
}

Viewer_MainWindow::~Viewer_MainWindow()
{
    delete ui;
}

void Viewer_MainWindow::on_actionOpen_triggered()
{

}

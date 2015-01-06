#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // downloader = Downloader();
    // Downloader d;
    // d.doDownload();
}

MainWindow::~MainWindow()
{
    delete ui;
}

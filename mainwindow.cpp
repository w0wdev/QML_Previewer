#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QML IDE");
    this->setWindowIcon(Util::getIcon("icons/silver_v.svg"));
    reloadPages();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reloadPages()
{
    while (ui->pageWidget->count() != 0) {
        QWidget* widget = ui->pageWidget->widget(0);
        ui->pageWidget->removeWidget(widget);
        widget->deleteLater();
    }
    
    pageScripting = new PageScripting(this);
    ui->pageWidget->addWidget(pageScripting);

    ui->pageWidget->setCurrentIndex(0);

    this->setStyleSheet(
        QString("background-color: %1;")
        .arg(Util::getAppQColor("lightBackground").name())
    );
}

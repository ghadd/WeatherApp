#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::App) {
    ui->setupUi(this);

    ui->cityInfoWidget->setVisible(false);
    ui->weatherInfoScrollArea->setVisible(false);
    currentPlace = nullptr;
}

App::~App() {
    delete ui;
}


void App::on_actionNew_City_triggered()
{
    QString country, city;
}

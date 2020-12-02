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
    Place *place;
    try {
        place = getPlace();
    } catch (const InvalidPlaceException &e) {
        QMessageBox::warning(this, "City not found.", QString::fromLocal8Bit(e.what()));
        return;
    }
    catch (const std::runtime_error &e) {
        /* No user feedback for cancelled input */
        return;
    }

    ui->cityInfoWidget->setVisible(true);
    ui->cityNamingLabel->setText(place->toQString());
    ui->cityCoordinatesLabel->setText(place->coordsQString());
    ui->currentObservingDateEdit->setDate(QDate::currentDate());
}

Place *App::getPlace()
{
    bool ok;
    QString country = QInputDialog::getText(this, "Country", "Enter country: ", QLineEdit::Normal, "", &ok);
    if (!ok)
        throw std::runtime_error("Invalid country");

    QString city = QInputDialog::getText(this, "City", "Enter city in " + country + " :", QLineEdit::Normal , "", &ok);
    if (!ok)
        throw std::runtime_error("Invalid country");

    Place *place = new Place(country, city);
    place->validateSelf();

    return place;
}


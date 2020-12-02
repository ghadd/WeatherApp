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
    try {
        loadPlace(false);
    }  catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "City already exists.", QString::fromLocal8Bit(e.what()));
    }
}

void App::on_actionOpen_City_triggered()
{
    try {
        loadPlace(true);
    }  catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "City was not found.", QString::fromLocal8Bit(e.what()));
    }
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

void App::loadPlace(bool open)
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

    QString folderName = QDir::home().filePath(QString::fromStdString(config::as_string(config::WEATHER_DIRECTORY_FORMAT))
            .arg(place->country())
            .arg(place->city()));

    if (open) {
        if (!QDir(folderName).exists()) {
            QString message = place->toQString() + " is not found in file system. Try adding new city.";

            throw std::runtime_error(message.toLocal8Bit());
        }
    }
    else {
        if (QDir(folderName).exists()) {
            QString message = place->toQString() + " already exists. Try opening it.";

            throw std::runtime_error(message.toLocal8Bit());
        }
        else {
            if (!QDir::home().mkpath(folderName)) {
                throw std::runtime_error("Could not create directory for " + place->toQString().toLocal8Bit());
            }
        }
    }

    currentPlace = place;
    ui->cityInfoWidget->setVisible(true);
    ui->cityNamingLabel->setText(place->toQString());
    ui->cityCoordinatesLabel->setText(place->coordsQString());
    ui->currentObservingDateEdit->setDate(QDate::currentDate());
}

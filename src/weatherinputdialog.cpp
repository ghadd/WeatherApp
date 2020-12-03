#include "weatherinputdialog.h"
#include "ui_weatherinputdialog.h"

WeatherInputDialog::WeatherInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeatherInputDialog)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

WeatherInputDialog::~WeatherInputDialog()
{
    delete ui;
}

Weather WeatherInputDialog::getWeather(QWidget *parent, bool *ok, InvalidPlaceException *e)
{
    WeatherInputDialog *dialog = new WeatherInputDialog(parent);

    const int ret = dialog->exec();
    *ok = !!ret;

    if (!*ok) return Weather::INVALID_WEATHER;


    QDate *date = new QDate(dialog->ui->dateEdit->date());
    Place *place = new Place(dialog->ui->countryLineEdit->text(), dialog->ui->cityLineEdit->text());
    try {
        place->validateSelf();
    }
    catch (const InvalidPlaceException &exc) {
        *e = exc;
        *ok = false;
    }

    WeatherInfo *winfo = new WeatherInfo(
                dialog->ui->temperatureSpinBox->value() + 273.15,
                dialog->ui->pressureSpinBox->value(),
                dialog->ui->humiditySpinBox->value()
                );

    Weather weather(date, place, winfo);

    dialog->deleteLater();
    return weather;
}

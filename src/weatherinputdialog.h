#ifndef WEATHERINPUTDIALOG_H
#define WEATHERINPUTDIALOG_H

#include <QDialog>
#include <QDate>

#include "weather.h"
#include "place.h"
#include "weatherinfo.h"

namespace Ui {
class WeatherInputDialog;
}

class WeatherInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WeatherInputDialog(QWidget *parent = nullptr);
    ~WeatherInputDialog();

    static Weather getWeather(QWidget *parent, bool *ok, InvalidPlaceException *e = nullptr);

private:
    Ui::WeatherInputDialog *ui;
};

#endif // WEATHERINPUTDIALOG_H

#ifndef APP_H
#define APP_H

#include <QDir>
#include <QMainWindow>
#include <QInputDialog>
#include <QWidgetAction>
#include <QMessageBox>

#include "src/place.h"
#include "src/config.h"
#include "src/weather.h"

QT_BEGIN_NAMESPACE
namespace Ui { class App; }
QT_END_NAMESPACE

class App : public QMainWindow {
Q_OBJECT

public:
    App(QWidget *parent = nullptr);

    ~App();

private slots:
    void on_actionNew_City_triggered();

    void on_actionOpen_City_triggered();

private:
    Place *getPlace();
    void loadPlace(bool open);
    void updateWeatherInfo();
    void showWeather();

private:
    Ui::App *ui;
    Place *currentPlace;
    QVector<Weather> currentShownWeather;
};

#endif // APP_H

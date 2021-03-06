#ifndef APP_H
#define APP_H

#include <QDir>
#include <QMainWindow>
#include <QInputDialog>
#include <QWidgetAction>
#include <QMessageBox>
#include <QResizeEvent>
#include <QAbstractButton>
#include <QCloseEvent>
#include <QDateTime>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QChartView>

#include "src/place.h"
#include "src/config.h"
#include "src/weather.h"
#include "src/weatherinputdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class App; }
QT_END_NAMESPACE

class App : public QMainWindow {
Q_OBJECT

public:
    App(QWidget *parent = nullptr);

    ~App();

    void resizeEvent(QResizeEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionNew_City_triggered();

    void on_actionOpen_City_triggered();

    void on_actionAbout_this_project_triggered();

    void on_actionAbout_author_triggered();

    void on_actionClose_city_triggered();

    void on_actionQuit_triggered();

    void on_actionNew_Weather_Record_triggered();

    void on_actionLoad_weather_graph_triggered();

    void on_currentObservingDateEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_actionWarmest_and_Coldest_days_triggered();

private:
    Place *getPlace();
    void loadPlace(bool open);
    void updateWeatherInfo();
    void showWeatherWeek();
    void showWeather(const Weather &weather, size_t idx);

private:
    Ui::App *ui;
    Place *currentPlace;
    QVector<Weather> currentShownWeather;

private:
    QVector<QLabel *> dateLabels;
    QVector<QLabel *> imageLabels;
    QVector<QLabel *> stubLabels;

// Images // renaming needed
private:
    QPixmap pixmap;
    QSize initSize;
    QSize initLabelImageSize;

private:
    QMainWindow *chartWindow;
};

#endif // APP_H

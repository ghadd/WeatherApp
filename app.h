#ifndef APP_H
#define APP_H

#include <QDir>
#include <QMainWindow>
#include <QInputDialog>
#include <QWidgetAction>
#include <QMessageBox>
#include <QResizeEvent>

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

    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_actionNew_City_triggered();

    void on_actionOpen_City_triggered();

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
};

#endif // APP_H

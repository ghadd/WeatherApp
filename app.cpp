#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::App) {
    ui->setupUi(this);

    ui->cityInfoWidget->setVisible(false);
    ui->weatherInfoScrollArea->setVisible(false);
    currentPlace = nullptr;

    dateLabels = {ui->dayDateLabel_1,
                  ui->dayDateLabel_2,
                  ui->dayDateLabel_3,
                  ui->dayDateLabel_4,
                  ui->dayDateLabel_5,
                  ui->dayDateLabel_6,
                  ui->dayDateLabel_7 };

    imageLabels = {ui->dayImageLabel_1,
                   ui->dayImageLabel_2,
                   ui->dayImageLabel_3,
                   ui->dayImageLabel_4,
                   ui->dayImageLabel_5,
                   ui->dayImageLabel_6,
                   ui->dayImageLabel_7 };

    stubLabels = {ui->dayStubLabel_1,
                  ui->dayStubLabel_2,
                  ui->dayStubLabel_3,
                  ui->dayStubLabel_4,
                  ui->dayStubLabel_5,
                  ui->dayStubLabel_6,
                  ui->dayStubLabel_7 };

    initLabelImageSize = QSize(100, 100);
    initSize = this->size();
    pixmap.load(":/images/weather_states/half_sunny_day.png"); // todo

    chartWindow = new QMainWindow(this);
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

    QString city = QInputDialog::getText(this, "City", "Enter city in " + country + ":", QLineEdit::Normal , "", &ok);
    if (!ok)
        throw std::runtime_error("Invalid city");

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
            .arg(place->country(), place->city()));

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

    updateWeatherInfo();
    showWeatherWeek();
}

void App::updateWeatherInfo()
{
    QVector<Weather> weatherWeek;
    QDate endDate = ui->currentObservingDateEdit->date();
    QDate startDate = endDate.addDays(-7);

    for (QDate it = startDate; it != endDate; it = it.addDays(1)) {
        Weather weather = Weather::inPlace(&it, currentPlace);
        weatherWeek.append(weather);
        if (weather.isValid()) {
            weather.save();
        }
    }

    currentShownWeather = weatherWeek;
}

void App::showWeatherWeek()
{
    ui->weatherInfoScrollArea->setVisible(true);

    for (int i = 0; i < 7; i++) {
        showWeather(currentShownWeather[i], i);
    }
}

void App::showWeather(const Weather &weather, size_t idx)
{
    QLabel *date = dateLabels[idx];
    QLabel *image = imageLabels[idx];
    QLabel *stub = stubLabels[idx];

    date->setText(weather.date()->toString());

    image->setPixmap(pixmap.scaled(image->size(), Qt::KeepAspectRatio));
    image->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    stub->setText(weather.winfo()->toQString());
}

void App::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();

    qreal resizeFactor = (qreal)size.width() / (qreal)initSize.width();
    if (resizeFactor)
    {
        for (int i = 0; i < imageLabels.size(); i++)
        {
            imageLabels[i]->setFixedSize(initLabelImageSize * resizeFactor);
            imageLabels[i]->setPixmap(pixmap.scaled(imageLabels[i]->size(), Qt::KeepAspectRatio));
        }
    }
}

void App::closeEvent(QCloseEvent *event)
{
    event->ignore();

    QMessageBox* box = new QMessageBox(QMessageBox::Question,
                                       qApp->applicationName(),
                                       "Are you sure you want to quit?",
                                       QMessageBox::Yes | QMessageBox::No,
                                       this);

    QObject::connect(box->button(QMessageBox::Yes), &QAbstractButton::clicked, qApp, &QApplication::quit);
    QObject::connect(box->button(QMessageBox::No), &QAbstractButton::clicked, box, &QObject::deleteLater);
    QObject::connect(qApp, &QApplication::aboutToQuit, box, &QObject::deleteLater);
    box->show();
}

void App::on_actionAbout_this_project_triggered()
{
    QMessageBox::about(this, "About this app.",
                       "A Qt5 based course work project which is able to monitor weather in any point "
                       "in the world, supporting multiple languages thanks to "
                       "<a href=\"https://openweathermap.org/api\">OpenWeatherMap</a>"
                       " and "
                       "<a href=\"https://opencagedata.com/api\">OpenCage</a> APIs. "
                       "Also the offline mode supports inter- and extrapolating existing "
                       "cached records using polynomial Lagrange formula.");
}

void App::on_actionAbout_author_triggered()
{
    QMessageBox::about(this, "About me.",
                       "My name is Dan. I'm a student of Lviv polytechnic, the faculty of "
                       "Software Engineering. I'm kinda liking to code. Here's my github page: "
                       "<a href=\"https://github.com/ghadd/\">ghadd</a>.");
}

void App::on_actionClose_city_triggered()
{
    if (!currentPlace) {
        QMessageBox::warning(this, "Could not close a place.", "There is no place open at the moment.");
        return;
    }

    ui->cityInfoWidget->setVisible(false);
    ui->weatherInfoScrollArea->setVisible(false);
    currentPlace = nullptr;
}

void App::on_actionQuit_triggered()
{
    closeEvent(new QCloseEvent());
}

void App::on_actionNew_Weather_Record_triggered()
{
    bool ok;
    InvalidPlaceException *exc = nullptr;
    auto weather = WeatherInputDialog::getWeather(this, &ok, exc);

    if (!ok) {
        if (exc) {
            QMessageBox::warning(this, "City not found.", QString::fromLocal8Bit(exc->what()));
            delete exc;
        }
        return;
    } else {
        weather.save();
        QMessageBox::information(this, "Success.", "Weather record was added successfully.");
    }
}

void App::on_actionLoad_weather_graph_triggered()
{
    if (!currentPlace) {
        QMessageBox::warning(this, "Could not load a chart.", "There is no place open at the moment.");
        return;
    }

    using namespace QtCharts;

    QChart *chart = new QChart();
    QSplineSeries * series = new QSplineSeries;

    QString dirPath = QString::fromStdString(
            config::as_string(config::WEATHER_DIRECTORY_FORMAT))
            .arg(currentPlace->country(), currentPlace->city());

    QDir wd = QDir::home();
    if (!wd.cd(dirPath)) {
        // handle no such dir
    }

    std::vector<QDate> dates;
    std::array<std::vector<data_t<qreal>>, 3> dataSet =
            getCollectedData<qreal, 3>(wd, &dates);

    pairsort(dates, dataSet[0]);

    for (size_t i = 0; i < dates.size(); i++) {
        series->append(dates[i].startOfDay().toMSecsSinceEpoch(), dataSet[0][i].second - 273.15);
    }

    chart->addSeries(series);

    chart->createDefaultAxes();
    chart->setTitle("Weather in " + currentPlace->toQString());

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("dd-MM-yyyy");
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->axes().at(0)->setVisible(false);
    series->attachAxis(axisX);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartWindow->setCentralWidget(chartView);
    chartWindow->resize(400, 300);
    chartWindow->show();
}

void App::on_currentObservingDateEdit_dateTimeChanged(const QDateTime &dateTime)
{
    updateWeatherInfo();
    showWeatherWeek();
}

#include "weather.h"

const Weather Weather::INVALID_WEATHER = Weather(
            new QDate(0, 0, 0),
            new Place("", ""),
            new WeatherInfo(0, 0, 0)
            );

QDate *Weather::date() const { return date_; }

void Weather::setDate(QDate *date) { date_ = date; }

Place *Weather::place() const { return place_; }

void Weather::setPlace(Place *place) { place_ = place; }

WeatherInfo *Weather::winfo() const { return winfo_; }

void Weather::setWinfo(WeatherInfo *winfo) { winfo_ = winfo; }

tao::json::value Weather::toJson() const {
    tao::json::value jsonWeather = {
            {"date", date()->toString(Qt::ISODate).toStdString()},
            {"place",
                     {{"country",     place()->country().toStdString()},
                             {"city",     place()->city().toStdString()},
                             {"lat",      place()->lat()},
                             {"lon", place()->lon()}
                     }
            },
            {"winfo",
                     {{"temperature", winfo()->temperature()},
                             {"pressure", winfo()->pressure()},
                             {"humidity", winfo()->humidity()}}}};

    return jsonWeather;
}

Weather Weather::fromJson(const tao::json::value& jsonValue) {
    auto jsonDate = jsonValue.as<std::string>("date");
    auto *date = new QDate(
            QDate::fromString(QString::fromStdString(jsonDate), Qt::ISODate));

    auto jsonPlace = jsonValue.at("place");
    auto *place =
            new Place(QString::fromStdString(jsonPlace.as<std::string>("country")),
                      QString::fromStdString(jsonPlace.as<std::string>("city")));
    place->setLat(jsonPlace.as<qreal>("lat"));
    place->setLon(jsonPlace.as<qreal>("lon"));

    auto jsonWInfo = jsonValue.at("winfo");
    auto *winfo = new WeatherInfo(jsonWInfo.as<qreal>("temperature"),
                                         jsonWInfo.as<qreal>("pressure"),
                                         jsonWInfo.as<qreal>("humidity"));

    return Weather(date, place, winfo);
}

Weather Weather::load(QDate *date, Place *place)
{
    QDir possibleDir = QDir::home();
    QString possibleFolder = QString::fromStdString(config::as_string(config::WEATHER_DIRECTORY_FORMAT))
            .arg(place->country(), place->city());

    possibleDir.cd(possibleFolder);
    QString possibleFileName = QString::fromStdString(config::as_string(config::WEATHER_FILE_FORMAT))
            .arg(date->toString(Qt::ISODate));

    QFile possibleFile(possibleDir.filePath(possibleFileName));
    if (!possibleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw CorruptedWeatherFile(possibleFileName);
    }

    QTextStream ts(&possibleFile);
    Weather weather;
    ts >> weather;

    possibleFile.close();

    return weather;
}

Weather Weather::currentInPlaceAPI(Place *place) {
    cpr::Response cprResponse =
            cpr::Get(cpr::Url{config::as_string(config::API_ENTRY_CURRENT_WEATHER)},
                     cpr::Parameters{
                             {"lat",   QString::number(place->lat()).toStdString()},
                             {"lon",   QString::number(place->lon()).toStdString()},
                             {"appid", config::as_string(config::OPEN_WEATHER_API_KEY)}});

    tao::json::value jsonWeather;
    try {
        jsonWeather = tao::json::from_string(cprResponse.text);
    } catch (const tao::json::pegtl::parse_error &pe) {
        // handle parse error
        return Weather::INVALID_WEATHER;
    }

    // check response status for validity

    auto *date = new QDate(QDate::currentDate());

    auto *winfo =
            new WeatherInfo(jsonWeather.at("main").as<qreal>("temp"),
                            jsonWeather.at("main").as<qreal>("pressure"),
                            jsonWeather.at("main").as<qreal>("humidity"));

    return Weather(date, place, winfo);
}

Weather Weather::inPlaceAtDateAPI(QDate *date, Place *place) {
    cpr::Response cprResponse =
            cpr::Get(cpr::Url{config::as_string(config::API_ENTRY_HISTORICAL_WEATHER)},
                     cpr::Parameters{
                             {"lat",   QString::number(place->lat()).toStdString()},
                             {"lon",   QString::number(place->lon()).toStdString()},
                             {"dt",    QString::number(date->startOfDay().toTime_t()).toStdString()},
                             {"appid", config::as_string(config::OPEN_WEATHER_API_KEY)}
                     });

    tao::json::value jsonWeather;
    try {
        jsonWeather = tao::json::from_string(cprResponse.text);
    } catch (const tao::json::pegtl::parse_error &pe) {
        // handle parse error
        return Weather::INVALID_WEATHER;
    }

    // check response status for validity

    try {
        auto *winfo =
                new WeatherInfo(jsonWeather.at("current").as<qreal>("temp"),
                               jsonWeather.at("current").as<qreal>("pressure"),
                               jsonWeather.at("current").as<qreal>("humidity"));

        return Weather(date, place, winfo);
    }
    catch (const std::out_of_range &e) {
        return Weather::guessAtDate(date, place);
    }
}

Weather Weather::guessAtDate(QDate *date, Place *place) {
    QString dirPath = QString::fromStdString(
            config::as_string(config::WEATHER_DIRECTORY_FORMAT))
            .arg(place->country(), place->city());

    QDir wd = QDir::home();
    if (!wd.cd(dirPath)) {
        // handle no such dir
    }

    // get out this 3 in some enum or something
    std::array<std::vector<data_t<qreal>>, 3> dataSet =
            getCollectedData<qreal, 3>(wd, new std::vector<QDate>);
    std::array<qreal, 3> results =
            interpolateData<qreal, 3>(dataSet, totalDays(*date));

    Weather res(date, place, new WeatherInfo(results[0], results[1], results[2]));

    return res;
}

Weather Weather::currentInPlace(Place *place, bool api) {
    // try to locate saved one
    try {
        Weather weather = Weather::load(new QDate(QDate::currentDate()), place);
        return weather;
    }
    catch (const CorruptedWeatherFile &e) {
        // if the file is corrupted or does not exist
    }
    catch (const WeatherParseError &e) {
        // if the file was parsed badly
    }
    catch (const tao::json::pegtl::parse_error &e) {
        // if an engine could not parse a json
    }

    if (api)
        return currentInPlaceAPI(place);
    else
        return guessAtDate(new QDate(QDate::currentDate()), place);
}

Weather Weather::inPlace(QDate *date, Place *place, bool api) {
    // try to locate saved one
    try {
        Weather weather = Weather::load(date, place);
        return weather;
    }
    catch (const CorruptedWeatherFile &e) {}

    if (api)
        return inPlaceAtDateAPI(date, place);
    else
        return guessAtDate(date, place);
}

bool Weather::isValid() const {
    if (!this->date()->isValid())
        return false;
    if (*this->place() == Place::INVALID_PLACE)
        return false;
    if (*this->winfo() == WeatherInfo::INVALID_WEATHER_INFO)
        return false;

    return true;
}

void Weather::save() const {
    // ~/.weather/<country>/<city>/<date>.json
    QString dirPath = QString::fromStdString(
            config::as_string(config::WEATHER_DIRECTORY_FORMAT))
            .arg(place()->country(), place()->city());

    if (!QDir::home().mkpath(dirPath)) {
        // handle this (throw error)
    }

    QDir wd = QDir::home();
    wd.cd(dirPath);

    QFile file(wd.filePath(
            QString::fromStdString(config::as_string(config::WEATHER_FILE_FORMAT))
                    .arg(date()->toString(Qt::ISODate))));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // handle error (throw exception)
    }

    QTextStream ts(&file);
    ts << *this;

    file.close();
}

Weather::Weather(QDate *date, Place *place) {
    // todo test
    *this = Weather::inPlace(date, place);
}

QTextStream &operator<<(QTextStream &out, const Weather &weather) {
    out << QString::fromStdString(tao::json::to_string(weather.toJson(), 2));
    return out;
}

QTextStream &operator>>(QTextStream &in, Weather &weather) {
    QString jsonWeather;
    bool reading = false;
    int curlyBracesCount = 0;

    while ((curlyBracesCount != 0 || !reading) && !in.atEnd()) {
        QChar ch = in.read(1)[0];
        jsonWeather += ch;

        if (ch == '{') {
            if (!reading)
                reading ^= true;
            curlyBracesCount++;
        } else if (ch == '}') {
            curlyBracesCount--;
        }
    }

    if (!reading || curlyBracesCount) {
        throw WeatherParseError("null");
    }
    else
        try {
            weather =
                    Weather::fromJson(tao::json::from_string(jsonWeather.toStdString()));
        } catch (const tao::json::pegtl::parse_error &pe) {
            __throw_exception_again;
        }

    return in;
}

int totalDays(const QDate &date) {
    QDate UNIX_START = QDate(1970, 1, 1);
    int res = -date.daysTo(UNIX_START);
    return res;
}

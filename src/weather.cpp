#include "weather.h"

const Weather Weather::INVALID_WEATHER = {};


QDate *Weather::date() const
{
    return date_;
}

void Weather::setDate(QDate *date)
{
    date_ = date;
}

Place *Weather::place() const
{
    return place_;
}

void Weather::setPlace(Place *place)
{
    place_ = place;
}

WeatherInfo *Weather::winfo() const
{
    return winfo_;
}

void Weather::setWinfo(WeatherInfo *winfo)
{
    winfo_ = winfo;
}


const tao::json::value Weather::toJson() const
{
    const tao::json::value jsonWeather = {
        { "date", date()->toString(Qt::ISODate).toStdString() },
        { "place", {
              { "country", place()->country().toStdString() },
              { "city", place()->city().toStdString() }
          }},
        { "winfo", {
              { "temperature", winfo()->temperature() },
              { "pressure", winfo()->pressure() },
              { "humidity", winfo()->humidity() }
          } }
    };

    return jsonWeather;
}

Weather Weather::fromJson(const tao::json::value jsonValue)
{
    auto jsonDate = jsonValue.as<std::string>("date");
    QDate *date = new QDate(QDate::fromString(QString::fromStdString(jsonDate), Qt::ISODate));

    auto jsonPlace = jsonValue.at("place");
    Place *place = new Place(
                QString::fromStdString(jsonPlace.as<std::string>("country")),
                QString::fromStdString(jsonPlace.as<std::string>("city"))
                );

    auto jsonWInfo = jsonValue.at("winfo");
    WeatherInfo *winfo = new WeatherInfo(
                jsonWInfo.as<qreal>("temperature"),
                jsonWInfo.as<qreal>("pressure"),
                jsonWInfo.as<qreal>("humidity")
                );

    return Weather(date, place, winfo);
}

Weather Weather::currentInPlaceAPI(Place *place)
{
    cpr::Response cprResponse = cpr::Get(
                cpr::Url {config::as_string(config::API_ENTRY_CURRENT_WEATHER)},
                cpr::Parameters {
                    { "lat", QString::number(place->lat()).toStdString() },
                    { "lon", QString::number(place->lon()).toStdString() },
                    { "appid", config::as_string(config::OPEN_WEATHER_API_KEY) }
                }
                );

    tao::json::value jsonWeather;
    try
    {
        jsonWeather = tao::json::from_string(cprResponse.text);
    }
    catch (const tao::json::pegtl::parse_error &pe)
    {
        // handle parse error
        return Weather::INVALID_WEATHER;
    }

    // check response status for validity

    QDate *date = new QDate(QDate::currentDate());

    WeatherInfo *winfo = new WeatherInfo(
                jsonWeather.at("main").as<qreal>("temp"),
                jsonWeather.at("main").as<qreal>("pressure"),
                jsonWeather.at("main").as<qreal>("humidity")
                );

    return Weather(date, place, winfo);
}

Weather Weather::guessAtDate(QDate *date, Place *place)
{
    // todo perform inter / extrapolation
}

Weather Weather::currentInPlace(Place *place, bool api)
{
    if (api)
        return currentInPlaceAPI(place);
    else
        return guessAtDate(new QDate(QDate::currentDate()), place);
}

bool Weather::isValid() const
{
    if (!this->date()->isValid()) return false;
    if (*this->place() == Place::INVALID_PLACE) return false;
    if (*this->winfo() == WeatherInfo::INVALID_WEATHER_INFO) return false;

    return true;
}

void Weather::save() const
{
    // ~/.weather/<country>/<city>/<date>.json
    QDir::mkpath()
}

Weather::Weather(QDate *date, Place *place) {
    // todo loading from API
}

QTextStream &operator<<(QTextStream &out, const Weather &weather)
{
    out << QString::fromStdString(tao::json::to_string(weather.toJson(), 2));
    return out;
}

QTextStream &operator>>(QTextStream &in, Weather &weather)
{
    QString jsonWeather;
    bool reading = false;
    int curlyBracesCount = 0;

    while ((curlyBracesCount != 0 || !reading) && !in.atEnd())
    {
        QChar ch = in.read(1)[0];
        jsonWeather += ch;

        if (ch == '{')
        {
            if (!reading) reading ^= true;
            curlyBracesCount++;
        }
        else if (ch == '}')
        {
            curlyBracesCount--;
        }
    }

    if (!reading)
        throw std::runtime_error("No valid json found.");
    else if (curlyBracesCount)
        throw std::runtime_error("Seems like json is malformed.");
    else
        try
        {
            weather = Weather::fromJson(tao::json::from_string(jsonWeather.toStdString()));
        }
        catch (const tao::json::pegtl::parse_error &pe)
        {
            __throw_exception_again;
        }

    return in;
}

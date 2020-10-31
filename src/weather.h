#ifndef WEATHER_H
#define WEATHER_H

#include <QString>
#include <QTextStream>
#include <QDate>
#include <QtGlobal> // qt types compatibility
#include <QDir>
#include <QFile>

#include <cpr/cpr.h>
#include <tao/json.hpp>

#include "place.h"
#include "weatherinfo.h"
#include "config.h"


#define constructors public
#define s_public public
#define s_private private
#define getset public
#define members private
#define operators public
#define conversions public

class Weather
{   
constructors:
    Weather()
        : date_(nullptr), place_(nullptr), winfo_(nullptr) {}; // [x]
    Weather(const Weather &weather)
        : date_(weather.date()), place_(weather.place()), winfo_(weather.winfo()) {}; // [x]
    Weather(QDate *date, Place *place, WeatherInfo *winfo)
        : date_(date), place_(place), winfo_(winfo) {}; // [x]

    // [-]
    Weather(QDate *date, Place *place); // throws warning when date is not in latest 5 days

conversions:
    [[nodiscard]] const tao::json::value toJson() const; // [x]
    static Weather fromJson(tao::json::value jsonValue); // [x]

operators:
    friend QTextStream &operator<<(QTextStream &out, const Weather &weather); // [x]
    friend QTextStream &operator>>(QTextStream &in, Weather &weather); // [x]

    bool operator==(const Weather &weather) const { return this->toJson() == weather.toJson(); } // [x]
    bool operator!=(const Weather &weather) const { return !(*this == weather); } // [x]

s_public:
    static Weather guessAtDate(QDate *date, Place *place); // [-]
    static Weather currentInPlaceAPI(Place *place); // [-x] // status check
    static Weather currentInPlace(Place *place, bool api = true); // [-]
    // throw extra/inter polation exceptions

    static const Weather INVALID_WEATHER;

public:
    bool isValid() const; // [x]
    void save() const; // [-]
    // throws error, caught when saving (file stuff)

members:
    /* date of the record */
    QDate *date_{};

    /* weather location */
    Place *place_{};

    /* Main weather information */
    WeatherInfo *winfo_{};

    friend class WeatherInfo;

getset:
    QDate *date() const; // [x]
    void setDate(QDate *pDate); // [x]

    Place *place() const; // [x]
    void setPlace(Place *pPlace); // [x]

    WeatherInfo *winfo() const; // [x]
    void setWinfo(WeatherInfo *winfo); // [x]
};

#endif // WEATHER_H

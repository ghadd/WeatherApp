#ifndef WEATHER_H
#define WEATHER_H

#include <QString>
#include <QTextStream>
#include <QDateTime>
#include <QDate>
#include <QtGlobal> // qt types compatibility
#include <QDir>
#include <QFile>

#include <cpr/cpr.h>
#include <tao/json.hpp>

#include "place.h"
#include "weatherinfo.h"
#include "config.h"
#include "logic.h"


#define constructors public
#define s_public public
#define getset public
#define members private
#define operators public
#define conversions public

class Weather {
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

    [[nodiscard]] tao::json::value toJson() const; // [x]
    static Weather fromJson(const tao::json::value& jsonValue); // [x]

operators:

    friend QTextStream &operator<<(QTextStream &out, const Weather &weather); // [x]
    friend QTextStream &operator>>(QTextStream &in, Weather &weather); // [x]

    bool operator==(const Weather &weather) const { return this->toJson() == weather.toJson(); } // [x]
    bool operator!=(const Weather &weather) const { return !(*this == weather); } // [x]

s_public:
    static Weather load(QDate *date, Place *place);

    static Weather guessAtDate(QDate *date, Place *place); // [x]
    static Weather currentInPlaceAPI(Place *place); // [-x] // status check
    static Weather inPlaceAtDateAPI(QDate *date, Place *place); // [-]
    static Weather currentInPlace(Place *place, bool api = true); // [x]
    static Weather inPlace(QDate *date, Place *place, bool api = true); // [-]
    // throw extra/inter polation exceptions

    static const Weather INVALID_WEATHER;

public:
    [[nodiscard]] bool isValid() const; // [x]
    void save() const; // [x]
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

    [[nodiscard]] QDate *date() const; // [x]
    void setDate(QDate *pDate); // [x]

    [[nodiscard]] Place *place() const; // [x]
    void setPlace(Place *pPlace); // [x]

    [[nodiscard]] WeatherInfo *winfo() const; // [x]
    void setWinfo(WeatherInfo *winfo); // [x]
};

int totalDays(const QDate &date);

template<typename T, int N>
std::array<std::vector<data_t<T>>, N> getCollectedData(QDir dir) {
    std::array<std::vector<data_t<T> >, N> dataSet;

    dir.setFilter(QDir::Files);
    uint nFiles = dir.count();
    for (auto &v : dataSet)
        v.resize(nFiles);

    uint pos = 0;
    QDirIterator dirIt(dir);
    while (dirIt.hasNext()) {
        QFile file(dirIt.next());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // handle this
        }

        Weather weather;
        QTextStream ts(&file);
        ts >> weather;

        uint argpos = 0;
        dataSet[argpos++][pos] =
                std::make_pair(totalDays(*weather.date()), weather.winfo()->temperature());
        dataSet[argpos++][pos] =
                std::make_pair(totalDays(*weather.date()), weather.winfo()->pressure());
        dataSet[argpos++][pos] =
                std::make_pair(totalDays(*weather.date()), weather.winfo()->humidity());

        pos++;
    }

    return dataSet;
}

template<typename T, int N>
std::array<T, N> interpolateData(const std::array<std::vector<data_t<T> >, N> &dataSet, T atX) {
    std::array<T, N> interpolatedData;

    for (int i = 0; i < N; i++)
        interpolatedData[i] = interpolate<T>(dataSet[i], atX);

    return interpolatedData;
}

#endif // WEATHER_H

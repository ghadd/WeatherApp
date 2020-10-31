#ifndef WEATHERINFO_H
#define WEATHERINFO_H

#include <QtGlobal>

class WeatherInfo
{
public:
    static const WeatherInfo INVALID_WEATHER_INFO;

    WeatherInfo();
    WeatherInfo(qreal temperature, qreal pressure, qreal humidity)
        : temperature_(temperature), pressure_(pressure), humidity_(humidity) {};

    bool operator==(const WeatherInfo &winfo);
    bool operator!=(const WeatherInfo &winfo);

    [[nodiscard]] qreal temperature() const;
    void setTemperature(const qreal &temperature);

    [[nodiscard]] qreal pressure() const;
    void setPressure(const qreal &pressure);

    [[nodiscard]] qreal humidity() const;
    void setHumidity(const qreal &humidity);

private:
    /* temperature in Kelvins */
    qreal temperature_{};

    /* atmosphere pressure in kPa */
    qreal pressure_{};

    /* relative 0-100% value of humidity */
    qreal humidity_{};
};

#endif // WEATHERINFO_H

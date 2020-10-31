#include "weatherinfo.h"

const WeatherInfo WeatherInfo::INVALID_WEATHER_INFO = {};

WeatherInfo::WeatherInfo()
= default;

bool WeatherInfo::operator==(const WeatherInfo &winfo)
{
    return this->temperature() == winfo.temperature() &&
            this->pressure() == winfo.pressure() &&
            this->humidity() == winfo.humidity();
}

bool WeatherInfo::operator!=(const WeatherInfo &winfo)
{
    return !(*this == winfo);
}

qreal WeatherInfo::temperature() const
{
    return temperature_;
}

void WeatherInfo::setTemperature(const qreal &temperature)
{
    temperature_ = temperature;
}

qreal WeatherInfo::pressure() const
{
    return pressure_;
}

void WeatherInfo::setPressure(const qreal &pressure)
{
    pressure_ = pressure;
}

qreal WeatherInfo::humidity() const
{
    return humidity_;
}

void WeatherInfo::setHumidity(const qreal &humidity)
{
    humidity_ = humidity;
}

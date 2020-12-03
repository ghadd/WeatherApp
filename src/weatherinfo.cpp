#include "weatherinfo.h"

const WeatherInfo WeatherInfo::INVALID_WEATHER_INFO = {};

WeatherInfo::WeatherInfo()
= default;

QString WeatherInfo::toQString() const
{
    return QString("🌡️ %1°C\n"
                   "🌪️ %2hPa\n"
                   "🌫️ %3%")
            .arg(temperature_ - 273.15)
            .arg(pressure_)
            .arg(humidity_);
}

bool WeatherInfo::operator==(const WeatherInfo &winfo) const {
    return this->temperature() == winfo.temperature() &&
           this->pressure() == winfo.pressure() &&
           this->humidity() == winfo.humidity();
}

bool WeatherInfo::operator!=(const WeatherInfo &winfo) const {
    return !(*this == winfo);
}

qreal WeatherInfo::temperature() const {
    return temperature_;
}

void WeatherInfo::setTemperature(const qreal &temperature) {
    temperature_ = temperature;
}

qreal WeatherInfo::pressure() const {
    return pressure_;
}

void WeatherInfo::setPressure(const qreal &pressure) {
    pressure_ = pressure;
}

qreal WeatherInfo::humidity() const {
    return humidity_;
}

void WeatherInfo::setHumidity(const qreal &humidity) {
    humidity_ = humidity;
}

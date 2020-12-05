#ifndef WEATHEREXCEPTION
#define WEATHEREXCEPTION

#include <exception>
#include "weather.h"

class WeatherException : public std::exception {
public:
    WeatherException() = default;
    const char *what() const noexcept override;
};

class CorruptedWeatherFile : public WeatherException {
public:
    CorruptedWeatherFile(QString filename);
    const char *what() const noexcept override;

private:
    QString cause;
};

class WeatherParseError : public WeatherException {
public:
    WeatherParseError(QString filename);
    const char *what() const noexcept override;

private:
    QString cause;
};

#endif

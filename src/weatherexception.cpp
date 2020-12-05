
#include "weatherexception.h"

const char *WeatherException::what() const noexcept
{
    return "Unhandled error in weather object.";
}

CorruptedWeatherFile::CorruptedWeatherFile(QString filename)
{
    cause = std::move(filename);
}

const char *CorruptedWeatherFile::what() const noexcept
{
    return "File " + cause.toLocal8Bit() + " is corrupted.";
}

WeatherParseError::WeatherParseError(QString filename)
{
    cause = filename;
}

const char *WeatherParseError::what() const noexcept
{
    if (cause.isEmpty()) {
        return "Could not parse a JSON.";
    }
    return "No valid json found in " + cause.toLocal8Bit() + ".";
}

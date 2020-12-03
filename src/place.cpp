#include "place.h"

#include <iostream>

const Place Place::INVALID_PLACE = {"", ""};

Place::Place(const QString &country, const QString &city) {
    country_ = country;
    city_ = city;
    lat_ = 0;
    lon_ = 0;
    validated = false;
}

void Place::validateCoords(const tao::json::value &bounds) {
    const tao::json::value northeast = bounds.at("northeast");
    const tao::json::value southwest = bounds.at("southwest");
    this->lat_ = (northeast.as<qreal>("lat") + southwest.as<qreal>("lat")) / 2;
    this->lon_ = (northeast.as<qreal>("lng") + southwest.as<qreal>("lng")) / 2;
}

void Place::validateCountryAndCity(const tao::json::value &components) {
    this->country_ = QString::fromStdString(components.as<std::string>("country"));
    this->city_ = QString::fromStdString(components.as<std::string>("city"));
}

void Place::validateSelf() {
    cpr::Response cprResponse = cpr::Get(
            cpr::Url{config::as_string(config::API_ENTRY_OPEN_CAGE)},
            cpr::Parameters{
                    {"q",   (this->toQString()).toStdString()},
                    {"key", config::as_string(config::OPEN_CAGE_API_KEY)}
            }
    );
    tao::json::value jsonResponse;
    try {
        jsonResponse = tao::json::from_string(cprResponse.text);
        validateCountryAndCity(jsonResponse.at("results").at(0).at("components"));
        validateCoords(jsonResponse.at("results").at(0).at("bounds"));

        validated = true;
    }
    catch (const tao::json::pegtl::parse_error &pe) {
        InvalidPlaceException exc(*this);
        *this = INVALID_PLACE;
        throw exc;
    }
    catch (const std::out_of_range &e) {
        InvalidPlaceException exc(*this);
        *this = INVALID_PLACE;
        throw exc;
    }

    // check for valid status
}

QString Place::toQString() const {
    return QString("%1, %2").arg(city_, country_);
}

QString Place::coordsQString() const
{
    char latChar = lat_ > 0 ? 'N' : 'S';
    char lonChar = lon_ > 0 ? 'E' : 'W';

    return QString("%1%2, %3%4")
            .arg(lat_)
            .arg(latChar)
            .arg(lon_)
            .arg(lonChar);
}

bool Place::operator==(const Place &place) const {
    return this->city() == place.city() && this->country() == place.country();
}

bool Place::operator!=(const Place &place) const {
    return !(*this == place);
}

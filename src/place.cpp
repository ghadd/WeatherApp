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
    }
    catch (const tao::json::pegtl::parse_error &pe) {
        *this = INVALID_PLACE;
        __throw_exception_again;
    }

    // check for valid status

    validateCountryAndCity(jsonResponse.at("results").at(0).at("components"));
    validateCoords(jsonResponse.at("results").at(0).at("bounds"));

    validated = true;
}

QString Place::toQString() {
    return QString("%1,%2").arg(city_, country_);
}

bool Place::operator==(const Place &place) const {
    return this->city() == place.city() && this->country() == place.country();
}

bool Place::operator!=(const Place &place) const {
    return !(*this == place);
}

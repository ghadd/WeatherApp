#ifndef PLACE_H
#define PLACE_H

#include <QString>
#include <QtGlobal>
#include <utility>

#include <cpr/cpr.h>
#include <tao/json.hpp>

#include "config.h"

class Place
{
public:
    static const Place INVALID_PLACE;

    Place(const QString &country, const QString &city);
    QString toQString();

    bool operator==(const Place &place);
    bool operator!=(const Place &place);

    void validateSelf();

private:
    void validateCoords(const tao::json::value &bounds);
    void validateCountryAndCity(const tao::json::value &copmonents);

public:
    [[nodiscard]] inline QString country() const { return country_; }
    inline void setCountry(const QString &country) { country_ = country; }

    [[nodiscard]] inline QString city() const { return city_; }
    void setCity(const QString &city) { city_ = city; };

    [[nodiscard]] inline qreal lat() const { return lat_; }
    void setLat(const qreal &lat) { lat_ = lat; }

    [[nodiscard]] inline qreal lon() const { return lon_; }
    void setLon(const qreal &lon) { lon_ = lon; }

private:
    QString country_;
    QString city_;
    qreal lat_;
    qreal lon_;

    bool validated;
};

#endif // PLACE_H

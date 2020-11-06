
#include <iostream>

#include "weather.h"
#include "place.h"

#include <vector>
#include "logic.h"

int main() {
    std::string country = "Польща", city = "Щецин";

    auto *place = new Place(
            QString::fromStdString(country),
            QString::fromStdString(city)
    );
    place->validateSelf();

    Weather weather = Weather::guessAtDate(new QDate(2020, 10, 30), place);
    weather.save();

    return 0;
}

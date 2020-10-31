
#include <iostream>
#include <cpr/cpr.h>
#include <tao/json.hpp>

#include "weather.h"
#include "place.h"

#include <utility>
#include <vector>
#include "logic.h"

int main() {
    Place *place = new Place("Україна", "Львів");
    place->validateSelf();


    std::cout << place->toQString().toStdString() << " " << place->lat() << " " << place->lon() << std::endl;

    return 0;
}

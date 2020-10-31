#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <string_view>

namespace config {
    using namespace std::literals;
    inline constexpr auto OPEN_CAGE_API_KEY = "f1b05a4d831640d7bda171da5066ce44"sv;
    inline constexpr auto OPEN_WEATHER_API_KEY = "90b34303c90966d9988b014c6388d0f4"sv;


    /* Params:
     * 'lat': place lattitudue
     * 'lon': place longtitude
     * 'appid': api key
     *
     * Result: ::main
     * ::temp
     * ::pressure
     * ::humidity
     */
    inline constexpr auto API_ENTRY_CURRENT_WEATHER = "api.openweathermap.org/data/2.5/weather"sv;

    /* Params:
     * 'lat': place lattitudue
     * 'lon': place longtitude
     * 'dt': time in unix
     * 'appid': api key
     */
    inline constexpr auto API_ENTRY_HISTORICAL_WEATHER = "http://api.openweathermap.org/data/2.5/onecall/timemachine"sv;

    /* Params:
     * 'q': place query
     * 'key': api key
     *
     * Result: ::results::bounds::northeast
     * ::lat
     * ::lng
     */
    inline constexpr auto API_ENTRY_OPEN_CAGE = "https://api.opencagedata.com/geocode/v1/json"sv;

    /* %1: $HOME
     * %2: Country
     * %3: City
     * %4: Date (ISO)
     */
    inline constexpr auto WEATHER_DIRECTORY_FORMAT = "%1/.weather/%2/%3/%4"sv;

    std::string as_string(const std::string_view &sv);
}

#endif // CONFIG_H

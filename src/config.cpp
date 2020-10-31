#include "config.h"

std::string config::as_string(const std::string_view &sv)
{
    return {sv.begin(), sv.end()};
}

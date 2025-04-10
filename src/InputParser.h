#pragma once

#include <Scale.h>

#include <filesystem>

namespace balancer {
std::vector<std::unique_ptr<Scale>> importScalesForest(const std::filesystem::path& source);
}

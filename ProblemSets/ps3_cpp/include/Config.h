#pragma once

// Structures to hold runtime configurations.
#include <common/BasicConfig.h>
#include "FParse.h"

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include <opencv2/core/core.hpp>

#include <cstddef>
#include <iostream>
#include <memory>
#include <random>

namespace config {
static constexpr char FILE_LOGGER[] = "file_logger";
static constexpr char STDOUT_LOGGER[] = "logger";
}; // namespace config

class Config {
private:
    std::shared_ptr<spdlog::logger> _logger, _fileLogger;

public:
    // Structures
    struct Images : BasicConfig {
        cv::Mat _picA, _picB;

        Images() = default;
        Images(const YAML::Node& imagesNode);
    };

    struct Points : BasicConfig {
        // Vectors of column-vectors for each of the point sets
        cv::Mat _picA, _picB, _picANorm, _pts3D, _pts3DNorm;

        Points() = default;
        Points(const YAML::Node& node);

    private:
        // Load points from a text file
        template <typename T>
        bool loadPoints(const YAML::Node& node, const std::string& key, cv::Mat& points) {
            auto logger = spdlog::get(config::STDOUT_LOGGER);
            auto flogger = spdlog::get(config::FILE_LOGGER);
            if (node[key]) {
                std::string path = node[key].as<std::string>();
                points = FParse::parseAs<T>(path);
                if (!points.empty()) {
                    flogger->info("Loaded points from {}", path);
                    return true;
                }
            }

            logger->error("Could not load points from param \"{}\"", key);
            return false;
        }
    };

    Images _images;
    Points _points;

    // Path to which output images will be written
    std::string _outputPathPrefix;
    // Seed for mersenne twister engine
    std::unique_ptr<std::seed_seq> _mersenneSeed;

    bool _configDone = false;

    Config(const std::string& configFilePath);

    bool loadConfig(const YAML::Node& config);
};
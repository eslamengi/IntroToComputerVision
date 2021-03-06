#pragma once

// Structures to hold runtime configurations.
#include <common/BasicConfig.h>
#include <common/Utils.h>

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
    struct ImageSet : BasicConfig {
    private:
        // Input images
        std::vector<cv::Mat> _pics;

    public:
        ImageSet() = default;
        ImageSet(const YAML::Node& node, const std::string& dirKey);
        using iterator = std::vector<cv::Mat>::iterator;
        using const_iterator = std::vector<cv::Mat>::const_iterator;

        // Loads a set of images in alphabetical order into a vector of cv::Mats
        bool loadImFromDir(const std::string& dir);

        // Indexing operator so that we don't have to access _pics directly
        cv::Mat& operator[](int idx) { return _pics[idx]; }

        const cv::Mat& operator[](int idx) const { return _pics[idx]; }

        inline iterator begin() noexcept { return _pics.begin(); }
        inline const_iterator begin() const noexcept { return _pics.cbegin(); }
        inline iterator end() noexcept { return _pics.end(); }
        inline const_iterator end() const noexcept { return _pics.end(); }
    };

    ImageSet _yosemite, _pupper, _juggle, _shift;
    bool _useGpu = false;
    size_t _lkWinSize1, _lkWinSize3, _lkWinSize4, _pyrLevel3a, _pyrLevel3b;

    // Path to which output images will be written
    std::string _outputPathPrefix;

    bool _configDone = false;

    Config(const std::string& configFilePath);

    bool loadConfig(const YAML::Node& config);
};
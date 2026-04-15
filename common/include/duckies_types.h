/// @file duckies_types.h
/// @brief Declares types used in the duckies application.

#ifndef TYPES_H
#define TYPES_H

#include <chrono>
#include <string>
#include <vector>

namespace duckies { 

/// @brief Represents a geographical coordinate with latitude and longitude.
struct Coordinate {
    float latitude = 0.0f;
    float longitude = 0.0f;
}; 

/// @brief Represents a collection of rubber duck sighting data, including coordinates
class RubberDuckData {
    std::vector<Coordinate> coordinates_;
    std::vector<std::chrono::year_month_day> dates_;
    std::vector<std::string> descriptions_;

public:
    /// @brief Inserts a new rubber duck sighting into data collection
    /// @param latitude The latitude of the rubber duck sighting.
    /// @param longitude The longitude of the rubber duck sighting.
    /// @param date The date of the rubber duck sighting.
    /// @param description A description of the rubber duck sighting.
    void insert(float latitude, float longitude, 
        std::chrono::year_month_day date, std::string description) 
    {
        coordinates_.push_back(Coordinate{latitude, longitude});
        dates_.emplace_back(date);
        descriptions_.emplace_back(std::move(description));
    }

    /// @brief Reserves additional capacity in data vectors to accommodate more entries
    /// @param additional_capacity 
    void reserve_additional(size_t additional_capacity) {

        const size_t current_capacity = coordinates_.capacity();
        const size_t new_capacity = current_capacity + additional_capacity;

        coordinates_.reserve(new_capacity);
        dates_.reserve(new_capacity);
        descriptions_.reserve(new_capacity);
    }

    // accessor methods 
    /// @brief Gets the collection of coordinates for all rubber duck sightings.
    /// @return A constant reference to the vector of coordinates.
    const std::vector<Coordinate>& get_coordinates() const { return coordinates_; }

    /// @brief Gets collection of dates for all rubber duck sightings.
    /// @return A constant reference to the vector of dates.
    const std::vector<std::chrono::year_month_day>& get_dates() const { return dates_; }
    
    /// @brief Gets collection of descriptions for all rubber duck sightings.
    /// @return A constant reference to vector of descriptions.
    const std::vector<std::string>& get_descriptions() const { return descriptions_; }
};
}

#endif 
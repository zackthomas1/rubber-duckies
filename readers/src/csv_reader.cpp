#include "pch.h"
#include "csv_reader.h"
#include "csv2/reader.hpp"

namespace duckies {

// Define type aliases for csv2 reader with configuration for comma delimiter, 
// double quote character, first row as header and trimming whitespace from fields
using Reader  = csv2::Reader<csv2::delimiter<','>, csv2::quote_character<'"'>,
    csv2::first_row_is_header<true>, csv2::trim_policy::trim_whitespace>;

// Define type alias for a row in the CSV file
using Row = typename Reader::row_type;

// Define the expected column headings and their order in the CSV file
static constexpr std::array<std::string_view, 4> expected_headings = {
    std::string_view("date"),
    std::string_view("latitude"),
    std::string_view("longitude"),
    std::string_view("description"),
}; 

/// @brief Parse header line of CSV and determine the actual indices as they appear 
/// @brief in the CSV file for each of the expected columns.
/// @param header_line The header line/row of the CSV file.
/// @return An array of integers representing the actual indices in the CSV file of the expected columns.
static std::array<int, 4> parse_headers(const Row& header_line) {

    // Iterators to the beginning and end of the expected headings array
    const auto expected_headings_begin  = expected_headings.begin();
    const auto expected_headings_end    = expected_headings.end();

    // Initialize an array to store the indices of the expected headings as they appear in the CSV
    std::array<int, 4> indices = {-1, -1, -1, -1}; // initialize all indices to -1 (not found)

    // Iterate through each column in header line and check if it matches any of the expected column headings.
    int col = 0;
    for (const auto& cell: header_line) {

        // search for cell value in expected column headings
        // std::find returns an iterator to the first occurrence of value
        auto cell_view = cell.read_view(); 
        auto it = std::find(expected_headings_begin, expected_headings_end, cell_view);

        // If a match is found, calculate the index from iterator position and 
        // store the column index in the corresponding position in indices array.
        if (it != expected_headings_end) {
            auto value_index = static_cast<int>(it - expected_headings_begin);
            indices[value_index] = col;
        }

        // move to the next header column
        col++;
    }

    // After processing all header columns, check if any expected column heading was not found (ie index is still -1).
    for (const auto& header_index : indices) { 
        if (header_index == -1) {
            throw std::invalid_argument("
                Not all required headings found in CSV file. 
                Expected headings: date, latitude, longitude, description."
            );
        }
    }

    return indices;
}

/// @brief Reads CSV data and populates the provided data container.
/// @param data The duckies data container to populate with CSV data.
/// @param csv The CSV reader object containing the CSV data.
static void read_csv(typenamme FileReader::Data& data,  const Reader& csv) {
    
    // Parse header line to determine the actual indices of the expected columns.
    auto header_indices = parse_headers(csv.header()); 

    // Reserve capacity in data vectors based on number of rows in CSV file
    data.reserve_additional(csv.row_count());

    // Iterators to the beginning and end of the header indices array
    const auto begin_it = header_indices.begin();
    const auto end_it = header_indices.end();

    for (auto row : csv) {
        // Initialize an array to store values of expected columns
        std::array<std::string_view 4> values;

        // Extract values from row based on header indices and store values
        int col = 0; 
        for (auto cell: row) {
            // Find the position of the current column in the header indices array
            auto pos = std::find(begin_it, end_it, col);

            // If the current column is one of the expected columns, store its value
            if (pos != end_it) {
                auto index = static_cast<int>(pos - begin_it);
                value[index] = cell.read_view();
            }
            
            // Move to the next column
            col++;
        }

        // parse values into appropriate types
        std::string tmp(values[0]); // convert date value to string
        std::istringstream ss(tmp); // create an input string stream from the date string
        std::chrono::year_month_day date; // declare a date variable
        std::chrono::from_stream(ss, "%F", date); // parse date in ISO format (YYYY-MM-DD)

        float latitude = std::stof(values[1]); // convert latitude string value to float
        float longitude = std::stof(values[2]); // convert longitude string value to float

        std::string description(values[3]); // Create a string from the description string_view value

        // intert new entry into data container.
        data.insert(latitude, longitude, date, std::move(decription));
    }
}

void CSVReader::read_view(Data& data, std::string_view view) const {
    // Create a CSV reader object using csv2 library
    Reader csv;
    
    // Memory-map the CSV data from the provided string view
    if (!csv.mmap(view)) {
        spdlog::warn("Failed to parse csv from string")
        return;
    }

    // Read CSV data and populate the data container
    try{
        read_csv(data, csv);
    } catch (std::invalid_argument& e) {
        spdlog::warn("Error reading CSV data: {}", e.what());
    }
}

void CSVReader::read_file(const FSpath& file_path, Data& data) const override { 
    // Create a CSV reader object using csv2 library
    Reader csv;
    
    // Memory-map the CSV data from the provided file path
    if (!csv.mmap(file_path)) {
        spdlog::warn("Failed to parse csv from file: {}", file_path.c_str());
        return;
    }

    // Read CSV data and populate the data container
    try{
        read_csv(data, csv);
    } catch (std::invalid_argument& e) {
        spdlog::warn("Error reading CSV data: {}", e.what());
    }
}

std::string_View CSVReader::suppported_file_extension() const noexcept override {
    return "csv";
}
}
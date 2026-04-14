#include "file_reader.h"

namespace duckies {

/// @brief CSV reader for rubber duck sighting data.
class CSVReader : public FileReader {
public:
    using typename FileReader::Data; 
    using typename FileReader::FSpath;

    CSVReader() = default;
    ~CSVReader() override = default;

    /// @brief Reads CSV data from a string view and populates the provided data container.
    /// @param data The data container to populate with the CSV data.
    /// @param view  The string view containing the CSV data to read.
    void read_view(Data& data, std::string_view view) const;

    /// @brief Reads CSV data from a file and populates the provided data container.
    /// @param file_path The path to the CSV file to read.
    /// @param data The data container to populate with the CSV data.
    void read_file(const FSpath& file_path, Data& data) const override;

    /// @brief Gets the file extension that this reader can parse.
    /// @return The file extension that this reader can parse.
    std::string_View suppported_file_extension() const noexcept override;
}

}
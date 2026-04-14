#include  <filesystem>
#include <string_view>

#include "duckies_types.h"  // contains RubberDuckData

namespace duckies {

/// @brief Abstract base class for reading rubber duck sighting data from file.
class FileReader {
public:
    // Type aliases for convenience
    /// @brief Alias for rubber duck data type
    using Data = RubberDuckData

    /// @brief Alias for filesystem path type
    using FSpath = std::filesystem::path;

    FileReader() = default;
    virtual ~FileReader() = default;

    /// @brief Read data from file into rubber duck data structure
    /// @param file_path The path to the file to be read.
    /// @param data The rubber duck data structure to populate with the read data.
    virtual void read_file(const FSpath& file_path, Data& data) const = 0;

    /// @brief Get the file extension that reader can parse
    /// @return The file extension that this reader can parse
    virtual std::string_view suppported_file_extension() const noexcept = 0;
};
}
/// @file main.cpp 
/// @brief Main entry point for the duckies application, handling configuration, logging, and execution flow.

#include <atomic>
#include <csignal>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <omp.h>

// headers for readers and clustering logic 

// namespace aliases
namespace po = boost::program_options;

// program metadata
static const char program_version[] = "1.0.0"; 
static const char program_description[] = "Duckies [options] file1 file2 ... \n - a command-line tool for processing duck clusters";
static const char program_usage[] = "Usage: duckies [options] file1 file2 ... \nTry 'duckies --help' for more information.";

// signal handler for SIGINT (Ctrl+C)
static std::atomic_int signal_count = 0;

// function declarations
static void setup_signals();
bool check_interrupts() noexcept;
template<typename F>
void handle_errors(F&& func) noexcept;
static po::variables_map parse_config(int argc, const char *const *argv);
static void setup_logging(const po::variables_map& args);
static void setup_threading(const po::variables_map& args);
static void run_and_report(const po::variables_map& args);

int main(int argc, char** argv) {
    setup_signals();

    handle_errors([&]{
        // parse configuration and set up logging and threading based on config
        const auto args = parse_config(argc, argv); 
        setup_logging(args);
        setup_threading(args);

        run_and_report(args);
    });
    
    return EXIT_SUCCESS;
}

extern "C"
void sigint_handler(int signum) {
    auto current = signal_count.fetch_add(1);
    if (current > 0) {
        std::exit(EXIT_FAILURE);
    }
}

static void setup_signals() {
    std::signal(SIGINT, &sigint_handler);
}

bool check_interrupts() noexcept {
    auto interrupt = signal_count.load() > 0;
    if (interrupt) {
        spdlog::warn("Interrupt signal received. Attempting to stop gracefully...");
    }
    return interrupt;
}

template<typename F>
void handle_errors(F&& func) noexcept {
    try {
        func();
    } catch (const std::exception& exc) {
        spdlog::critical("Fatal error: {}", exc.what());
        std::exit(EXIT_FAILURE);
    }
}

po::variables_map parse_config(int argc, const char *const *argv) { 
    using PathVec = std::vector<std::filesystem::path>; 

    // Define command-line options using Boost.Program_options
    po::options_description options {"Options"}; 
    options.add_options()
        ("help, h", po::bool_switch(), "produce_help_message")
        ("version", po::bool_switch(), "print the version and exit")
        ("verbose, v", po::bool_switch(), "print logging information to the terminal")
        ("jobs, j", po::value<int>(), "set maximum number of threads to use")
        ("paths", po::value<PathVec>(), "path to process");

    // Define positional options, allowing user to specify file paths without needing to use --paths flag explicitly
    po::positional_options_description positional;
    // all positional arguments are treated as paths to process, the value of '-1' means 'unlimited' 
    positional.add("paths", -1);

    po::variables_map cli_args;
    try {
        // Create a command line parser and store parsed option in cli_args
        po::command_line_parser parser(argc, argv);
        parser.options(options);
        parser.positional(positional);
        po::store(parser.run(), cli_args);
    
        // Also parse environment variables with prefix "DUCKIES_"
        po::store(po::parse_environment(options, "DUCKIES_"), cli_args);
        po::notify(cli_args);
    } catch (const std::exception &exc) {
        // If there is an error during parsing, print error message and usage information, then exit with failure
        std::cerr << exc.what() << "\n\n" << program_description << options << "\n"; 
        std::exit(EXIT_FAILURE); 
    }

    // Handle --version and --help options
    if (cli_args["version"].as<bool>()) {
        std::cerr << "Duckies version " << program_version << "\n";
        std::exit(EXIT_SUCCESS);
    }

    if (cli_args["help"].as<bool>()) {
        std::cerr << program_description << "\n\n" << options << "\n"; 
        std::exit(EXIT_SUCCESS); 
    }

    // Ensure that at least one file path is provided, otherwise print error message and usage information
    if (cli_args["paths"].empty()) {
        std::cerr << "Error: No input files provided. \n\n" << program_usage << "\n";
        std::exit(EXIT_FAILURE);
    }

    // Return parsed command-line arguments as variables_map for use in program.
    return cli_args;
}

static void setup_logging(const po::variables_map& args) {
    // Create a console logger with color support and set it as default spdlog logger. 
    // The log pattern includes timestamp, log level, and message.
    auto console_logger = spdlog::stderr_color_mt("console"); 
    console_logger->set_pattern("[%Y-%b-%d %T.%e] [%L] %v"); 
    spdlog::set_default_logger(console_logger); 

    // Set log level based on whether verbose flag is set. By default, log level is warning, but if verbose flag is set, log level is info.
    console_logger->set_level(spdlog::level::warn); // default log level is warning
    if (args["verbose"].as<bool>()) {
        console_logger->set_level(spdlog::level::info); // if verbose flag is set, log level is info
    }

    spdlog::info("Logging initialized. Verbose mode is {}.", args["verbose"].as<bool>() ? "ON" : "OFF");
}

static void setup_threading(const po::variables_map& args) {
    auto num_threads = omp_get_max_threads(); 

    if (args.count("jobs")) {
        auto requested_threads = args["jobs"].as<int>();
        num_threads = std::min(requested_threads, num_threads);
        spdlog::info("Setting maximum number of threads to {} based on user input.", num_threads);
    } else {
        // set conservative deault
        num_threads = 2; 
        spdlog::info("No thread count specified. Using default of {} threads.", num_threads);
    }
    omp_set_num_threads(num_threads);
}

struct Coordinate {
    float latitude;
    float logitude;
};

static void run_and_report(const po::variables_map& args) {
    std::vector<Coordinate> cluster_results; 

    // TODO: Implement actual logic to read input files, process data, and populate cluster_results with coordinates of duck clusters.
    // Placeholder for actual processing logic that populates cluster_results with coordinates of duck clusters.
    cluster_results.push_back({37.7749f, -122.4194f}); // Example coordinate for San Francisco
    cluster_results.push_back({34.0522f, -118.2437f}); // Example coordinate for Los Angeles
    cluster_results.push_back({40.7128f, -74.0060f});  // Example coordinate for New York City

    for (const auto& cluster: cluster_results) {
        std::cout << std::format("{: 7.3f} {: 8.3f}\n", cluster.latitude, cluster.logitude);
    }
}
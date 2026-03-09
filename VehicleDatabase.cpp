#include "VehicleDatabase.h"

#include <fstream>
#include <iostream>
#include <sstream>

VehicleRow::VehicleRow(std::vector<std::string> f) : fields(std::move(f)) { // Constructor to initialize fields and extract key info
    if (fields.size() >= 4) {
        manufacturer = trimQuotes(fields[0]);
        modelYear    = trimQuotes(fields[1]);
        vehicleType  = trimQuotes(fields[3]);
    }
}

bool VehicleDatabase::loadFromCSV(const std::string& filename) { // Loads vehicle data from a CSV file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open: " << filename << std::endl;
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) return false;

    // Parse header
    std::stringstream headerSS(line);
    std::string field;
    while (std::getline(headerSS, field, ',')) {
        headers.push_back(trimQuotes(field));
    }

    while (std::getline(file, line)) { // Loading each line of the CSV
        if (line.empty()) continue;

        std::vector<std::string> fields;
        std::stringstream ss(line);
        while (std::getline(ss, field, ',')) {
            fields.push_back(trimQuotes(field));
        }
        if (fields.size() < 4) continue;

        rows.emplace_back(std::move(fields));

        std::string key = toLowerStr(rows.back().manufacturer);
        manufacturerIndex[key].push_back(rows.size() - 1);
    }

    std::cout << "Loaded " << rows.size() << " records, "
              << manufacturerIndex.size() << " manufacturers.\n";
    return true;
}

std::vector<std::string> VehicleDatabase::getManufacturers() const { // Pushes all manufacturers into a list and sorts them alphabetically
    std::vector<std::string> list;
    for (const auto& p : manufacturerIndex) {
        if (!p.second.empty()) {
            list.push_back(rows[p.second[0]].manufacturer);
        }
    }
    std::sort(list.begin(), list.end(),
              [](const std::string& a, const std::string& b) {
                  return toLowerStr(a) < toLowerStr(b);
              });
    return list;
}

std::vector<std::string> VehicleDatabase::getVehicleTypes(const std::string& manufacturer) const { // Gets all vehicle types for a given manufacturer
    auto it = manufacturerIndex.find(toLowerStr(manufacturer));
    if (it == manufacturerIndex.end()) return {};

    std::unordered_set<std::string> types;
    for (std::size_t i : it->second) {
        const std::string& t = rows[i].vehicleType;
        if (!t.empty() && t != "-") types.insert(t);
    }

    std::vector<std::string> result(types.begin(), types.end());
    std::sort(result.begin(), result.end());
    return result;
}

bool VehicleDatabase::hasVehicle(const std::string& manufacturer, // Checks if a vehicle exists for given manufacturer, type, and year
                                 const std::string& vehicleType,
                                 const std::string& year) const {
    auto it = manufacturerIndex.find(toLowerStr(manufacturer));
    if (it == manufacturerIndex.end()) return false;

    for (std::size_t i : it->second) {
        const VehicleRow& r = rows[i];
        if (r.vehicleType == vehicleType && r.modelYear == year) {
            return true;
        }
    }
    return false;
}

const std::vector<std::string>& VehicleDatabase::getHeaders() const {
    return headers;
}

std::vector<std::string> VehicleDatabase::getVehicleRow(const std::string& manufacturer, // Gets the data row for a specific vehicle
                                                     const std::string& vehicleType,
                                                     const std::string& year) const {
    auto it = manufacturerIndex.find(toLowerStr(manufacturer));
    if (it == manufacturerIndex.end()) return {};

    for (std::size_t i : it->second) {
        const VehicleRow& r = rows[i];
        if (r.vehicleType == vehicleType && r.modelYear == year) {
            return r.fields;
        }
    }
    return {};
}

std::vector<YearRow> VehicleDatabase::getAllRowsForType(const std::string& manufacturer, // Gets all data rows for a specific manufacturer and vehicle type, for the "plot over years" feature
                                                        const std::string& vehicleType) const {
    std::vector<YearRow> result;
    auto it = manufacturerIndex.find(toLowerStr(manufacturer));
    if (it == manufacturerIndex.end()) return result;

    for (std::size_t idx : it->second) {
        const VehicleRow& r = rows[idx];
        if (r.vehicleType == vehicleType) {
            result.push_back({r.modelYear, r.fields});
        }
    }

    std::sort(result.begin(), result.end(), [](const YearRow& a, const YearRow& b) { // Adds sorting by year, handling "Prelim." as the latest year since it is preliminary data
        int ya = (a.year.find("Prelim.") != std::string::npos) ? 2024 : std::stoi(a.year);
        int yb = (b.year.find("Prelim.") != std::string::npos) ? 2024 : std::stoi(b.year);
        return ya < yb;
    });

    return result;
}
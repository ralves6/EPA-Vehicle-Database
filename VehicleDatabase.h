#ifndef VEHICLE_DATABASE_H
#define VEHICLE_DATABASE_H

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


inline std::string trimQuotes(const std::string& s) { // Trims quotes from CSV entries
    if (!s.empty() && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

inline std::string toLowerStr(const std::string& s) { // Converts string to lowercase for easier comparisons
    std::string lower = s;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lower;
}

struct YearRow {
    std::string year;
    std::vector<std::string> fields;
};

struct VehicleRow {
    std::vector<std::string> fields;
    std::string manufacturer;
    std::string modelYear;
    std::string vehicleType;

    explicit VehicleRow(std::vector<std::string> f);
};

class VehicleDatabase {
public:
    bool loadFromCSV(const std::string& filename);

    std::vector<std::string> getManufacturers() const;
    std::vector<std::string> getVehicleTypes(const std::string& manufacturer) const;
    bool hasVehicle(const std::string& manufacturer, // Checks if a vehicle exists for given manufacturer, type, and year
                    const std::string& vehicleType,
                    const std::string& year) const;
    const std::vector<std::string>& getHeaders() const;
    std::vector<std::string> getVehicleRow(const std::string& manufacturer,
                                           const std::string& vehicleType,
                                           const std::string& year) const;
    std::vector<YearRow> getAllRowsForType(const std::string& manufacturer,
                                           const std::string& vehicleType) const;

private:
    std::vector<std::string> headers;
    std::vector<VehicleRow> rows;
    std::unordered_map<std::string, std::vector<std::size_t>> manufacturerIndex;
};

#endif // VEHICLE_DATABASE_H
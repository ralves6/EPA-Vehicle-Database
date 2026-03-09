/* CSCI 200: Final Project
 *
 * Author: Raistlin Alves
 *
 * This code utilizes the EPA Vehicle Database provided by the U.S. Environmental Protection Agency to look at vehicle data and display various items to the user, including: Their annual fuel costs; Company / Vehicle emissions and performance trends over the years; Various engineering and physics calculations based on user-defined parameters.
 *
 * Assistance: Grok helped with some formatting and code structure ideas for cleaner code in different sections, as this complex data inherently had me adding tons of if statements and nested loops that made the code messy.
 */
#include "VehicleDatabase.h"

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
    // Welcome Message
    cout << "EPA Real-World Vehicle Data Lookup Tool\n";
    cout << string(70, '=') << "\n\n";

    // Load CSV File is the user wants to use a more updated EPA database
    VehicleDatabase db;
    cout << "Please enter the name of the CSV file to load (Just press enter for table_export.csv): ";
    string filename;
    getline(cin, filename);
    if (filename.empty()) {
        filename = "table_export.csv";
    }
    if (!db.loadFromCSV(filename)) {
        cerr << "Failed to load the file!\n";
        return 1;
    }
    cout << "Success!\n\n";

    // Manufacturer Selection
    vector<string> manufacturers = db.getManufacturers();
    cout << "Available Manufacturers:\n";
    cout << string(70, '-') << '\n';
    for (size_t i = 0; i < manufacturers.size(); ++i) {
        cout << left << setw(20) << manufacturers[i];
        if ((i + 1) % 4 == 0 || i == manufacturers.size() - 1) cout << '\n';
    }
    cout << string(70, '-') << "\n\n";

    cout << "MANUFACTURER REFERENCE GUIDE (Some Companies Have Parents!)" << endl; // Formatting so users can find manufacturers that are subsidiaries
    cout << setw(40) << "BMW" << ": Mini, Rolls Royce" << endl;
    cout << setw(40) << "Ford" << ": Lincoln, Roush, Shelby" << endl;
    cout << setw(40) << "General Motors(GM)" << ": Chevrolet, GMC, Cadillac, Buick" << endl;
    cout << setw(40) << "Honda" << ": Acura" << endl;
    cout << setw(40) << "Hyundai" << ": Genesis" << endl;
    cout << setw(40) << "Mercedes" << ": Maybach" << endl;
    cout << setw(40) << "Nissan" << ": Infiniti" << endl;
    cout << setw(40) << "Stellantis" << ": Alfa Romeo, Chrysler, Dodge, Fiat, Jeep, Maserati, Ram" << endl;
    cout << setw(40) << "Toyota" << ": Lexus" << endl;
    cout << setw(40) << "Volkswagon (VW)" << ": Audi, Bentley, Bugatti, Lamborghini, Porshe" << endl << endl;
    cout << "Enter manufacturer (Case Sensitive, Defaults to All): ";
    string manInput;
    getline(cin, manInput);
    if (manInput == "Mini" || manInput == "Rolls Royce") manInput = "BMW"; // Handling parent companies for more user convenience
    else if (manInput == "Lincoln" || manInput == "Roush" || manInput == "Shelby") manInput = "Ford";
    else if (manInput == "Chevrolet" || manInput == "GMC" || manInput == "Cadillac" || manInput == "Buick") manInput = "General Motors";
    else if (manInput == "Acura") manInput = "Honda";
    else if (manInput == "Genesis") manInput = "Hyundai";
    else if (manInput == "Maybach") manInput = "Mercedes";
    else if (manInput == "Infiniti") manInput = "Nissan";
    else if (manInput == "Alfa Romeo" || manInput == "Chrysler" || manInput == "Dodge" || manInput == "Fiat" || manInput == "Jeep" || manInput == "Maserati" || manInput == "Ram") manInput = "Stellantis";
    else if (manInput == "Lexus") manInput = "Toyota";
    else if (manInput == "Audi" || manInput == "Bentley" || manInput == "Bugatti" || manInput == "Lamborghini" || manInput == "Porshe") manInput = "Volkswagon";
    string selectedMan = manufacturers[0];
    for (const string& m : manufacturers) {
        if (toLowerStr(m).find(toLowerStr(manInput)) != string::npos) {
            selectedMan = m;
            break;
        }
    }
    cout << "Selected: " << selectedMan << "\n\n";

    // Vehicle Type Selection, unfortunetly EPA doesn't do vehicle models, only types
    vector<string> types = db.getVehicleTypes(selectedMan);
    cout << "Vehicle Types (" << types.size() << "):\n";
    cout << string(60, '-') << '\n';
    for (size_t i = 0; i < types.size(); ++i) {
        cout << "  " << (i + 1) << ". " << types[i] << '\n';
    }
    cout << string(60, '-') << "\n\n";

    cout << "Enter number (Defaults to 1): ";
    string typeNumStr;
    getline(cin, typeNumStr);
    int typeChoice = 1;
    try { typeChoice = stoi(typeNumStr); } catch (...) {}
    if (typeChoice < 1 || typeChoice > (int)types.size()) typeChoice = 1;
    string selectedType = types[typeChoice - 1];
    cout << "Selected: " << selectedType << "\n\n";

    // Options Menu, letting user pick what they want to do for as many times as they want
    int option;
    bool again = true;
    while (again == true) {
        while (true) {
            cout << "What would you like to explore?\n\n";
            cout << "  1. How much money am I spending on gas?\n";
            cout << "  2. How is my car model / company doing over the years?\n";
            cout << "  3. What engineering values can we gather?\n\n";
            cout << "Enter 1, 2, or 3: ";
            string optStr;
            getline(cin, optStr);
            try {
                option = stoi(optStr);
                if (option >= 1 && option <= 3) break;
            } catch (...) {}
            cout << "Invalid, try again.\n\n";
        }

        // Year Input (only for 1 and 3)
        string year;
        if (option == 1 || option == 3) {
            cout << "\nEnter model year: ";
            getline(cin, year);
        }

        // Utilizing a switch for the three options
        switch (option) {
            case 1: { // Gas Cost Calculator
                if (year.empty() || !db.hasVehicle(selectedMan, selectedType, year)) {
                    cout << "\nNo data found for that year.\n";
                    break;
                }

                vector<string> row = db.getVehicleRow(selectedMan,selectedType, year);

                cout << "\nOption 1: Gas Cost Calculator\n";
                cout << string(50, '-') << "\n\n";

                cout << "1. Use EPA average MPG\n2. Estimate MPG using your City/Highway mix\n" ;
                string choice;
                getline(cin, choice);
                double mpg;
                if (toLowerStr(choice).find("1") != string::npos) {
                    mpg = stod(row[7]);
                    cout << "Using average MPG: " << mpg << "\n\n";
                } else {
                    cout << "City driving %: ";
                    string cityStr;
                    getline(cin, cityStr);
                    double cityPct = 55.0;
                    try { cityPct = stod(cityStr); } catch (...) {}
                    double hwyPct = 100 - cityPct;
                    double cityMpg = stod(row[8]);
                    double hwyMpg = stod(row[9]);
                    mpg = (cityPct/100.0)*cityMpg + (hwyPct/100.0)*hwyMpg;
                    cout << "Estimated MPG: " << mpg << "\n\n";
                }

                cout << "1. Use EPA average miles per year\n2. Give your miles per year\n";
                string milesStr;
                getline(cin, milesStr);
                double miles = 15000.0;
                if (toLowerStr(milesStr) != "1") {
                    cout << "Enter your estimated miles per year: ";
                    getline(cin, milesStr);


                }

                cout << "Gas price per gallon: ";
                string priceStr;
                getline(cin, priceStr);
                double price = 3.50;
                try { price = stod(priceStr); } catch (...) {}

                double cost = (miles / mpg) * price;
                cout << fixed << setprecision(2);
                cout << "\nAnnual gas cost: $" << cost << "\n\n";
                break;
            }

            case 2: { // Model Trends Over the Years
                cout << "\nOption 2: Model Trends Over the Years\n";
                cout << string(60, '-') << "\n\n";

                vector<YearRow> yearRows = db.getAllRowsForType(selectedMan, selectedType);
                if (yearRows.empty()) {
                    cout << "No data.\n";
                    break;
                }

                const vector<string>& h = db.getHeaders();

                auto col = [&h](const string& name) -> int {
                    for (size_t i = 0; i < h.size(); ++i)
                        if (toLowerStr(h[i]).find(toLowerStr(name)) != string::npos)
                            return static_cast<int>(i);
                    return -1;
                };

                int co2 = col("real-world co2 (g/mi)"); // Grok assisted me in formatting this psudo-graph, as I was struggling to get it to look nice, and using an external library like MATLAB would've been a headache
                int wt  = col("weight (lbs)");
                int hp  = col("horsepower");
                int mpg = col("real-world mpg") && toLowerStr(h[col("real-world mpg")]).find("city") == string::npos ? col("real-world mpg") : -1;
                int acc = col("acceleration");
                int fp  = col("footprint");

                cout << left
                    << setw(18)  << "Year"
                    << setw(22) << "CO2 (g/mi)"
                    << setw(22) << "Weight (lbs)"
                    << setw(20) << "HP"
                    << setw(20) << "MPG"
                    << setw(22) << "0-60 (s)"
                    << setw(22) << "Footprint (sq ft)" << '\n';
                cout << string(120, '-') << '\n';

                for (const auto& r : yearRows) {
                    auto v = [&](int i) { return i>=0 && i<(int)r.fields.size() ? r.fields[i] : "-"; }; // Grok also assisted with this searching method to make it cleaner instead of multiple if statements
                    cout << left
                        << setw(18)  << r.year
                        << setw(22) << v(co2)
                        << setw(22) << v(wt)
                        << setw(20) << v(hp)
                        << setw(20) << v(mpg)
                        << setw(22) << v(acc)
                        << setw(22) << v(fp) << '\n';
                }
                cout << string(120, '-') << '\n';
                break;
            }

            case 3: { // Engineering & Physics Calculations
                cout << "\nOption 3: Vehicle Engineering & Physics Calculations\n";
                cout << string(70, '-') << "\n\n";

                // Get vehicle data for this year
                if (year.empty() || !db.hasVehicle(selectedMan, selectedType, year)) {
                    cout << "No data found for " << year << " " << selectedMan << " " << selectedType << ".\n";
                    break;
                }

                vector<string> row = db.getVehicleRow(selectedMan, selectedType, year);
                const vector<string>& headers = db.getHeaders();

                // Since the user is interested in data, ask them if they want to see everything we can grab
                cout << "Would you like to see all raw EPA data for this vehicle before calculations? (y/n, defaults to n): ";
                string showRaw;
                getline(cin, showRaw);

                if (toLowerStr(showRaw).find("y") != string::npos) {
                    cout << "\n" << string(80, '=') << "\n";
                    cout << "RAW EPA DATA: " << selectedMan << " " << selectedType << " " << year << "\n";
                    cout << string(80, '=') << "\n";
                    for (size_t i = 0; i < headers.size(); ++i) {
                        string val = (i < row.size()) ? row[i] : "(no data)";
                        cout << left << setw(45) << headers[i] << ": " << val << "\n";
                    }
                    cout << string(80, '=') << "\n\n";
                }

                // Default engineering parameters by vehicle type
                struct VehicleParams {
                    double Cd, frontalArea, wheelbase, frontWeightFrac;
                    double cgHeight, wheelRadius, wheelMass, tireRadius, tireMass;
                    double mu, transEff;
                };

                map<string, VehicleParams> defaults = { // This long section utilized various online resources to gather average vehicle parameters for different vehicle types, so even if the user doesn't want to input their own data, they can still get a reasonable estimate
                    {"All",           {0.375, 2.5,  2.9,  0.54, 0.60,  0.365, 25, 0.325, 12, 0.825, 0.89}},
                    {"All Car",       {0.285, 2.15, 2.75, 0.56, 0.525, 0.32,  20, 0.30,  10, 0.825, 0.90}},
                    {"All Truck",     {0.475, 2.95, 3.25, 0.50, 0.725, 0.40,  32, 0.38,  17, 0.80,  0.84}},
                    {"Car SUV",       {0.36,  2.7,  2.9,  0.53, 0.65,  0.36,  24, 0.34,  14, 0.80,  0.88}},
                    {"Minivan/Van",   {0.36,  2.8,  3.05, 0.515,0.65,  0.375, 25, 0.33,  13, 0.80,  0.86}},
                    {"Pickup",        {0.455, 2.9,  3.3,  0.50, 0.725, 0.40,  31, 0.39,  18, 0.80,  0.84}},
                    {"Sedan/Wagon",   {0.28,  2.15, 2.75, 0.56, 0.525, 0.32,  20, 0.30,  10, 0.825, 0.90}},
                    {"Truck SUV",     {0.49,  2.9,  3.15, 0.51, 0.725, 0.395, 32, 0.38,  17, 0.80,  0.84}}
                };

                string bestKey = "All";
                for (const auto& p : defaults) {
                    if (toLowerStr(selectedType).find(toLowerStr(p.first)) != string::npos) {
                        bestKey = p.first;
                        break;
                    }
                }
                VehicleParams p = defaults[bestKey];

                cout << "Using defaults for: \"" << bestKey << "\"\n\n";

                // User override for each parameter
                auto ask = [&](const string& name, double& val, const string& unit = "") {
                    cout << name << " [" << val << unit << "] Keep or change? (enter new value or press Enter): ";
                    string in;
                    getline(cin, in);
                    if (!in.empty()) {
                        try { val = stod(in); cout << "  Set to " << val << unit << "\n"; }
                        catch (...) { cout << "  Invalid, keeping " << val << unit << "\n"; }
                    }
                };

                ask("Drag Coefficient (Cd)", p.Cd); // Using the same formatting Grok helped me with earlier
                ask("Frontal Area (m^2)", p.frontalArea, " m^2");
                ask("Wheelbase (m)", p.wheelbase, " m");
                ask("Front Axle Weight Fraction", p.frontWeightFrac);
                ask("Center of Gravity Height (m)", p.cgHeight, " m");
                ask("Wheel Radius (m)", p.wheelRadius, " m");
                ask("Wheel Mass (kg)", p.wheelMass, " kg");
                ask("Tire Radius (m)", p.tireRadius, " m");
                ask("Tire Mass (kg)", p.tireMass, " kg");
                ask("Friction Coefficient (mu)", p.mu);
                ask("Transmission Efficiency", p.transEff);

                // Constants
                const double g = 9.81;
                const double airDensity = 1.2;
                const double rollingResist = 0.007;

                // Safe CSV reading, Grok helped me with this lambda function to make it cleaner
                auto safe_stod = [](const string& s, double fallback = 0.0) -> double {
                    if (s.empty() || s == "-" || s == "−" || s == "--") return fallback;
                    try { return stod(s); }
                    catch (...) { return fallback; }
                };
                // Finding values the EPA gives us that we can actually use for calculations
                double weightLbs = safe_stod(row[13], 4000.0);
                double weightKg = weightLbs * 0.453592;
                double horsepower = safe_stod(row[16], 300.0);

                string driveType = "Unknown";
                double front = safe_stod(row[23], 0.0);
                double fourwd = safe_stod(row[24], 0.0);
                double rear = safe_stod(row[25], 0.0);

                if (fourwd > 0.0) driveType = "4WD/AWD";
                else if (front > 0.0) driveType = "FWD";
                else if (rear > 0.0) driveType = "RWD";

                // User inputs for conditions like wind and grade
                cout << "\nRoad & Wind Conditions:\n";
                cout << string(40, '-') << "\n";

                cout << "Is there wind? (y/n): ";
                string windInput;
                getline(cin, windInput);
                double windSpeed = 0.0;
                if (toLowerStr(windInput).find("y") != string::npos) {
                    cout << "Wind speed (m/s): ";
                    string ws;
                    getline(cin, ws);
                    try { windSpeed = stod(ws); } catch (...) { windSpeed = 0; }

                    cout << "Is it headwind or tailwind? (h/t): ";
                    string dir;
                    getline(cin, dir);
                    if (toLowerStr(dir).find("t") != string::npos) {
                        windSpeed = -windSpeed;
                    }
                    cout << "Wind speed set to: " << windSpeed << " m/s\n";
                } else {
                    cout << "No wind.\n";
                }

                cout << "Vehicle speed (mph): ";
                string speedStr;
                getline(cin, speedStr);
                double v_mph = 0.0;
                try { v_mph = stod(speedStr); } catch (...) { v_mph = 0; }
                double v = v_mph * 0.44704;  // mph → m/s

                cout << "Road grade (%, e.g. 5 for 5% uphill, -3 for downhill): ";
                string gradeStr;
                getline(cin, gradeStr);
                double gradePercent = 0.0;
                try { gradePercent = stod(gradeStr); } catch (...) {}

                // Physics Calculations, see MEGN 391 Introduction to Automotive Design if you are more interested about how these equations are made!
                double powerW = horsepower * 745.7;

                double thetaRad = atan(gradePercent / 100.0);
                double thetaDeg = thetaRad * 180.0 / 3.1415926535;

                double vRel = v + windSpeed;
                double F_aero = 0.5 * airDensity * p.Cd * p.frontalArea * vRel * vRel;
                double F_grade = weightKg * g * sin(thetaRad);
                double F_roll = weightKg * g * rollingResist * cos(thetaRad);

                double I_tire = 0.5 * p.tireMass * (p.tireRadius * p.tireRadius);
                double I_wheel = 0.5 * p.wheelMass * (p.wheelRadius * p.wheelRadius);
                double I_total_rotational = 4.0 * (I_tire + I_wheel);
                double equivMass = weightKg + (I_total_rotational / (p.wheelRadius * p.wheelRadius));

                double P_aero = F_aero * v;
                double P_roll = F_roll * v;
                double P_grade = F_grade * v;
                double P_available = powerW * p.transEff;
                double P_motor = powerW;

                double lr = p.wheelbase * p.frontWeightFrac;
                double lf = p.wheelbase - lr;
                double F_friction = 0;
                if (driveType == "FWD") {
                    F_friction = (p.mu * (lr / p.wheelbase) * weightKg * g)/ (1 + (p.mu * p.cgHeight / p.wheelbase));
                } else if (driveType == "RWD") {
                    F_friction = (p.mu * (lf / p.wheelbase) * weightKg * g)/ (1 - (p.mu * p.cgHeight / p.wheelbase));
                } else if (driveType == "4WD/AWD") {
                    F_friction = (p.mu * (lr / p.wheelbase) * weightKg * g)/ (1 + (p.mu * p.cgHeight / p.wheelbase)) +
                                (p.mu * (lf / p.wheelbase) * weightKg * g)/ (1 - (p.mu * p.cgHeight / p.wheelbase));
                }
                double P_wheel_friction = F_friction * v;

                bool traction_limited = false;
                if (P_available > P_wheel_friction) {
                    traction_limited = true;
                    P_available = P_wheel_friction;
                }
                double a = (v > 0.01) ? (P_available - P_aero - P_roll - P_grade) / (equivMass * v) : 0.0;

                // Final Output
                cout << "\n" << string(70, '=') << "\n";
                cout << "PHYSICS RESULTS (" << selectedMan << " " << selectedType << " " << year << ")\n";
                cout << string(70, '=') << "\n";
                cout << fixed << setprecision(3);

                if (traction_limited) {
                    cout << left << setw(40)
                    << "YOU ARE TRACTION LIMITED"
                    << ": Power Available Is Limited By Traction\n";

                if (!traction_limited) cout << left << setw(40)
                    << "YOU ARE POWERTRAIN LIMITED"
                    << ": Power Available Is Limited By Motor and Transmission Efficiency\n";

                cout << left << setw(40) << "Horsepower (hp)" << ": " << horsepower << "\n";
                cout << left << setw(40) << "Total Power Available (W)" << ": " << P_available << "\n";
                cout << left << setw(40) << "Power Rejected as Heat (W)" << ": " << P_motor * (1 - p.transEff) << "\n\n";

                cout << left << setw(40) << "Vehicle Speed (m/s)" << ": " << v << " (mph) " << v_mph << "\n";
                cout << left << setw(40) << "Traction to Powertrain Transition Speed (m/s)" << ": " << (P_motor*p.transEff)/F_friction << " mph (" << ((P_motor*p.transEff)/F_friction)/2.237 << ")\n";

                cout << left << setw(40) << "Wind Speed (m/s, -head/+tail)" << ": " << windSpeed << "\n";
                cout << left << setw(40) << "Road Grade (%)" << ": " << gradePercent << " (" << thetaDeg << " degrees)\n";
                cout << left << setw(40) << "Mass (kg)" << ": " << weightKg << "\n";
                cout << left << setw(40) << "Drive Type" << ": " << driveType << "\n\n";

                cout << "Forces:\n";
                cout << left << setw(40) << "  Aerodynamic Drag" << ": " << F_aero << " N\n";
                cout << left << setw(40) << "  Grade Force" << ": " << F_grade << " N\n";
                cout << left << setw(40) << "  Rolling Resistance" << ": " << F_roll << " N\n\n";

                cout << "Power Losses:\n";
                cout << left << setw(40) << "  Aero Power Loss" << ": " << P_aero << " W\n";
                cout << left << setw(40) << "  Rolling Power Loss" << ": " << P_roll << " W\n";
                cout << left << setw(40) << "  Grade Power Loss" << ": " << P_grade << " W\n\n";

                cout << left << setw(40) << "Equivalent Mass (kg)" << ": " << equivMass << "\n";
                cout << left << setw(40) << "Theoretical Acceleration" << ": " << a << " m/s^2\n";
                if (a > 0) {
                    double timeTo60 = 26.8224 / a;
                    cout << left << setw(40) << "Estimated 0-60 mph time" << ": " << timeTo60 << " seconds\n";
                }

                cout << string(70, '=') << "\n\n";
                cout << "Physics calculations complete!\n";
                break;
            }
            }
        }
        cout << "Would you like to perform another analysis? (y/n, defaults to n): "; // Asking user if they want to go again, saves time from reloading the program
        string againInput;
        getline(cin, againInput);
        if (toLowerStr(againInput).find("y") == string::npos) {
            again = false;
        }
    }

    cout << "\nPress Enter to exit.";
    string dummy;
    getline(cin, dummy);
    return 0;
}
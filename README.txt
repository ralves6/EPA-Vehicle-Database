This program combines the U.S. EPA’s Automotive Trends Report real-world dataset with vehicle modeling (Using the Road Load Equations from MEGN 391: Introduction to Automotive) to give you accurate insights into any vehicle, vehicle model, or company in the United States from 1975 to 2023 (and some preliminary 2024 data).
Unlike the familiar “window sticker” MPG numbers, this tool uses the EPA’s adjusted real-world fuel economy and CO₂ figures — the same numbers the government uses for regulatory analysis and manufacturer CAFE compliance.
With this code, you can:
Look up any manufacturer, model year, and vehicle class (Commercial Vehicles, no Ferrari, McLaren, or similar)
See all 60+ data fields the EPA provides in a more absorbable format (powertrain mix, drivetrain distribution, turbo prevalence, transmission gearing, etc.)
Calculate personal annual fuel cost using either the combined real-world MPG or a custom city/highway split
Explore multi-year trends on certain vehicle types, vehicle companies, or for all cars in general!
Perform engineering-based performance calculations (0–60 mph, traction vs. powertrain limits, power losses, etc.)

FOR FUTURE USE:
If you want to expand the dataset (If the EPA releases 1975 - 2026 for example), you will need to take the following precautions:
1. Place the csv file in the same folder path as either the .exe file (If not compiling the code), or the main.cpp/Makefile/VehicleDatabase.hpp'
2. You must download the "Detailed Data by Manufacturer" data csv from the EPA. This can be found at https://www.epa.gov/automotive-trends/explore-automotive-trends-data#DetailedData
    If this link fails in the future, you might be able to also find it through the main EPA page and taking the following actions:
    1. Go to the main page and search "Automotive"
    2. Select "The EPA Automotive Trends Report"
    3. Click "Download Report"
    4. Click the dropdown "Automotive Trends Data" on the left toolbar and select "Explore the Automotive Trends Data"
    5. Click "Detailed Automotive Trends Data" (Has to be detailed data for code to work)
    6. Wait for the Web Viewer to load, and click "Export Detailed Data by Manufacturer (csv)
    7. Place downloaded csv in the same filepath as .exe or compiling files
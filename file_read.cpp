#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Struct to hold screening data
struct ScreeningData {
    string movieName;
    string date;
    string hour;
    double price;
    vector<vector<bool>> occupancy;
};

// Function to read screening data from file
ScreeningData readScreeningData(const string& filename) {
    ifstream file(filename);
    ScreeningData data;

    if (file.is_open()) {
        string line;

        // Reading movie name, date, hour
        getline(file, line); // Movie
        data.movieName = line.substr(line.find(":") + 2);
        
        getline(file, line); // Date
        data.date = line.substr(line.find(":") + 2);

        getline(file, line); // Hour
        data.hour = line.substr(line.find(":") + 2);

        // Reading price
        getline(file, line); // Price
        size_t pos = line.find(":");
        if (pos != string::npos) {
            string priceStr = line.substr(pos + 1); // Extract price string
            // Remove leading and trailing whitespace
            priceStr.erase(0, priceStr.find_first_not_of(" \t\n\r\f\v"));
            priceStr.erase(priceStr.find_last_not_of(" \t\n\r\f\v") + 1);
            // Remove square brackets
            priceStr.erase(remove(priceStr.begin(), priceStr.end(), '['), priceStr.end());
            priceStr.erase(remove(priceStr.begin(), priceStr.end(), ']'), priceStr.end());
            try {
                data.price = stod(priceStr);
            } catch (const std::invalid_argument& e) {
                cerr << "Invalid price format: " << priceStr << endl;
                data.price = 0.0; // Default value
            }
        } else {
            cerr << "Invalid format for price" << endl;
            data.price = 0.0; // Default value
        }

        // Skipping empty line
        getline(file, line);

        // Reading occupancy
        data.occupancy.clear();
        while (getline(file, line)) {
            if (line.find("Occupancy:") != string::npos) {
                continue; // Skip the line containing "Occupancy:"
            }
            vector<bool> row;
            istringstream iss(line);
            string value;
            while (getline(iss, value, ',')) {
                if (!value.empty()) {
                    bool isOccupied = (value == "1");
                    row.push_back(isOccupied);
                }
            }
            // Remove the last element if it's an empty string (due to the comma at the end of each row)
            if (!row.empty() && row.back() == false) {
                row.pop_back();
            }
            data.occupancy.push_back(row);
        }

        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }

    return data;
}

int main() {
    // Prompt the user to enter the file name
    cout << "Enter the name of the file (without the '.txt' extension): ";
    string filename;
    cin >> filename;

    // Add the ".txt" extension to the file name
    filename += ".txt";
    ScreeningData screeningData = readScreeningData(filename);

    // Displaying read data
    cout << "Movie: " << screeningData.movieName << endl;
    cout << "Date: " << screeningData.date << endl;
    cout << "Hour: " << screeningData.hour << endl;
    // Display price with two decimal places
    cout.precision(2);
    cout << fixed << "Price: " << screeningData.price << endl;

    cout << "Occupancy:" << endl;
    for (const auto& row : screeningData.occupancy) {
        for (bool seat : row) {
            cout << seat << " ";
        }
        cout << endl;
    }

    return 0;
}

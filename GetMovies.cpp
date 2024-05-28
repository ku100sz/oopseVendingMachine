#include "other.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Class implementations

Screening::Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy)
    : movieName(movieName), date(date), hour(hour), price(price), occupancy(occupancy) {
    totalOccupiedSeats = count(occupancy.begin(), occupancy.end(), true);
}

// Overloaded constructor with default values
Screening::Screening()
    : movieName("Unknown"), date("Unknown"), hour("Unknown"), price(0.0), occupancy(30, false), totalOccupiedSeats(0) {}

void Screening::display() const {
    display(true); // Default to showing price
}

// Overloaded display method
void Screening::display(bool showPrice) const {
    cout << "Movie: " << movieName << endl;
    cout << "Date: " << date << endl;
    cout << "Hour: " << hour << endl;
    if (showPrice) {
        cout.precision(2);
        cout << fixed << "Price: " << price << " PLN" << endl;
    }
    cout << "Total Occupied Seats: " << totalOccupiedSeats << endl;
    cout << "Occupancy:" << endl;

    // Display column headers
    cout << "  A B C D E" << endl;

    // Display grid with numbered rows
    for (int row = 0; row < 6; ++row) {
        cout << row + 1 << " ";
        for (int col = 0; col < 5; ++col) {
            int index = row * 5 + col;
            if (index < occupancy.size()) {
                cout << occupancy[index] << " ";
            } else {
                cout << "0 "; // Default to 0 if index is out of bounds
            }
        }
        cout << endl;
    }
}

string Screening::getMovieName() const {
    return movieName;
}

int Screening::getTotalOccupiedSeats() const {
    return totalOccupiedSeats;
}

bool Screening::isSeatAvailable(int row, char col) const {
    int colIndex = toupper(col) - 'A';
    int index = (row - 1) * 5 + colIndex;
    return (index >= 0 && index < occupancy.size() && !occupancy[index]);
}

void Screening::occupySeat(int row, char col) {
    int colIndex = toupper(col) - 'A';
    int index = (row - 1) * 5 + colIndex;
    if (index >= 0 && index < occupancy.size() && !occupancy[index]) {
        occupancy[index] = true;
        totalOccupiedSeats++;
    }
}

double Screening::getPrice() const {
    return price;
}

void Screening::saveToFile(ofstream& file) const {
    file << movieName << "," << date << "," << hour << "," << price << ",";
    for (size_t i = 0; i < occupancy.size(); ++i) {
        file << (occupancy[i] ? "1" : "0");
        if (i < occupancy.size() - 1) {
            file << ",";
        }
    }
    file << endl;
}

void Theater::addScreening(const Screening& screening) {
    screenings.push_back(screening);
}

void Theater::loadScreeningsFromFile(const string& filename) {
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        bool isFirstLine = true;

        while (getline(file, line)) {
            if (isFirstLine) {
                isFirstLine = false;
                continue; // Skip header line
            }

            stringstream lineStream(line);
            string movieName, date, hour, cell;
            double price;
            vector<bool> occupancy;

            // Read movie name
            getline(lineStream, movieName, ',');

            // Read date
            getline(lineStream, date, ',');

            // Read hour
            getline(lineStream, hour, ',');

            // Read price
            getline(lineStream, cell, ',');
            price = stod(cell);

            // Read occupancy
            while (getline(lineStream, cell, ',')) {
                occupancy.push_back(cell == "1");
            }

            Screening screening(movieName, date, hour, price, occupancy);
            addScreening(screening);
        }

        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

void Theater::saveScreeningsToFile(const string& filename) const {
    ofstream file(filename);

    if (file.is_open()) {
        // Write header line
        file << "Name,Date,Hour,Price,Occupancy" << endl;

        // Write each screening to file
        for (const auto& screening : screenings) {
            screening.saveToFile(file);
        }

        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

vector<Screening>& Theater::getScreenings() {
    return screenings;
}

Theater getMovies() {
    string filename = "movies"; 
    Theater theater;
    theater.loadScreeningsFromFile(filename);
    return theater;
}

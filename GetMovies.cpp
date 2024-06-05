#include "other.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Implementation of Movie methods

Movie::Movie() : movieName("Not specified"), genre("None"), minAge(0) {}

Movie::Movie(const string& name, const string& genre, int minAge)
    : movieName(name), genre(genre), minAge(minAge) {}

string Movie::getMovieName() const {
    return movieName;
}

void Movie::setMovieName(const string& name) {
    movieName = name;
}

string Movie::getGenre() const {
    return genre;
}

void Movie::setGenre(const string& genre) {
    this->genre = genre;
}

int Movie::getMinAge() const {
    return minAge;
}

void Movie::setMinAge(int age) {
    if (age >= 0 && age <= 120) {
        minAge = age;
    } else {
        cout << "Invalid age value assigned." << endl;
    }
}

// Implementation of Screening methods

Screening::Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy)
    : Movie(movieName, "None", 0), date(date), hour(hour), price(price), occupancy(occupancy), totalOccupiedSeats(0), totalPrice(0.0) {
    totalOccupiedSeats = count(occupancy.begin(), occupancy.end(), true);
}

Screening::Screening()
    : Movie("Unknown", "None", 0), date("Unknown"), hour("Unknown"), price(0.0), occupancy(30, false), totalOccupiedSeats(0), totalPrice(0.0) {}

void Screening::display() const {
    display(true); // Default to showing price
}

void Screening::display(bool showPrice) const {
    cout << "Movie: " << movieName << endl;
    cout << "Genre: " << getGenre() << endl;
    cout << "Min Age: " << getMinAge() << endl;
    cout << "Date: " << date << endl;
    cout << "Hour: " << hour << endl;
    if (showPrice) {
        cout.precision(2);
        cout << fixed << "Price: " << price << " PLN" << endl;
    }
    int availableSeats = 30 - totalOccupiedSeats;
    cout << "Available Seats: " << availableSeats << endl;
    cout << "Occupancy:" << endl;

    // Display column headers
    cout << "  A B C D E" << endl;

    // Display grid with numbered rows
    for (int row = 0; row < 6; ++row) {
        cout << row + 1 << " ";
        for (int col = 0; col < 5; ++col) {
            int index = row * 5 + col;
            if (index < occupancy.size()) {
                cout << (occupancy[index] ? "X" : "O") << " "; // 'X' for occupied, 'O' for available
            } else {
                cout << "0 "; // Default to 0 if index is out of bounds
            }
        }
        cout << endl;
    }
}

string Screening::getDate() const {
    return date;
}

void Screening::setDate(const string& date) {
    this->date = date;
}

string Screening::getHour() const {
    return hour;
}

void Screening::setHour(const string& hour) {
    this->hour = hour;
}

double Screening::getPrice() const {
    return price;
}

void Screening::setPrice(double price) {
    int movieHour = stoi(hour.substr(0, 2));
    if (movieHour < 15) {
        price *= 0.7; // Apply 30% discount
    }
    if (price >= 0) { // Example condition
        this->price = price;
    }
}

vector<bool> Screening::getOccupancy() const {
    return occupancy;
}

void Screening::setOccupancy(const vector<bool>& occupancy) {
    this->occupancy = occupancy;
    totalOccupiedSeats = count(occupancy.begin(), occupancy.end(), true); // Recalculate total occupied seats
}

int Screening::getTotalOccupiedSeats() const {
    return totalOccupiedSeats;
}

void Screening::setTotalOccupiedSeats(int totalOccupiedSeats) {
    if (totalOccupiedSeats >= 0) {
        this->totalOccupiedSeats = totalOccupiedSeats;
    }
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

void Screening::calculateAndSetTotalPrice(int numReducedTickets, int numNormalTickets) {
    double reducedPrice = price * 0.5;
    totalPrice = (numReducedTickets * reducedPrice) + (numNormalTickets * price);
    setPrice(totalPrice);
}

// Theater class implementations

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

            Screening screening;
            screening.setMovieName(movieName);
            screening.setDate(date);
            screening.setHour(hour);
            screening.setPrice(price);
            screening.setOccupancy(occupancy);

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

// Method to set the genre for a movie
void Theater::setGenreForMovie(const string& movieName, const string& genre) {
    for (auto& screening : screenings) {
        if (screening.getMovieName() == movieName) {
            screening.setGenre(genre);
        }
    }
}

// Method to set the minimum age for a movie
void Theater::setMinAgeForMovie(const string& movieName, int minAge) {
    for (auto& screening : screenings) {
        if (screening.getMovieName() == movieName) {
            screening.setMinAge(minAge);
        }
    }
}

// Function to load movies from the file
Theater getMovies() {
    string filename = "movies";
    Theater theater;
    theater.loadScreeningsFromFile(filename);
    return theater;
}

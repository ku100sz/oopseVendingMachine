#include "other.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

// Default constructor for the Movie class
// Initializes the movie with default values
Movie::Movie() : movieName("Not specified"), genre("None"), minAge(0) {}

// Parameterized constructor for the Movie class
// Initializes the movie with provided values
Movie::Movie(const string& name, const string& genre, int minAge)
    : movieName(name), genre(genre), minAge(minAge) {}

// Getter for movieName
string Movie::getMovieName() const {
    return movieName;
}

// Setter for movieName
void Movie::setMovieName(const string& name) {
    movieName = name;
}

// Getter for genre
string Movie::getGenre() const {
    return genre;
}

// Setter for genre
void Movie::setGenre(const string& genre) {
    this->genre = genre;
}

// Getter for minAge
int Movie::getMinAge() const {
    return minAge;
}

// Setter for minAge with validation
void Movie::setMinAge(int age) {
    if (age >= 0 && age <= 120) {
        minAge = age;
    } else {
        cout << "Invalid age value assigned." << endl;
    }
}

// Parameterized constructor for the Screening class
// Initializes the screening with provided values and counts occupied seats
Screening::Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy)
    : Movie(movieName, "None", 0), date(date), hour(hour), price(price), occupancy(occupancy), totalOccupiedSeats(0), totalPrice(0.0) {
    totalOccupiedSeats = count(occupancy.begin(), occupancy.end(), true);
}

// Default constructor for the Screening class
// Initializes the screening with default values
Screening::Screening()
    : Movie("Unknown", "None", 0), date("Unknown"), hour("Unknown"), price(0.0), occupancy(30, false), totalOccupiedSeats(0), totalPrice(0.0) {}

// Display method with default option to show price
// Displays screening details including movie information
void Screening::display() const {
    display(true); // Default to showing price
}

// Overloaded display method
// Optionally displays the price along with screening details
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

// Getter for date
string Screening::getDate() const {
    return date;
}

// Setter for date
void Screening::setDate(const string& date) {
    this->date = date;
}

// Getter for hour
string Screening::getHour() const {
    return hour;
}

// Setter for hour
void Screening::setHour(const string& hour) {
    this->hour = hour;
}

// Getter for price
double Screening::getPrice() const {
    return price;
}

// Setter for price with discount logic
// Applies a 30% discount if the screening is before 15:00
void Screening::setPrice(double price) {
    int movieHour = stoi(hour.substr(0, 2));
    if (movieHour < 15) {
        price *= 0.7; // Apply 30% discount
    }
    if (price >= 0) { // Example condition
        this->price = price;
    }
}

// Getter for occupancy
vector<bool> Screening::getOccupancy() const {
    return occupancy;
}

// Setter for occupancy
// Updates the occupancy status and recalculates the total occupied seats
void Screening::setOccupancy(const vector<bool>& occupancy) {
    this->occupancy = occupancy;
    totalOccupiedSeats = count(occupancy.begin(), occupancy.end(), true); // Recalculate total occupied seats
}

// Getter for totalOccupiedSeats
int Screening::getTotalOccupiedSeats() const {
    return totalOccupiedSeats;
}

// Setter for totalOccupiedSeats
void Screening::setTotalOccupiedSeats(int totalOccupiedSeats) {
    if (totalOccupiedSeats >= 0) {
        this->totalOccupiedSeats = totalOccupiedSeats;
    }
}

// Check if a seat is available
// Returns true if the seat at the specified row and column is available
bool Screening::isSeatAvailable(int row, char col) const {
    int colIndex = toupper(col) - 'A';
    int index = (row - 1) * 5 + colIndex;
    return (index >= 0 && index < occupancy.size() && !occupancy[index]);
}

// Mark a seat as occupied
void Screening::occupySeat(int row, char col) {
    int colIndex = toupper(col) - 'A';
    int index = (row - 1) * 5 + colIndex;
    if (index >= 0 && index < occupancy.size() && !occupancy[index]) {
        occupancy[index] = true;
        totalOccupiedSeats++;
    }
}

// Save screening details to file
// Writes the screening details and occupancy status to a file
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

// Calculate and set the total price based on the number of reduced and normal tickets
void Screening::calculateAndSetTotalPrice(int numReducedTickets, int numNormalTickets) {
    double reducedPrice = price * 0.5;
    totalPrice = (numReducedTickets * reducedPrice) + (numNormalTickets * price);
    setPrice(totalPrice);
}

// Adds a screening to the theater
void Theater::addScreening(const Screening& screening) {
    screenings.push_back(screening);
}

// Loads screenings from a file
// Reads screening details from a file and adds them to the theater
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

// Saves screenings to a file
// Writes the details of all screenings in the theater to a file
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

// Getter for the list of screenings
vector<Screening>& Theater::getScreenings() {
    return screenings;
}

// Sets the genre for a specific movie
// Updates the genre for all screenings of the specified movie
void Theater::setGenreForMovie(const string& movieName, const string& genre) {
    for (auto& screening : screenings) {
        if (screening.getMovieName() == movieName) {
            screening.setGenre(genre);
        }
    }
}

// Sets the minimum age for a specific movie
// Updates the minimum age for all screenings of the specified movie
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

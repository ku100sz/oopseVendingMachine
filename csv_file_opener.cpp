#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Class to hold screening data
class Screening {
public:
    Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy)
        : movieName(movieName), date(date), hour(hour), price(price), occupancy(occupancy) {}

    void display() const {
        cout << "Movie: " << movieName << endl;
        cout << "Date: " << date << endl;
        cout << "Hour: " << hour << endl;
        cout.precision(2);
        cout << fixed << "Price: " << price << endl;
        cout << "Occupancy:" << endl;
        for (bool seat : occupancy) {
            cout << seat << " ";
        }
        cout << endl;
    }

private:
    string movieName;
    string date;
    string hour;
    double price;
    vector<bool> occupancy;
};

// Class to manage a collection of screenings
class Theater {
public:
    void addScreening(const Screening& screening) {
        screenings.push_back(screening);
    }

    void loadScreeningsFromFile(const string& filename) {
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

    void displayScreenings() const {
        for (const auto& screening : screenings) {
            screening.display();
            cout << "----------------------" << endl;
        }
    }

private:
    vector<Screening> screenings;
};

int main() {
    // Prompt the user to enter the file name
    cout << "Enter the name of the file (with the '.csv' extension): ";
    string filename;
    cin >> filename;

    Theater theater;
    theater.loadScreeningsFromFile(filename);
    theater.displayScreenings();

    return 0;
}

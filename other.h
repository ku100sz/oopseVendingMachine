#ifndef OTHER_H
#define OTHER_H

#include <vector>
#include <string>

using namespace std;

class Movie {
protected:
    string movieName; // Name of the movie

private:
    string genre;     // Genre of the movie
    int minAge;       // Minimum age to watch the movie

public:
    // Constructors
    Movie();
    Movie(const string& name, const string& genre, int minAge);

    // Getters and Setters
    string getMovieName() const;
    void setMovieName(const string& name);

    string getGenre() const;
    void setGenre(const string& genre);

    int getMinAge() const;
    void setMinAge(int age);
};

class Screening : public Movie {
private:
    string date;          // Date of the screening
    string hour;          // Hour of the screening
    double price;         // Price of a single ticket
    vector<bool> occupancy; // Seat occupancy status
    int totalOccupiedSeats; // Total number of occupied seats
    double totalPrice;    // Total price for the tickets

public:
    // Constructors
    Screening();
    Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy);

    // Getters and Setters for Screening attributes
    string getDate() const;
    void setDate(const string& date);

    string getHour() const;
    void setHour(const string& hour);

    double getPrice() const;
    void setPrice(double price);

    vector<bool> getOccupancy() const;
    void setOccupancy(const vector<bool>& occupancy);

    int getTotalOccupiedSeats() const;
    void setTotalOccupiedSeats(int totalOccupiedSeats);

    bool isSeatAvailable(int row, char col) const;
    void occupySeat(int row, char col);
    void saveToFile(ofstream& file) const;
    void calculateAndSetTotalPrice(int numReducedTickets, int numNormalTickets);

    // Display methods
    void display() const;
    void display(bool showPrice) const;
};

class Theater {
public:
    void addScreening(const Screening& screening);
    void loadScreeningsFromFile(const string& filename);
    void saveScreeningsToFile(const string& filename) const;
    vector<Screening>& getScreenings();

    // Methods to set genre and minimum age for movies
    void setGenreForMovie(const string& movieName, const string& genre);
    void setMinAgeForMovie(const string& movieName, int minAge);

private:
    vector<Screening> screenings; // List of screenings in the theater
};

// Function to load movies from the file
Theater getMovies();

#endif // OTHER_H

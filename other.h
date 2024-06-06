#ifndef OTHER_H
#define OTHER_H

#include <vector>
#include <string>

using namespace std;

// The Movie class represents basic information about a movie.
class Movie {
protected:
    string movieName; // Name of the movie, accessible by derived classes

private:
    string genre;     // Genre of the movie
    int minAge;       // Minimum age to watch the movie

public:
    // Constructors
    Movie();
    Movie(const string& name, const string& genre, int minAge);

    // Getters and Setters
    string getMovieName() const; // Returns the name of the movie
    void setMovieName(const string& name); // Sets the name of the movie

    string getGenre() const; // Returns the genre of the movie
    void setGenre(const string& genre); // Sets the genre of the movie

    int getMinAge() const; // Returns the minimum age required to watch the movie
    void setMinAge(int age); // Sets the minimum age with validation
};

// The Screening class represents a movie screening, inheriting from the Movie class.
class Screening : public Movie {
private:
    string date; // Date of the screening
    string hour; // Hour of the screening
    double price; // Price of a single ticket
    vector<bool> occupancy; // Seat occupancy status (true if occupied, false if available)
    int totalOccupiedSeats; // Total number of occupied seats
    double totalPrice; // Total price for the purchased tickets

public:
    // Constructors
    Screening();
    Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy);

    // Getters and Setters for Screening attributes
    string getDate() const; // Returns the date of the screening
    void setDate(const string& date); // Sets the date of the screening

    string getHour() const; // Returns the hour of the screening
    void setHour(const string& hour); // Sets the hour of the screening

    double getPrice() const; // Returns the price of a single ticket
    void setPrice(double price); // Sets the price of a ticket, applies discount if before 15:00

    vector<bool> getOccupancy() const; // Returns the occupancy status of the seats
    void setOccupancy(const vector<bool>& occupancy); // Sets the occupancy status of the seats

    int getTotalOccupiedSeats() const; // Returns the total number of occupied seats
    void setTotalOccupiedSeats(int totalOccupiedSeats); // Sets the total number of occupied seats

    // Other methods
    bool isSeatAvailable(int row, char col) const; // Checks if a seat is available
    void occupySeat(int row, char col); // Marks a seat as occupied
    void saveToFile(ofstream& file) const; // Saves the screening details to a file
    void calculateAndSetTotalPrice(int numReducedTickets, int numNormalTickets); // Calculates and sets the total price for tickets

    // Display methods
    void display() const; // Displays screening details with price
    void display(bool showPrice) const; // Displays screening details, optionally with price
};

// The Theater class manages a collection of movie screenings.
class Theater {
public:
    void addScreening(const Screening& screening); // Adds a screening to the theater
    void loadScreeningsFromFile(const string& filename); // Loads screenings from a file
    void saveScreeningsToFile(const string& filename) const; // Saves screenings to a file
    vector<Screening>& getScreenings(); // Returns a list of screenings

    // Methods to set genre and minimum age for movies
    void setGenreForMovie(const string& movieName, const string& genre); // Sets the genre for a specific movie
    void setMinAgeForMovie(const string& movieName, int minAge); // Sets the minimum age for a specific movie

private:
    vector<Screening> screenings; // List of screenings in the theater
};

// Function to load movies from the file
Theater getMovies();

#endif // OTHER_H

#ifndef OTHER_H
#define OTHER_H

#include <vector>
#include <string>

using namespace std;

class Screening {
public:
    Screening(const string& movieName, const string& date, const string& hour, double price, const vector<bool>& occupancy);
    Screening(); // Overloaded constructor
    void display() const;
    void display(bool showPrice) const; // Overloaded display method
    string getMovieName() const;
    void setMovieName(const string& movieName);
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

private:
    string movieName;
    string date;
    string hour;
    double price;
    vector<bool> occupancy;
    int totalOccupiedSeats;
    double totalPrice;
};

class Theater {
public:
    void addScreening(const Screening& screening);
    void loadScreeningsFromFile(const string& filename);
    void saveScreeningsToFile(const string& filename) const;
    vector<Screening>& getScreenings();

private:
    vector<Screening> screenings;
};

Theater getMovies();

#endif // OTHER_H

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

private:
    string movieName;
    string date;
    string hour;
    double price;
    vector<bool> occupancy;
};

class Theater {
public:
    void addScreening(const Screening& screening);
    void loadScreeningsFromFile(const string& filename);
    const vector<Screening>& getScreenings() const;

private:
    vector<Screening> screenings;
};

Theater getMovies();

#endif // OTHER_H

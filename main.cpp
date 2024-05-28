#include "other.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    Theater theater = getMovies();

    while (true) {
        // List movies
        for (size_t i = 0; i < theater.getScreenings().size(); ++i) {
            cout << i + 1 << ". " << theater.getScreenings()[i].getMovieName() << endl;
        }
        cout << theater.getScreenings().size() + 1 << ". Exit" << endl;

        // Take user input
        cout << "Enter the number of the movie to see details or 'exit' to quit: ";
        string input;
        cin >> input;

        if (input == "exit" || input == to_string(theater.getScreenings().size() + 1)) {
            break;
        }

        try {
            size_t choice = stoi(input);
            if (choice > 0 && choice <= theater.getScreenings().size()) {
                theater.getScreenings()[choice - 1].display();
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } catch (invalid_argument&) {
            cout << "Invalid input. Please enter a number or 'exit'." << endl;
        }
    }

    return 0;
}

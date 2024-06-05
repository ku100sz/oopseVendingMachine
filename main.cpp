#include "other.h"
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <cstdlib> // Include for the exit function

using namespace std;

// Function to validate seat format
bool isValidSeat(const string& seat) {
    if (seat.length() != 2) return false;
    if (!isalpha(seat[0]) || !isdigit(seat[1])) return false;
    char col = toupper(seat[0]);
    int row = seat[1] - '0';
    return (col >= 'A' && col <= 'E' && row >= 1 && row <= 6);
}

int main() {
    // Load movie screenings from file
    Theater theater = getMovies();

    // Set genre and minimum age for movies
    theater.setGenreForMovie("GWTW", "Drama");
    theater.setMinAgeForMovie("GWTW", 12);

    theater.setGenreForMovie("GBaU", "Comedy");
    theater.setMinAgeForMovie("GBaU", 15);

    theater.setGenreForMovie("AVATAR", "Sci-Fi");
    theater.setMinAgeForMovie("AVATAR", 13);

    theater.setGenreForMovie("AVATAR2", "Sci-Fi");
    theater.setMinAgeForMovie("AVATAR2", 13);

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
                while (true) {
                    theater.getScreenings()[choice - 1].display();

                    cout << "1. Purchase ticket(s)" << endl;
                    cout << "2. Return" << endl;

                    cout << "Enter your choice: ";
                    string subInput;
                    cin >> subInput;

                    if (subInput == "2") {
                        break;
                    } else if (subInput == "1") {
                        int availableSeats = 30 - theater.getScreenings()[choice - 1].getTotalOccupiedSeats();
                        if (availableSeats == 0) {
                            cout << "No seats available. Please return to the previous menu." << endl;
                            cout << "1. Return" << endl;
                            while (true) {
                                cout << "Enter your choice: ";
                                string returnInput;
                                cin >> returnInput;
                                if (returnInput == "1") {
                                    break;
                                } else {
                                    cout << "Invalid choice. Please try again." << endl;
                                }
                            }
                            break;
                        }

                        int numReducedTickets;
                        while (true) {
                            cout << "Enter number of reduced tickets (50% off): ";
                            cin >> numReducedTickets;
                            if (cin.fail() || numReducedTickets < 0 || numReducedTickets > availableSeats) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid number. Please enter a number between 0 and " << availableSeats << "." << endl;
                            } else {
                                break;
                            }
                        }

                        int remainingSeats = availableSeats - numReducedTickets;
                        int numNormalTickets;
                        while (true) {
                            cout << "Enter number of normal tickets: ";
                            cin >> numNormalTickets;
                            if (cin.fail() || numNormalTickets < 0 || numNormalTickets > remainingSeats) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                cout << "Invalid number. Please enter a number between 0 and " << remainingSeats << "." << endl;
                            } else {
                                break;
                            }
                        }

                        int numTickets = numReducedTickets + numNormalTickets;

                        vector<string> seats;
                        for (int i = 0; i < numTickets; ++i) {
                            while (true) {
                                cout << "Choose seat (" << i + 1 << " of " << numTickets << "): ";
                                string seat;
                                cin >> seat;
                                if (seat == "exit") {
                                    goto endPurchase;
                                }
                                if (!isValidSeat(seat)) {
                                    cout << "Invalid seat. Please enter in the format [A-E][1-6] (e.g., B2)." << endl;
                                } else {
                                    int row = seat[1] - '0';
                                    char col = toupper(seat[0]);
                                    if (theater.getScreenings()[choice - 1].isSeatAvailable(row, col)) {
                                        seats.push_back(seat);
                                        theater.getScreenings()[choice - 1].occupySeat(row, col);
                                        break;
                                    } else {
                                        cout << "Seat " << seat << " is already occupied. Choose another seat." << endl;
                                    }
                                }
                            }
                        }

                        theater.getScreenings()[choice - 1].calculateAndSetTotalPrice(numReducedTickets, numNormalTickets);
                        double totalPrice = theater.getScreenings()[choice - 1].getPrice();

                        while (true) {
                            cout << "1. Pay " << totalPrice << " PLN" << endl;
                            cout << "2. Exit" << endl;

                            cout << "Enter your choice: ";
                            string payInput;
                            cin >> payInput;

                            if (payInput == "2") {
                                goto endPurchase;
                            } else if (payInput == "1") {
                                cout << "Transaction complete" << endl;
                                theater.saveScreeningsToFile("movies");
                                exit(0); // Exit the program after transaction is complete
                            } else {
                                cout << "Invalid input. Please try again." << endl;
                            }
                        }
                    } else {
                        cout << "Invalid choice. Please try again." << endl;
                    }
                }
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        } catch (invalid_argument&) {
            cout << "Invalid input. Please enter a number or 'exit'." << endl;
        }

endPurchase:
        continue;
    }

    return 0;
}

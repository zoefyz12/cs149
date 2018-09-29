#include "seatingchart.hh"

#include <array>
#include <pthread.h>
#include <semaphore.h>
#include <utility>
using namespace std;


SeatingChart::SeatingChart():
    seatsFilled(0),
    mediumRowSearchOrder({{5, 6, 4, 7, 3, 8, 2, 9, 1, 0}})
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);

    /*for (auto& a : chart) {
        a.fill("----");
    }*/
    for (int outer = 0; outer < 10; outer++) {
        for (int inner = 0; inner <10; inner++) {
            chart[inner][outer] = "----";
        }
    }
    pthread_mutex_unlock(&mutex);
}


bool SeatingChart::reserveSeat(Seller& seller, Customer& customer) {
    pthread_mutex_lock(&mutex);
    if (seatsFilled >= 100) {
        return false;
    }
    bool found = false;
    char sellerType = seller.name[0];
    if (sellerType == 'H') { //Search through from row 0
        for(int row = 0; row < 10; row++) {
            if (found) {
                break;
            }
            for(int column = 0; column < 10; column++) {
                if (chart[row][column] == "----") {
                    chart[row][column] = customer.name;
                    seatsFilled++;
                    found = true;
                    break;
                }
            }
        }
    } else if (sellerType == 'M') { //Search through alternating rows
        for(int orderIndex = 0; orderIndex < 10; orderIndex++) {
            if (found) {
                break;
            }
            int row = mediumRowSearchOrder[orderIndex];
            for(int column = 0; column < 10; column++) {
                if (chart[row][column] == "----") {
                    chart[row][column] = customer.name;
                    seatsFilled++;
                    found = true;
                    break;
                }
            }
        }
    } else if (sellerType == 'L') { //Search through from row 9
        for(int row = 9; row >= 0; row--) {
            if (found) {
                break;
            }
            for(int column = 0; column < 10; column++) {
                if (chart[row][column] == "----") {
                    chart[row][column] = customer.name;
                    seatsFilled++;
                    found = true;
                    break;
                }
            }
        }
    } else {
        cout << "There is an incorrect seller type afoot." << endl;
    }

    pthread_mutex_unlock(&mutex);
    return found;
}


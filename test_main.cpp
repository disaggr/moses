#include "moses.h"

void function_1() {
    std::vector<int> *huge_vector = new std::vector<int>(100000);
    //moses::PlaceGuard(moses::Place("Second Place"));
    int *huge_array = (int *) malloc(100000 * sizeof(int));
    for(int i = 0; i < 100000; i++) {
        huge_vector->at(i) = 1;
        huge_array[i] = 1;
    }
}

int main(int argc, char *argv[]) {
    //moses::PlaceGuard(moses::Place("First Place"));
    function_1();
    return 0;
}
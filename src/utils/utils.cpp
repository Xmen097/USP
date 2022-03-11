#include "utils.h"

namespace Utils {

    // O(n^2) bubble sort
    void sort_add(uint16_t* arr, uint16_t el) {
        uint16_t last_el = 0;
        for (int i=0; true; i++) {
            if (arr[i] == 0) {
                arr[i] = el;
                return;
            } 
            if (el > arr[i]) {
                uint16_t tmp = arr[i];
                arr[i] = el;
                el = tmp;
            }
        }
    }

}
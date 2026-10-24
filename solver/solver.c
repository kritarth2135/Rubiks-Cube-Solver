#include "solver/solver.h"

void insertion_sort(int *value_array, int *key_array, int size) {
    for (int i = 1; i < size; i++) {
        int key = key_array[i];
        int value = value_array[i];

        int j = i - 1;
        while (j >= 0 && key_array[j] > key) {
            key_array[j + 1] = key_array[j];
            value_array[j + 1] = value_array[j];
            j--;
        }
        key_array[j + 1] = key;
        value_array[j + 1] = value;
    }
}

int get_max(int value1, int value2, int value3, int value4) {
    int max = value1;
    if (value2 > max) {
        max = value2;
    }
    if (value3 > max) {
        max = value3;
    }
    if (value4 > max) {
        max = value4;
    }
    return max;
}

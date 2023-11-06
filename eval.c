#include <stdio.h>
int eval() {
    printf("Hello, World!\n");
    return 0;
}
#include <stdio.h>

// sorting the first n elements //
void selectionSort(int arr[], int n, int k) {
    for (int i = 0; i < k; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[maxIndex]) {
                maxIndex = j;
            }
        }
        // Swap the maximum element with the current element (i)
        int temp = arr[i];
        arr[i] = arr[maxIndex];
        arr[maxIndex] = temp;
    }
}

void sorted_array() {
    int arr[] = {5, 2, 9, 1, 5, 6, 8, 7, 4, 3};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 6; // Number of highest values to find

    printf("Original array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    selectionSort(arr, n, k);

    printf("\nTop %d values in descending order: ", k);
    for (int i = 0; i < k; i++) {
        printf("%d ", arr[i]);
    }

}

int material_score[12] = {
        100,      // white pawn score
        300,      // white knight scrore
        350,      // white bishop score
        500,      // white rook score
        1000,      // white queen score
        10000,      // white king score
        -100,      // black pawn score
        -300,      // black knight scrore
        -350,      // black bishop score
        -500,      // black rook score
        -1000,      // black queen score
        -10000,      // black king score
};

// pawn positional score
const int pawn_score[64] =
        {
                 90,  90,  90,  90,  90,  90,  90,  90,
                 30,  30,  30,  40,  40,  30,  30,  30,
                 20,  20,  20,  30,  30,  30,  20,  20,
                 10,  10,  10,  20,  20,  10,  10,  10,
                  5,   5,  10,  20,  20,   5,   5,   5,
                  0,   0,   0,   5,   5,   0,   0,   0,
                  0,   0,   0, -10, -10,   0,   0,   0,
                  0,   0,   0,   0,   0,   0,   0,   0
        };

// knight positional score
const int knight_score[64] =
        {
                 -5,   0,   0,   0,   0,   0,   0,  -5,
                 -5,   0,   0,  10,  10,   0,   0,  -5,
                 -5,   5,  20,  20,  20,  20,   5,  -5,
                 -5,  10,  20,  30,  30,  20,  10,  -5,
                 -5,  10,  20,  30,  30,  20,  10,  -5,
                 -5,   5,  20,  10,  10,  20,   5,  -5,
                 -5,   0,   0,   0,   0,   0,   0,  -5,
                 -5, -10,   0,   0,   0,   0, -10,  -5
        };

// bishop positional score
const int bishop_score[64] =
        {
                  0,   0,   0,   0,   0,   0,   0,   0,
                  0,   0,   0,   0,   0,   0,   0,   0,
                  0,   0,   0,  10,  10,   0,   0,   0,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,  10,   0,   0,   0,   0,  10,   0,
                  0,  30,   0,   0,   0,   0,  30,   0,
                  0,   0, -10,   0,   0, -10,   0,   0
        };

// rook positional score
const int rook_score[64] =
        {
                 50,  50,  50,  50,  50,  50,  50,  50,
                 50,  50,  50,  50,  50,  50,  50,  50,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,   0,  10,  20,  20,  10,   0,   0,
                  0,   0,   0,  20,  20,   0,   0,   0
        };
// queen positional score
const int queen_score[64] =
        {
                -20, -10, -10,  -5,  -5,  -10, -10, -20,
                -10,   0,   0,   0,   0,    0,   0, -10,
                -10,   0,   5,   5,   5,    5,   0, -10,
                 -5,   0,   5,   5,   5,    5,   0,  -5,
                  0,   0,   5,   5,   5,    5,   0,  -5,
                -10,   5,   5,   5,   5,    5,   0, -10,
                -10,   0,   5,   0,   0,    0,   0, -10,
                -20, -10, -10,  -5,  -5,  -10, -10, -20
        };

// king positional score
const int king_score[64] =
        {
                  0,   0,   0,   0,   0,   0,   0,   0,
                  0,   0,   5,   5,   5,   5,   0,   0,
                  0,   5,   5,  10,  10,   5,   5,   0,
                  0,   5,  10,  20,  20,  10,   5,   0,
                  0,   5,  10,  20,  20,  10,   5,   0,
                  0,   0,   5,  10,  10,   5,   0,   0,
                  0,   5,   5,  -5,  -5,   0,   5,   0,
                  0,   0,   5,   0, -15,   0,  10,   0
        };
const int middlegame_king_score[64] =
        {
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -20, -30, -30, -40, -40, -30, -30, -20,
                -10, -20, -20, -20, -20, -20, -20, -10,
                 20,  20,   0,   0,   0,   0,  20,  20,
                 20,  30,  10,   0,   0,  10,  30,  20
        };
const int endgame_king_score[64] =
        {
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -30, -40, -40, -50, -50, -40, -40, -30,
                -20, -30, -30, -40, -40, -30, -30, -20,
                -10, -20, -20, -20, -20, -20, -20, -10,
                 20,  20,   0,   0,   0,   0,  20,  20,
                 20,  30,  10,   0,   0,  10,  30,  20
        };
/*
// Define the inner function
void innerFunction(int *ptr) {
    *ptr = 100; // Modify the value that 'ptr' points to
}

// Define the middle function
void middleFunction(int *ptr) {
    innerFunction(ptr); // Pass 'ptr' to the inner function
}

// Define the outer function
void outerFunction(int *ptr) {
    int data = 42;
    *ptr = &data; // Assign the address of 'data' to the pointer 'ptr'
    middleFunction(ptr); // Pass 'ptr' to the middle function

    printf("Modified data: %d\n", data); // Prints the modified value of 'data'
}

int main() {
    int ptr; // Declare a pointer
    outerFunction(&ptr); // Pass the pointer to the outer function

    return 0;
}
*/

/*
// Define the inner function
void innerFunction(int *ptr) {
    *ptr = 100; // Modify the value that 'ptr' points to
}

// Define the middle function
void middleFunction(int *ptr) {
    innerFunction(ptr); // Pass 'ptr' to the inner function
}

// Define the outer function
void outerFunction(int *ptr) {
    int data = 42;
    middleFunction(&data); // Pass the address of 'data' to the middle function

    printf("Modified data: %d\n", data); // Prints the modified value of 'data'
}

int main() {
    int data = 0; // Declare an integer variable
    outerFunction(&data); // Pass the address of 'data' to the outer function

    return 0;
}
*/

/*

// Define the inner function
void innerFunction(int *ptr, int increment) {
    *ptr += increment; // Modify the value that 'ptr' points to
}

// Define the middle function
void middleFunction(int *ptr) {
    innerFunction(ptr, 5); // Pass 'ptr' to the inner function with an increment of 5
}

// Define the outer function
void outerFunction(int *ptr) {
    int data = 42;
    middleFunction(&data); // Pass the address of 'data' to the middle function

    printf("Modified data: %d\n", data); // Prints the modified value of 'data'
}

int main() {
    int data = 0; // Declare an integer variable
    outerFunction(&data); // Pass the address of 'data' to the outer function

    return 0;
}

*/
#include <stdio.h>

// Define the inner function
void innerFunction(int *ptr) {
    *ptr += 1; // Modify the value that 'ptr' points to
}
/*
// Define the middle function
void middleFunction(int *ptr) {
    innerFunction(ptr);  // First call: increment by 5
    innerFunction(ptr); // Second call: increment by 10
    innerFunction(ptr);  // Third call: increment by 7
}

// Define the outer function
void outerFunction(int *ptr) {
    int data = 42;
    middleFunction(&data); // Pass the address of 'data' to the middle function

    printf("Modified data: %d\n", data); // Prints the modified value of 'data'
}

int main() {
    int data = 0; // Declare an integer variable
    outerFunction(&data); // Pass the address of 'data' to the outer function

    return 0;
}
 */
/*
static int *globalPtr; // Declare a global static int pointer

void passPointerAddress(int *paramPtr) {
    globalPtr = paramPtr;
}

int main() {
    int localVar = 42;
    int *localPtr = &localVar;

    passPointerAddress(localPtr); // Pass the address of the local pointer

    printf("Global pointer points to: %d\n", *globalPtr);

    return 0;
}
*/

/*
#include <string.h>

struct MyStruct {
    int id;
    char name[50];
};

int main() {
    struct MyStruct myArray[5];
    struct MyStruct* ptr = myArray; // Point 'ptr' to the first struct

    // Populate the first and third elements of the array using pointer arithmetic
    (ptr+0)->id = 1;
    strcpy((ptr+0)->name, "John");

    (ptr+2)->id = 3;
    strcpy((ptr+2)->name, "Alice");
    (ptr+2)->id = (ptr)->id;
    // Print the contents of the array

    printf("ID: %d, Name: %s\n", ptr[2].id, (ptr+2)->name);

    return 0;
}
*/

#include <stdio.h>
int eval() {
    printf("Hello, World!\n");
    return 0;
}
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

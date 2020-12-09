#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

int main() {
    fstream file1("hw3example.output"), file2("test.output");
    char string1[256], string2[256];
    int j;
    j = 0;

    while (!file1.eof()) {
        file1.getline(string1, 256);
        file2.getline(string2, 256);
        j++;
        if (strcmp(string1, string2) != 0) {
            cout << j << "-th strings are not equal"
                 << "\n";
            cout << "   " << string1 << "\n";
            cout << "   " << string2 << "\n";
        }
    }
    return 0;
}
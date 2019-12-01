#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;

double readFile(const char * filename, double * double_values)
{
    ifstream file;

    char * memblock;
    //double * double_values;
    unsigned long size = 0;

    // Setup the file for reading in binary mode.
    file.open(filename, ios::in | ios::binary | ios::ate);

    if (file.is_open())
    {
        cout << "File opened" << endl;
        // Get the size of the file in bytes
        size = file.tellg();

        cout << size << endl;

        // Allocate the memory block, read the entire file, close the file.
        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        // Convert the memblock into a double
        memcpy(double_values, memblock, size);

        file.close();
        delete[] memblock;
    }

    //return double_values;
    return 0;
}
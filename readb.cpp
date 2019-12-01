#include <iostream> 
#include <fstream>
#include <armadillo>
#include <vector>

using namespace std; 
using namespace arma;

int main(int argc, char *argv[])
{
    unsigned long rows = strtoul(argv[2], nullptr, 10);
    unsigned long cols = 1;
    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols <<endl;

    mat b = zeros<dmat>(rows, cols);
    ifstream file;

    char * memblock;
    double * double_values;
    unsigned long size = 0;
    unsigned long int itr = 0;
    double tmp;


    // Setup the file for reading in binary mode.
    file.open(argv[1], ios::in | ios::binary | ios::ate);

    if (file.is_open())
    {
        // Get the size of the file in bytes
        size = file.tellg();

        // Allocate the memory block, read the entire file, close the file.
        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        // Convert the memblock into a double
        double_values = (double*) memblock;

        file.close();
    }

    // Iterate through the array and convert it to a matrix.
    for(int r = 0; r < rows; r++)
    {
        b(r, 0) = double_values[r];
    }

    delete[] memblock;

    cout << "\n\n\n";
    b.print("b:");
    cout << "\n\n\n";

    cout << "b(1,1): " << b(0, 0) << endl;
    cout << "b(2,1): " << b(1, 0) << endl;
    
	return 0; 
}

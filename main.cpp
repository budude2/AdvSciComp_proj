#include <iostream> 
#include <fstream>
#include <armadillo>
#include <vector>

using namespace std; 
using namespace arma;

int main(int argc, char *argv[])
{
    unsigned long rows = strtoul(argv[2], nullptr, 10);
    unsigned long cols = strtoul(argv[3], nullptr, 10);
    cout << "Rows: " << rows << endl;
    cout << "Cols: " << cols <<endl;

    mat A = zeros<dmat>(rows, cols);
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

        cout << "\nsize = " << size << "\n";

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
    for(int c = 0; c < cols; c++)
    {
        for(int r = 0; r < rows; r++)
        {
            A(r, c) = double_values[c * rows + r];
        }
    }

    delete[] memblock;

    cout << "\n\n\n";
    A.print("A:");
    cout << "\n\n\n";

    cout << "A(2,2): " << A(1, 1) << endl;
    cout << "A(1,2): " << A(0, 1) << endl;
    
	return 0; 
}

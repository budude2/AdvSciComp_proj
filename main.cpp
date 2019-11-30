#include <iostream> 
#include <fstream>
#include <armadillo>
#include <vector>

using namespace std; 
using namespace arma;

int main(int argc, char *argv[])
{
    mat A = zeros<dmat>(2, 3);

    char * memblock;
    unsigned long size = 0;
    ifstream file;
    long unsigned int itr = 0;

    double tmp;
    double * double_values;

    file.open(argv[1], ios::in | ios::binary | ios::ate);

    if (file.is_open())
    {
        size = file.tellg();

        cout << "\nsize = " << size << "\n";

        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        double_values = (double*) memblock;

        for(itr = 0; itr < 6; itr++)
        {
            cout << "itr: " << itr << ", " << double_values[itr] << "\n";
        }

        file.close();
    }

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 2; c++)
        {
            cout << double_values[r * 2 + c] << " ";
            A(c, r) = double_values[r * 2 + c];
        }
        cout << "\n";
    }

    cout << "\n\n\n";
    A.print("A:");
    cout << "\n\n\n";

    cout << "A(2,2): " << A(1, 1) << endl;
    cout << "A(1,2): " << A(0, 1) << endl;
    
    delete[] memblock;
	return 0; 
}

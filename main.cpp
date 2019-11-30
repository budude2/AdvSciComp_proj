#include <iostream> 
#include <fstream>
#include <armadillo>
#include <vector>

using namespace std; 
using namespace arma;

int main(int argc, char *argv[])
{
    mat A = zeros<dmat>(2,3);

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

    for(int x = 0; x < 2; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            cout << double_values[y * 2 + x] << " ";
            A(x,y) = double_values[y * 2 + x];
        }
        cout << "\n";
    }

    cout << "\n\n\n";
    A.print("A:");
    
    delete[] memblock;
	return 0; 
}

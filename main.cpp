#include <iostream> 
#include <fstream>
#include <armadillo>
#include <vector>

using namespace std; 

int main(int argc, char *argv[])
{
    ifstream file;
    long unsigned int itr = 0;

    std::vector<double> data;
    double tmp;

	cout << "You have entered " << argc 
	     << " arguments:" << "\n"; 
	      
	for (int i = 0; i < argc; ++i) 
		cout << argv[i] << "\n"; 

    cout << "\n";
    file.open(argv[1], ios::in | ios::binary | ios::ate);

    if (file.is_open())
    {
        unsigned long size = 0;
        size = file.tellg();

        cout << "\nsize = " << size << "\n";

        char * memblock;
        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();

        double* double_values = (double*) memblock;

        for(itr = 0; itr < 6; itr++)
        {
            cout << double_values[itr] << "\n";
        }

        file.close();
        delete[] memblock;
    }

	return 0; 
}

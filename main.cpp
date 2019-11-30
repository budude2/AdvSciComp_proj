#include <iostream> 
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std; 

int main(int argc, char *argv[])
{
    cv::Mat img;
    ifstream myfile;
    string line;
    long unsigned int itr = 0;
    std::string::size_type sz;

    double data[6];

	cout << "You have entered " << argc 
	     << " arguments:" << "\n"; 
	      
	for (int i = 0; i < argc; ++i) 
		cout << argv[i] << "\n"; 

    myfile.open(argv[1], ios::in);
    
    if (myfile.is_open())
    {
        while(getline(myfile, line))
        {
            data[itr] = stod(line, &sz);
            itr++;
        }
        myfile.close();
                  
    }
	
    for(itr = 0; itr < 6; itr++)
    {
        cout << data[itr] << "\n";
    }

    //img = cv::imread("sinogram.jpg", 1);

    //unsigned int width = img.cols;
    //unsigned int height = img.rows;

    //cout << width << " columns\n";
    //cout << height << " rows\n";

	return 0; 
}

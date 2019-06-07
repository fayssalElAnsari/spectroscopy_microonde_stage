/*

scan directory for files and calculate number of tdf files then filter out tdf files: (populateCourbesFiles() )
put the data from the .tdf files to two arrays (courbe1; courbe2): (readData() )
calculate the average of all tdf files and put it in an array (courbe1average; courbe2average) then in a file (averageWithoutSeperation.tdf)
get the value of the average of the first point in both curves files (averageFirstPoint1; averageFirstPoint2)
split to two different groupes of files (groupe1; group2)
calculate the average result curve for both groups and put it in files(average1.tdf; average2.tdf)

bugs:
1. fixed
we need to calculate the firstAveragePoint of the two directions,
then seperate into two groups and not just seperate to two groups with the average of one direction

2. fixed
the firstAveragePoint2 is wrong

3. fixed
the average doens't work for direction 2 because ???the .tdf filter is recursive we want a none recursive filter

todo:
.done.  shift the average and export info in the standard two direction layout
.       fft then calculate the length then get the derivative then get the S/N ration
*/

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <complex>
#include <valarray>
#include <regex>

using namespace std;

const int SIZE = 4096;
double averageFirstPoint1, averageFirstPoint2;

int numTdfFiles;
int* groupeSize = new int[4];

string* courbesFiles;

string* files = new string[100];

double* courbe1 = new double[SIZE];
double* courbe2 = new double[SIZE];


const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

void createFolders (){
    int check;
    string* names = new string[7];
    names[0]=".\\shots\\averages";
    names[1]=".\\shots\\groupe1";
    names[2]=".\\shots\\groupe1\\direction1";
    names[3]=".\\shots\\groupe1\\direction2";
    names[4]=".\\shots\\groupe2";
    names[5]=".\\shots\\groupe2\\direction1";
    names[6]=".\\shots\\groupe2\\direction2";
    for( int i = 0; i< 7; i++){
        string dirname = names[i];
//        clrscr();

        check = mkdir(dirname.c_str());

        // check if directory is created or not
        if (!check)
            cout<<names[i]<<" created"<<endl;
        else {
//            printf("Unable to create directory \n");
        }

    }
//    system("dir/p");
}

void setNumTdfFilesTotale(char* thePath){

}

void populateCourbesFiles(char* thePath) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (thePath)) != NULL) {
      /* print all the files and directories within directory */
      int i = 0;
      while ((ent = readdir (dir)) != NULL) {
        //printf ("%s\n", ent->d_name);
        files[i] = ent -> d_name;
        i++;
      }
      closedir (dir);
    } else {
      perror ("");
    }

    numTdfFiles = 0;
    for(int i = 0; i < 100; i++){
        std::string fn = files[i];
      if(fn.substr(fn.find_last_of(".") + 1) == "tdf") {
        numTdfFiles++;
      } else {

      }
    }

    int j = 0;
    courbesFiles = new string[numTdfFiles];
    for(int i = 0; i < 100; i ++){
        std::string fn = files[i];
      if(fn.substr(fn.find_last_of(".") + 1) == "tdf") {
        courbesFiles[j] = files[i];
        j++;
      } else {

      }
    }
}

void readData(string inFileName) {
    ifstream inFile;
    inFile.open(inFileName);
    string str;
    if (inFile.is_open())
    {
        inFile>>str;
        for (int i = 0; i < SIZE; i++)
        {
            inFile >> courbe1[i]>>courbe2[i];
        }
    } else {
        cout<<"can't read "<<inFileName<<endl;
    }
}

void readData2(string inFileName) {
    ifstream inFile;
    inFile.open(inFileName);
    if (inFile.is_open())
    {
        for (int i = 0; i < SIZE; i++)
        {
            inFile >> courbe1[i];
        }
    } else {
        cout<<"ERROR "<<inFileName<<" is not Open"<<endl;
    }
    inFile.close();
}

void calculateAverage(string* courbesFiles) {
    populateCourbesFiles(".\\shots");

    double* averageA = new double[SIZE];
    double* averageB = new double[SIZE];

    ofstream outFile;
    outFile.open(".\\shots\\averages\\averageWithoutSeperation.tdf");

    for(int i =0; i < numTdfFiles; i++){
        readData(".\\shots\\"+courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            averageA[j] += courbe1[j];
            averageB[j] += courbe2[j];
        }
    }

    for(int i=0; i < SIZE;i++){
        averageA[i] = averageA[i]/numTdfFiles;
        averageB[i] = averageB[i]/numTdfFiles;
        outFile<<averageA[i]<<"   "<<averageB[i]<<endl;
    }
    averageFirstPoint1 = averageA[0];
    averageFirstPoint2 = averageB[0];
    cout<<"the number of .tdf iles in \".\shots\" is: "<<numTdfFiles<<endl;
    cout<<"averagePoint1: "<<averageA[0]<<"; averagePoint2: "<<averageB[0]<<endl;
    outFile.close();
}

void splitToTwo(string* courbesFiles, double averagePoint){
    int* inGroupe = new int[4];
    for(int i = 0; i < 4; i++) {
        inGroupe[i] = 0;
    }
    ofstream outfile;

    for(int i= 0; i < numTdfFiles; i++) {
        readData(".\\shots\\"+courbesFiles[i]);
        if(courbe1[0] > averageFirstPoint1) {
            ///using outfile
            outfile.open(".\\shots\\groupe1\\direction1\\"+courbesFiles[i]);
            cout<<"opened .\\shots\\groupe1\\direction1\\"+courbesFiles[i]<<endl;
            for(int j = 0; j < SIZE; j++){
                outfile<<courbe1[j]<<endl;
            }

            inGroupe[0] = inGroupe[0]+1;
            outfile.close();
        } else {
            ///using outfile
            outfile.open(".\\shots\\groupe2\\direction1\\"+courbesFiles[i]);
            for(int j = 0; j < SIZE; j++){
                outfile<<courbe1[j]<<endl;
            }

            inGroupe[2] = inGroupe[2]+1;
            outfile.close();
        }

        if(courbe2[0] > averageFirstPoint2) {
            ///using outfile
            outfile.open(".\\shots\\groupe1\\direction2\\"+courbesFiles[i]);
            for(int j = 0; j < SIZE; j++){
                outfile<<courbe2[j]<<endl;
            }

            inGroupe[1] = inGroupe[1]+1;
            outfile.close();
        } else {
            ///using outfile
            outfile.open(".\\shots\\groupe2\\direction2\\"+courbesFiles[i]);
            for(int j = 0; j < SIZE; j++){
                outfile<<courbe2[j]<<endl;
            }

            inGroupe[3] = inGroupe[3]+1;
            outfile.close();
        }
    }

    cout<<"groupe 11: "<<inGroupe[0]<<endl;
    cout<<"groupe 12: "<<inGroupe[1]<<endl;
    cout<<"groupe 21: "<<inGroupe[2]<<endl;
    cout<<"groupe 22: "<<inGroupe[3]<<endl;

    for(int i = 0; i<4; i++) {
        groupeSize[i] = inGroupe[i];
    }
}

void nullify(double* &average){
        for(int i =0; i < SIZE; i++) {
            average[i] = 0;
        }
    }

void calculateAverage2(char* shotsPathName, int* groupSize) {
    double* average = new double[SIZE];
    nullify(average);
    ofstream outFile;

    populateCourbesFiles(".\\shots\\groupe1\\direction1");
    outFile.open(".\\shots\\averages\\average1_1.tdf");
    for(int i =0; i < groupSize[0]; i++){
        cout<<"1_1: "<<courbesFiles[i]<<endl;
        readData2(".\\shots\\groupe1\\direction1\\"+courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[0];
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();
    nullify(average);

    populateCourbesFiles(".\\shots\\groupe1\\direction2");
    outFile.open(".\\shots\\averages\\average1_2.tdf");

    for(int i =0; i < groupSize[1]; i++){
        cout<<"1_2: "<<courbesFiles[i]<<endl;
        readData2(".\\shots\\groupe1\\direction2\\"+courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[1];
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();
    nullify(average);

    populateCourbesFiles(".\\shots\\groupe2\\direction1");
    outFile.open(".\\shots\\averages\\average2_1.tdf");

    for(int i =0; i < groupSize[2]; i++){
        cout<<"2_1: "<<courbesFiles[i]<<endl;
        readData2(".\\shots\\groupe2\\direction1\\"+courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[2];
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();
    nullify(average);

    populateCourbesFiles(".\\shots\\groupe2\\direction2");
    outFile.open(".\\shots\\averages\\average2_2.tdf");

    for(int i =0; i < groupSize[3]; i++){
        cout<<"2_2: "<<courbesFiles[i]<<endl;
        readData2(".\\shots\\groupe2\\direction2\\"+courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[3];
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();
    nullify(average);
}

void calculateAverage3() {
    double* groupe1 = new double[SIZE];
    double* groupe2 = new double[SIZE];
    double* direction1Average = new double[SIZE];
    double* direction2Average = new double[SIZE];

    ifstream inFile1;
    ifstream inFile2;
    ofstream outFile;

    //load from files to arrays direction1
    inFile1.open(".\\shots\\averages\\average1_1.tdf");
    inFile2.open(".\\shots\\averages\\average2_1.tdf");
    if (inFile1.is_open()) {
        for (int j = 0; j < SIZE; j++) {
            inFile1 >> groupe1[j];
            inFile2 >> groupe2[j];
        }
    } else {
        cout<<"ERROR file is not Open"<<endl;
    }

        for(int i=0; i <SIZE-6; i++) {
            groupe1[i] = groupe1[i+6];
        }
        for(int i=SIZE; i >SIZE-6; i--) {
            groupe2[i] = 0;
        }

    //average after shifting direction1
    for (int j = 0; j < SIZE; j++) {
        direction1Average[j] = (groupe1[j] + groupe2[j])/ 2;
    }

    inFile1.close();
    inFile2.close();

    //load from files to arrays direction2
    inFile1.open(".\\shots\\averages\\average1_2.tdf");
    inFile2.open(".\\shots\\averages\\average2_2.tdf");
    if (inFile1.is_open())
    {
    for (int j = 0; j < SIZE; j++)
    {
        inFile1 >> groupe1[j];
        inFile2 >> groupe2[j];
    }
    } else {
        cout<<"ERROR file is not Open"<<endl;
    }
    //shifting the 2nd groupe
    for(int i=0; i <SIZE-6; i++) {
        groupe1[i] = groupe1[i+6];
    }
    for(int i=SIZE; i >SIZE-6; i--) {
        groupe2[i] = 0;
    }

    //average after shifting direction2
     for (int j = 0; j < SIZE; j++)
    {
        direction2Average[j] = (groupe1[j] + groupe2[j])/ 2;
    }
    inFile1.close();
    inFile2.close();

    //write to file
    outFile.open(".\\shots\\averages\\averageWithSeperation.tdf");
    for (int j = 0; j < SIZE; j++) {
        outFile<< direction1Average[j]<<"   "<< direction2Average[j]<<endl;
    }
    outFile.close();
    cout<<"all averages calculated!"<<endl;
}

void combineSingleResults(){
    char* pathName;
    pathName = ".\\shots\\averages";
    populateCourbesFiles(pathName);
    ofstream outFile;
    for(int i = 0; i < numTdfFiles; i++){
            readData(pathName+courbesFiles[i]);
//            outFile.open(pathName+"\\average1.tdf");
    }

    outFile.close();
}

// Cooley–Tukey FFT (in-place, divide-and-conquer)
// Higher memory requirements and redundancy although more intuitive
void fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
// Better optimized but less intuitive
// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
// The bug is now fixed @2017/05/30
void fft2(CArray &x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}

// inverse fft (in-place)
void ifft(CArray& x)
{
    // conjugate the complex numbers
    x = x.apply(std::conj);

    // forward fft
    fft( x );

    // conjugate the complex numbers again
    x = x.apply(std::conj);

    // scale the numbers
    x /= x.size();
}

void calculateFft(){
    Complex* inputCurve = new Complex[SIZE];
    readData(".\\shots\\averages\\averageWithSeperation.tdf");
    for(int i = 0; i < SIZE; i++) {
        inputCurve[i] = courbe1[i];
    }

    CArray data(inputCurve, SIZE);

    // forward fft
    fft2(data);
    std::cout << "fft done!" << std::endl;

    ofstream outFile;
    outFile.open(".\\shots\\averages\\fft_1.tdf");
    for(int i = 0; i < SIZE; i++) {
//        courbe1[i] = pow(pow(real(data[i])2,)+pow(imag(data[i]),2), 0.2);
        outFile<<pow(pow(real(data[i]),2)+ pow(imag(data[i]),2) ,0.5)<<endl;
    }

    outFile.close();
    for(int i = 0; i < SIZE; i++) {
        inputCurve[i] = courbe2[i];
    }

    CArray data2(inputCurve, SIZE);

    // forward fft
    fft2(data2);
    std::cout << "fft done!" << std::endl;

    outFile.open(".\\shots\\averages\\fft_2.tdf");
    for(int i = 0; i < SIZE; i++) {
//        courbe1[i] = pow(pow(real(data[i])2,)+pow(imag(data[i]),2), 0.2);
        outFile<<pow(pow(real(data2[i]),2)+ pow(imag(data2[i]),2) ,0.5)<<endl;
    }
}

void averageTester(){
    for(int n = 2; n<numTdfFiles; n=n*n){

    }
}

int main()
{
    averageFirstPoint1 = 0;
    averageFirstPoint2 = 0;
    char* pathNameShots = ".\\shots";

    createFolders();

    setNumTdfFilesTotale(pathNameShots);
    populateCourbesFiles(pathNameShots);
    calculateAverage(courbesFiles);
    splitToTwo(courbesFiles, averageFirstPoint1);
    calculateAverage2(".\\shots", groupeSize);
    calculateAverage3();

    calculateFft();


    return 0;
}

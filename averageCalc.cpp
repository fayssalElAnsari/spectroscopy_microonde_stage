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
. shift the average and export info in the standard two direction layout
*/

#include <string>
#include <iostream>
#include <fstream>
#include <dirent.h>


using namespace std;

const int SIZE = 4096;
double averageFirstPoint1, averageFirstPoint2;

int numTdfFiles;
int* groupeSize = new int[4];

string* courbesFiles;

string* files = new string[100];

double* courbe1 = new double[SIZE];
double* courbe2 = new double[SIZE];

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
            printf("Directory created\n \n");
        else {
            printf("Unable to create directory \n");
        }

    }
//    system("dir/p");
}

//void createFoldersList (){
//    string names = new string[7];
//    names[0]=".\\shots\\averages";
//    names[1]=".\\shots\\groupe1";
//    names[2]=".\\shots\\groupe1\\direction1";
//    names[3]=".\\shots\\groupe1\\direction2";
//    names[4]=".\\shots\\groupe2";
//    names[5]=".\\shots\\groupe2\\direction1";
//    names[6]=".\\shots\\groupe2\\direction2";
//    // Creating a directory
//    if (mkdir("geeksforgeeks", 0777) == -1)
//        cerr << "Error :  " << strerror(errno) << endl;
//
//    else
//        cout << "Directory created";
//}

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

void calculateAverage3(char* shotsPathName) {
    populateCourbesFiles(shotsPathName);

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

///fourier functions
//void four1(double* data, unsigned long nn)
//{
//    unsigned long n, mmax, m, j, istep, i;
//    double wtemp, wr, wpr, wpi, wi, theta;
//    double tempr, tempi;
//
//    // reverse-binary reindexing
//    n = nn<<1;
//    j=1;
//    for (i=1; i<n; i+=2) {
//        if (j>i) {
//            swap(data[j-1], data[i-1]);
//            swap(data[j], data[i]);
//        }
//        m = nn;
//        while (m>=2 && j>m) {
//            j -= m;
//            m >>= 1;
//        }
//        j += m;
//    };
//
//    // here begins the Danielson-Lanczos section
//    mmax=2;
//    while (n>mmax) {
//        istep = mmax<<1;
//        theta = -(2*M_PI/mmax);
//        wtemp = sin(0.5*theta);
//        wpr = -2.0*wtemp*wtemp;
//        wpi = sin(theta);
//        wr = 1.0;
//        wi = 0.0;
//        for (m=1; m < mmax; m += 2) {
//            for (i=m; i <= n; i += istep) {
//                j=i+mmax;
//                tempr = wr*data[j-1] - wi*data[j];
//                tempi = wr * data[j] + wi*data[j-1];
//
//                data[j-1] = data[i-1] - tempr;
//                data[j] = data[i] - tempi;
//                data[i-1] += tempr;
//                data[i] += tempi;
//            }
//            wtemp=wr;
//            wr += wr*wpr - wi*wpi;
//            wi += wi*wpr + wtemp*wpi;
//        }
//        mmax=istep;
//    }
//}
//
//
void FFT(double* courbe) {
    populateCourbesFiles(".\\shots\\averages\\");
    ofstream outFile;
    for(int i = 0; i < numTdfFiles; i++){
            readData(courbesFiles[i]);
            outFile.open(".\\shots\\averages\\"+courbesFiles[i]);
    }
//    four1(courbe1, numTdfFiles);

}

int main()
{
    averageFirstPoint1 = 0;
    averageFirstPoint2 = 0;
    char* pathNameShots = ".\\shots";

    createFolders();

    populateCourbesFiles(pathNameShots);
    calculateAverage(courbesFiles);
    splitToTwo(courbesFiles, averageFirstPoint1);
    calculateAverage2(".\shots", groupeSize);

    return 0;
}

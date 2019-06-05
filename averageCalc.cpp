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

2.
the firstAveragePoint2 is wrong
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
string* courbesFiles1, courbesFiles2;

string* files = new string[100];

double* courbe1 = new double[SIZE];
double* courbe2 = new double[SIZE];

double returnFirstPoint(string inFileName){
//    string str;
//    string strNum;
//    double y;
//    ifstream inFile;
//    inFile.open(inFileName);
////    cout<<"this is the inFileName: "<<inFileName<<endl;
//    if (inFile.is_open()){
//        inFile>>str;
//        inFile>>strNum;
//    }
//    y = stod(strNum);
//    return y;
}

void populateCourbesFiles(char* thePath) {
    ///scan directory for files
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (thePath)) != NULL) {
      /* print all the files and directories within directory */
      int i = 0;
//      cout<<"directory content: "<<endl;
      while ((ent = readdir (dir)) != NULL) {
        //printf ("%s\n", ent->d_name);
        files[i] = ent -> d_name;
//        cout<<files[i]<<endl;
        i++;
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
    }

    ///filter out only the files with extension ".tdf"
    ///calculate number of tdf files
    numTdfFiles = 0;
    for(int i = 0; i < 100; i++){
        std::string fn = files[i];
      if(fn.substr(fn.find_last_of(".") + 1) == "tdf") {
//        std::cout << "Yes..." << std::endl;
//        cout<<"files["<<i<<"] "<<files[i]<<endl;
//        cout<<courbesFiles[j]<<endl;
        numTdfFiles++;
      } else {
        //std::cout << "No..." << std::endl;
      }
    }

    ///get names of tdf files
    int j = 0;
    courbesFiles = new string[numTdfFiles];
    for(int i = 0; i < 100; i ++){
        std::string fn = files[i];
      if(fn.substr(fn.find_last_of(".") + 1) == "tdf") {
//        std::cout << "Yes..." << std::endl;
        courbesFiles[j] = files[i];
//        cout<<courbesFiles[j]<<" in "<<thePath<<endl;
//        cout<<"files["<<i<<"] "<<files[i]<<endl;
//        cout<<courbesFiles[j]<<endl;
        j++;
      } else {
        //std::cout << "No..." << std::endl;
      }
    }
//    cout<<"found "<<numTdfFiles<<" .tdf files in "<<thePath<<endl;
//    cout<<"****** ******"<<endl;
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
    }
}

void calculateAverage(string* courbesFiles) {
//    cout<<"start of calculate average"<<endl;
    populateCourbesFiles(".\\shots");

    double* averageA = new double[SIZE];
    double* averageB = new double[SIZE];

    ofstream outFile;
    outFile.open(".\\shots\\averages\\averageWithoutSeperation.tdf");

    //accumulate
    for(int i =0; i < numTdfFiles; i++){
        readData(".\\shots\\"+courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            averageA[j] += courbe1[j];
            averageB[j] += courbe2[j];
        }
    }
    //average
    for(int i=0; i < SIZE;i++){
        averageA[i] = averageA[i]/numTdfFiles;
        averageB[i] = averageB[i]/numTdfFiles;
        outFile<<averageA[i]<<"   "<<averageB[i]<<endl;
    }
    averageFirstPoint1 = averageA[0];
    averageFirstPoint2 = averageB[0];
    cout<<"the number of tdfFiles: "<<numTdfFiles<<"; averagePoint1: "<<averageA[0]<<"; averagePoint2: "<<averageB[0]<<endl;
    outFile.close();
//    cout<<"file : "<<"averageWithoutSeperation.tdf created"<<endl;
}

void calculateAverage2(char* shotsPathName, int* groupSize) {
    ofstream outFile;
    int fullSize = groupSize[0] + groupSize[1];

    ///calculate average for groupe one direction 1
    populateCourbesFiles(".\\shots\\groupe1\\direction1");
    outFile.open(".\\shots\\averages\\average1_1.tdf");
    double* average = new double[SIZE];

    //accumulate
    for(int i =0; i < groupSize[0]; i++){
        readData(courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

//    cout<<"the numtdffiles is: "<<numTdfFiles<<endl;
    //average
    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[0];
//        cout<<average[i]<<endl;
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();


    ///calculate average for groupe one direction 2
    populateCourbesFiles(".\\shots\\groupe1\\direction2");
    outFile.open(".\\shots\\averages\\average1_2.tdf");

    //accumulate
    for(int i =0; i < groupSize[1]; i++){
        readData(courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

//    cout<<"the numtdffiles is: "<<numTdfFiles<<endl;
    //average
    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[1];
//        cout<<average[i]<<endl;
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();

    ///calculate average for groupe two direction 1
    populateCourbesFiles(".\\shots\\groupe2\\direction1");
    outFile.open(".\\shots\\averages\\average2_1.tdf");

    //accumulate
    for(int i =0; i < groupSize[2]; i++){
        readData(courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

//    cout<<"the numtdffiles is: "<<numTdfFiles<<endl;
    //average
    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[2];
//        cout<<average[i]<<endl;
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();

    ///calculate average for groupe two direction 2
    populateCourbesFiles(".\\shots\\groupe2\\direction2");
    outFile.open(".\\shots\\averages\\average2_2.tdf");

    //accumulate
    for(int i =0; i < groupSize[3]; i++){
        readData(courbesFiles[i]);
        for(int j = 0; j < SIZE; j++){
            average[j] += courbe1[j];
        }
    }

//    cout<<"the numtdffiles is: "<<numTdfFiles<<endl;
    //average
    for(int i=0; i < SIZE;i++){
        average[i] = average[i]/groupSize[3];
//        cout<<average[i]<<endl;
        outFile<<average[i]<<"  "<<endl;
    }
    outFile.close();


//    ///groupe 2
//    populateCourbesFiles(".\\shots\\groupe2");
//    outFile2.open(".\\shots\\averages\\average2.tdf");
//    double* average2A = new double[SIZE];
//    double* average2B = new double[SIZE];
//
//    //accumulate
//    for(int i =0; i < groupe2Size; i++){
//        readData(courbesFiles[i]);
//        for(int j = 0; j < SIZE; j++){
//            average2A[j] += courbe1[j];
//            average2B[j] += courbe2[j];
//
//        }
//    }
//    //average
//    for(int i=0; i < SIZE;i++){
//        average2A[i] = average2A[i]/groupe2Size;
//        average2A[i] = average2B[i]/groupe2Size;
//
////        cout<<average[i]<<endl;
//        outFile2<<average2A[i]<<"   "<<average2B[i]<<endl;
//    }
//    outFile2.close();
}

void loadFiles() {
//    int currentPath;
//    //get current path
//    int bytes = GetModuleFileName(NULL, pBuf, len);
//    if(bytes == 0)
//        currentPath = '/';
//    else
//        currentPath = bytes;
//
//    DIR *dir;
//    struct dirent *ent;
//    if ((dir = opendir ("\\")) != NULL) {
//      /* print all the files and directories within directory */
//      while ((ent = readdir (dir)) != NULL) {
//        printf ("%s\n", ent->d_name);
//    }
//      closedir (dir);
//    } else {
//      /* could not open directory */
//      perror ("");
////      return EXIT_FAILURE;
//    }
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


            ///copy files from shots to shots\\groupe1 directly
////          cout<<"courbe1[0]: "<<courbe1[0]<<"; averagePoint: "<<averagePoint<<endl;
//            std::ifstream  src(".\\shots\\"+courbesFiles[i], std::ios::binary);
//            std::ofstream  dst(".\\shots\\groupe1\\"+courbesFiles[i],   std::ios::binary);
////            cout<<courbesFiles[i]<<" is in groupe : 1"<<endl;
//
//            dst << src.rdbuf();
            inGroupe[0] = inGroupe[0]+1;
            outfile.close();
        } else {
            ///using outfile
            outfile.open(".\\shots\\groupe2\\direction1\\"+courbesFiles[i]);
            for(int j = 0; j < SIZE; j++){
                outfile<<courbe1[j]<<endl;
            }

//            ///copy files from shots to shots\\groupe2
//            std::ifstream  src(".\\shots\\"+courbesFiles[i], std::ios::binary);
//            std::ofstream  dst(".\\shots\\groupe2\\"+courbesFiles[i],   std::ios::binary);
////            cout<<courbesFiles[i]<<" is in groupe : 2"<<endl;
//
//            dst << src.rdbuf();
            inGroupe[1] = inGroupe[1]+1;
            outfile.close();
        }

        if(courbe2[0] > averageFirstPoint2) {
            ///using outfile
            outfile.open(".\\shots\\groupe1\\direction2\\"+courbesFiles[i]);
            for(int j = 0; j < SIZE; j++){
                outfile<<courbe2[j]<<endl;
            }

            inGroupe[2] = inGroupe[2]+1;
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


    //split files in groupe1 and in group2 according to the direction

}

void printArray(double table[]){
    cout<<"called"<<endl;
}

void FFT(double* courbe) {

}

int main()
{
    char* pathNameShots = ".\\shots";
    populateCourbesFiles(pathNameShots);

    averageFirstPoint1 = 0;
    cout<<"number of tdffiles is: "<<numTdfFiles<<endl;

    ///calculating the average first point
//    for(int i=0; i < numTdfFiles; i++) {
//        str_i = std::to_string(i+1);
//        filename = "shot_"+str_i+".tdf";
//        filename = ".\\shots\\"+filename;
////       cout<<"this is the filename in main: "<<filename<<endl;
//        readData(filename);
//        firstPoint[i] = returnFirstPoint(filename);
////        cout<<i+1<<":"<<firstPoint[i]<<endl;
//    }
//
//    for(int i=0; i < numTdfFiles; i++) {
//        averagePoint += firstPoint[i];
////        cout<<"the average is now: "<<averagePoint<<";the first point is now: "<<firstPoint[i]<<endl;
//    }
//    averagePoint = averagePoint/numTdfFiles;
    calculateAverage(courbesFiles);
    splitToTwo(courbesFiles, averageFirstPoint1);
    calculateAverage2(".\shots", groupeSize);
    loadFiles();
    return 0;
}

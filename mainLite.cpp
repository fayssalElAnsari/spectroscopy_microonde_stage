/*
this file is supposed to be smaller and faster by using variables only and writing the end results in the average file

*/


#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>

using namespace std;

const int size = 32;
const int numDeGroupes = 2;
const int numDeDirections = 2;
const int numDacquisitions = 4096;

string* shotsFiles;
int numTdfFiles;
double firstAveragePoint1;
double firstAveragePoint2;

double**** a;

///build the folders boilerplate
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
        check = mkdir(dirname.c_str());
        if (!check)
            cout<<names[i]<<" created"<<endl;
    }
}

///get the names of shots files
void setShotsFiles (){
    //get the whole list of files in .\\shots
    char* thePath = ".\\shots";
    DIR *dir;
    string* files = new string[1000];//overflow?
    struct dirent *ent;
    if ((dir = opendir (thePath)) != NULL) {
      int i = 0;
      while ((ent = readdir (dir)) != NULL) {
        files[i] = ent -> d_name;
        i++;
      }
      closedir (dir);
    } else {
      perror ("");
    }

    //get the number of tdf files in .\\shots
    numTdfFiles = 0;
    for(int i = 0; i < 1000; i++){
        string fn = files[i];
        if(fn.substr(fn.find_last_of(".") + 1) == "tdf") {
        numTdfFiles++;
        }
    }
    cout<<"numTdfFiles "<<numTdfFiles<<endl;

    //get the names of tdf files in .\\shots
    int j = 0;
    shotsFiles = new string[numTdfFiles];
    for(int i = 0; i < 1000; i ++){
        std::string fn = files[i];
        if(fn.substr(fn.find_last_of(".") + 1) == "tdf") {
        shotsFiles[j] = files[i];
        j++;
        }
    }
}

void createContainer() {
    a = new double***[numTdfFiles];
    for(int i =0; i<numTdfFiles; i++){
       a[i] = new double**[numDeGroupes];
       for(int j =0; j<numDeGroupes; j++){
           a[i][j] = new double*[numDeDirections];
           for(int k = 0; k<numDeDirections;k++){
              a[i][j][k] = new double[numDacquisitions];
                for(int l = 0; l< numDacquisitions; l++){
                    a[i][j][k][l] = 0;
                }
           }
       }
    }
}

void setContainer() {
    string str;
    ifstream inFile;
    for(int i =0; i < numTdfFiles; i++) {
        inFile.open((".\\shots\\"+shotsFiles[i]).c_str());
        inFile>>str;
        if(inFile.is_open()){
            for(int i =0; i< numTdfFiles; i++){
                for(int l = 0; l < numDacquisitions; l++) {
                    inFile>> a[i][0][numDeDirections-2][l] >> a[i][0][numDeDirections-1][l];
                }
            }
            inFile.close();
        } else {
            cout<<"file not open!!!"<<endl;
        }
    }
}

void calculateAverage() {
    cout<<": "<<a[0][0][0][0]<<"     "<<a[0][0][1][0]<<endl;
    //accumulate
    for(int i = 0; i < numTdfFiles; i++) {
        for( int l =0; l<numDacquisitions; l++){
            a[0][0][0][l] += a[i][0][0][l];
            a[0][0][1][l] += a[i][0][1][l];
        }
    }

    for( int l =0; l<numDacquisitions; l++){
        cout<<l<<": "<<a[0][0][0][l]<<"     "<<a[0][0][1][l]<<endl;
    }



    //calculate the average
    for(int i = 0; i < numTdfFiles; i++) {
        for( int l =0; l<numDacquisitions; l++){
            a[0][0][0][l] = a[0][0][0][l]/numTdfFiles;
            a[0][0][1][l] = a[0][0][1][l]/numTdfFiles;
        }
    }

    firstAveragePoint1 = a[0][0][0][0];
    firstAveragePoint2 = a[0][0][1][0];
    cout<<"1: "<<firstAveragePoint1<<"2: "<< firstAveragePoint2<<endl;
}

int main(){
    createFolders();
    setShotsFiles();
    createContainer();
    setContainer();
    calculateAverage();
    return 0;
}


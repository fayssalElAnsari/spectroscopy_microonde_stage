/*
this file is supposed to be smaller and faster by using variables only and writing the end results in the average file

*/


#include <iostream>
#include <fstream>
#include <dirent.h>
#include <complex>
#include <valarray>
#include <string>

const int size = 32;
const int numDeGroupes = 2;
const int numDeDirections = 2;

string* shotsFiles;
int numTdfFiles;


double*** a;

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
    double* files = new double[1000];//overflow?
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
        if(fn.substr(files[i].find_last_of(".") + 1) == "tdf") {
        numTdfFiles++;
        }
    }

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
    a = new double*[numTdfFiles];
    for(int i = 0; i < numDeGroupes; i++){
        a[i]* = new int[numDeGroupes];
        for(int j=0; j < numDeDirections; j++){
            a[i][j] = new int[numDeDirections];
        }
    }
}

void setContainer() {
    ifstream inFile;
    for(int i =0; i < numTdfFiles; i++) {
        inFile.open(".\\shots\\"+shotsFiles);

    }
}

int main(){
    createFolders();
    setShotsFiles();
    createContainer();

    return 0;
}


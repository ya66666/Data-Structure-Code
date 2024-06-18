//11120102 莊凱任
//11120127 陳宥任

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;

#define Max_LEN 8
#define Max_BUFFER 300

struct Student {
    char putID[10]; // 發訊者學號
    char getID[10]; // 收訊者學號
    float weight;   // 量化權重
};

struct Index {
	float weight ;
	int offset ;
};

bool ReadBinaryFile(string fileNum) {
    bool success = false;
    fileNum = "pairs" + fileNum + ".bin";
    ifstream inputFile(fileNum.c_str(), ios::binary);

    if (inputFile.is_open()) success = true;
    else cout << endl << fileNum << " does not exist!!!" << endl << endl;

    return success;
}

void ExternalSort(string fileNum, int num) {
    string Num = fileNum;
    fileNum = "pairs" + fileNum + ".bin";
    ifstream inputFile(fileNum.c_str(), ios::binary);
    vector<string> tempFiles;
    int fileCount = 0;
    int count = 0;

    auto internalStart = chrono::high_resolution_clock::now();

    // 分批讀取資料並排序
    while (!inputFile.eof()) {
        vector<Student> buffer;
        buffer.reserve(num);
        Student student;

        for (int i = 0; i < num && inputFile.read(reinterpret_cast<char*>(&student), sizeof(Student)); ++i)  buffer.push_back(student);

        if (buffer.empty()) break;

        // 排序
        stable_sort(buffer.begin(), buffer.end(), [](const Student& a, const Student& b) {
            return a.weight > b.weight;
        });

        // 寫入臨時檔案
        string tempFileName = "temp" + to_string(fileCount++) + ".bin";
        ofstream tempFile(tempFileName, ios::binary);
        for (auto s : buffer) {
            tempFile.write(reinterpret_cast<const char*>(&s), sizeof(Student));
        }
        tempFile.close();
        tempFiles.push_back(tempFileName);
        count = count + 1;
    }

    auto internalEnd = chrono::high_resolution_clock::now();
    auto internalSortTime = chrono::duration_cast<chrono::milliseconds>(internalEnd - internalStart).count();

    inputFile.close();
    cout << "The internal sort is completed. Check the initial sorted runs!" << endl << endl;

    auto externalStart = chrono::high_resolution_clock::now();

    // 合併排序
    cout << "Now there are " << tempFiles.size() << " runs." << endl << endl;
    while (tempFiles.size() > 1) {
        vector<string> newTempFiles;
        for (int i = 0; i < tempFiles.size(); i += 2) {
            if (i + 1 < tempFiles.size()) {
                // 合併兩個臨時檔案
                string tempFileName = "temp" + to_string(fileCount++) + ".bin";
                ifstream file1(tempFiles[i], ios::binary);
                ifstream file2(tempFiles[i + 1], ios::binary);
                ofstream outFile(tempFileName, ios::binary);

                Student student1, student2;
                bool hasStudent1 = file1.read(reinterpret_cast<char*>(&student1), sizeof(Student)).good();
                bool hasStudent2 = file2.read(reinterpret_cast<char*>(&student2), sizeof(Student)).good();

                while (hasStudent1 && hasStudent2) {
                    if (student1.weight >= student2.weight) {
                        outFile.write(reinterpret_cast<const char*>(&student1), sizeof(Student));
                        hasStudent1 = file1.read(reinterpret_cast<char*>(&student1), sizeof(Student)).good();
                    } 
					else {
                        outFile.write(reinterpret_cast<const char*>(&student2), sizeof(Student));
                        hasStudent2 = file2.read(reinterpret_cast<char*>(&student2), sizeof(Student)).good();
                    }
                }

                while (hasStudent1) {
                    outFile.write(reinterpret_cast<const char*>(&student1), sizeof(Student));
                    hasStudent1 = file1.read(reinterpret_cast<char*>(&student1), sizeof(Student)).good();
                }

                while (hasStudent2) {
                    outFile.write(reinterpret_cast<const char*>(&student2), sizeof(Student));
                    hasStudent2 = file2.read(reinterpret_cast<char*>(&student2), sizeof(Student)).good();
                }

                file1.close();
                file2.close();
                outFile.close();

                newTempFiles.push_back(tempFileName);
            } 
			else {
                // 單獨一個檔案，直接加入新臨時檔案列表
                newTempFiles.push_back(tempFiles[i]);
            }
        }

        // 刪除舊的臨時檔案
        for (auto tempFile : tempFiles) {
            if (find(newTempFiles.begin(), newTempFiles.end(), tempFile) == newTempFiles.end()) {
                remove(tempFile.c_str());
            }
        }

        tempFiles = newTempFiles;
        cout << "Now there are " << tempFiles.size() << " runs." << endl << endl;
    }

    // 最終合併結果
    if (!tempFiles.empty() && count != 1) {
    	//刪檔 
    	remove(("order" + Num + ".bin").c_str());
        rename(tempFiles[0].c_str(), ("order" + Num + ".bin").c_str());
    }

    auto externalEnd = chrono::high_resolution_clock::now();
    auto externalSortTime = chrono::duration_cast<chrono::milliseconds>(externalEnd - externalStart).count();

    auto totalTime = internalSortTime + externalSortTime;

    cout << "The execution time ..." << endl;
    cout << "Internal Sort = " << internalSortTime << " ms" << endl;
    cout << "External Sort = " << externalSortTime << " ms" << endl;
    cout << "Total Execution Time = " << totalTime << " ms" << endl;
}

bool ReadBin( string fileNum, vector <Index> & all ) {
	vector <Student> data(300) ;
	Index temp ;
	float w = 99 ;
	int offset = 0, key = 1 ;
	
    string Name = fileNum ;
	fileNum = "order" + fileNum + ".bin" ;
	ifstream inputFile( fileNum.c_str(), ios::binary ) ;
	
	if ( ! inputFile.is_open() ) {
		cout << Name << ".bin does not exist!!!" << endl << endl; 
		cout << "<Primary index>: (key, offset)" << endl;
		return false;
	}
	
	cout << "<Primary index>: (key, offset)" << endl;
	
	do {
		inputFile.read( reinterpret_cast<char*>(data.data()), 300 * sizeof(Student) ) ;
		
		for ( int i = 0 ; i < 300 ; i++ ) {
			if ( w > data[i].weight ) {
				w = data[i].weight ;
				temp.weight = w ;
				temp.offset = offset ;
				all.push_back( temp ) ;
				if ( w != 0 ) 
					cout << "[" << key << "] (" << w << ", " << offset << ")" << endl ;
				
				key += 1 ;
			}
			
			offset += 1 ;
		}
		
		data.clear() ;
		data.resize(300) ;
	} while ( ! inputFile.eof() ) ;

	inputFile.close() ;
	return true;
} 

int Find( float number, vector <Index> all ) {
	int index = 0 ; 
	
	for ( int i = 0 ; i < all.size() ; i++ ) {
		if ( all[i].weight == number ) 
			index = i ;
	}
	
	return index ;
}

void WriteToFile( string fileNum, vector <Index> all, float number ) {
	vector <Student> data ;
	Student dd ;
	ofstream fout ;
	float w = -1 ;
	int offset = 0, key = 0 ;
	string fileDouble = fileNum ;
	 
	fileNum = "order" + fileNum + ".bin" ;
	ifstream inputFile( fileNum.c_str(), ios::binary ) ;
	if ( ! inputFile.is_open() ) {
		cout << "There is no index!" << endl << endl ; 
		return ;
	}
	
	auto timestart = chrono::high_resolution_clock::now() ;
	int index = Find( number, all ) ;
	while ( inputFile.read( reinterpret_cast<char*>( &dd ), sizeof(Student) ) ) {
		if ( w != dd.weight ) {
			w = dd.weight ;
			data.push_back( dd ) ;
		}
	}
	
	auto timeend = chrono::high_resolution_clock::now() ;
	
	fout.open( ("order"+ fileDouble +".txt").c_str(), ios::out | ios::trunc ) ;
	fout << "Search " << number << " : " << all[index+1].offset << " records, " << static_cast<double>(chrono::duration_cast<chrono::microseconds>(timeend - timestart).count() ) / 1000 << " ms" << endl ;
	// 寫檔 
	for ( int i = 0 ; i < data.size() ; i++ ) {
		fout << "[" << 1 + all[i].offset << "] " <<  data[i].putID << " " << data[i].getID << " " << data[i].weight << endl ;
	}
		
	fout.close() ;
	inputFile.close() ;
}

int main() {
    int i = 0;
    int num = 300;
    int time = 0;
    int command = 0;
    bool doit = false;
    float number = 0.0;
    vector <Index> all ;
    string fileNum;
    cout << endl;

    do {
        cout << "*** The buffer size is " << num << endl;
        cout << "Input a number in [1, 60000]: ";
        cin >> num;
        while(num < 1 || num > 60000){
        	cout << endl << "### It is NOT in [1,60000] ###" << endl << endl;
        	cout << "Input a number in [1, 60000]: ";
            cin >> num;
		}
        cout << endl;
        cout << "***********************************************" << endl;
        cout << " On-machine Exercise 05                       *" << endl;
        cout << " Mission 1: External Merge Sort on a Big File *" << endl;
        cout << " Mission 2: Construction of Primary Index     *" << endl;
        cout << " Mission 3: Range Search by Primary Index     *" << endl;
        cout << "***********************************************" << endl;
        cout << "########################################################" << endl;
        cout << "Mission 1: External merge sort" << endl;
        cout << "########################################################" << endl << endl;

    again:
        cout << "Input the file name: [0]Quit " << endl;
        cin >> fileNum;
        cin.ignore(Max_LEN, '\n');

        if (fileNum == "0") {
            cout << endl << "[0]Quit or [Any other key]continue?" << endl;
            cin >> command;
            cin.ignore(Max_LEN, '\n');
            cout << endl;
        } 
		else {
            if (!ReadBinaryFile(fileNum)) goto again;
            else {
            	cout << endl;
                ExternalSort(fileNum, num);
                
                //任務二開始 
                cout << endl << endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
                cout << "Mission 2: Build the primary index" << endl;
                cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
                
                all.clear() ;
                doit = ReadBin( fileNum, all ) ;
                cout << endl << endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
                cout << "Mission 3: Threshold search on primary index" << endl;
                cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
            second:
                if (doit){
                  cout << "Please input a threshold in the range [0,1]: " << endl;
                  cin >> number;
                  if (number > 1.0 || number <= 0.0) {
				    cout << endl << "The threshold is not in the range [0,1]!" << endl;
                  }
                  else{
                  	//執行任務三 
                  	WriteToFile( fileNum, all, number ) ;
				  }
				  
				  cout << endl << "[0]Quit or [Any other key]continue?" << endl;
                  cin >> command;
                  cin.ignore(Max_LEN, '\n');
                  if (command != 0) {
                  	cout << endl;
                  	goto second;
				  }
                }
                else cout << "There is no index!" << endl;
            }
            
            cout << endl << "[0]Quit or [Any other key]continue?" << endl;
            cin >> command;
            cin.ignore(Max_LEN, '\n');
            cout << endl;
        }
		
    } while (command != 0);

    system("pause");
    return 0;
}

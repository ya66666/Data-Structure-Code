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

using namespace std;
string FileName;

#define Max_LEN 8

struct Student{	
	char sid[10] ; //學號
	char sname[10] ; // 名字 
	unsigned char scores[6]; //6個分數 
	float average ; //平均分數 
} ; 

struct HashEntry {
    string sid;
    string sname;
    float average;
    int hvalue;
    int time;
    bool occupied;

    HashEntry() : sid(""), sname(""), average(0.0), hvalue(-1), time(0), occupied(false) {}
};

bool Load(string fileNum){
  bool success = false;
  
  string fileName = "input" + fileNum + ".bin";
  fileNum = "input"+ fileNum +".txt";
  
  ifstream inputFile( fileNum.c_str() );
  if (inputFile.is_open()){
    success = true;
    ofstream outputFile(fileName.c_str(), ios::binary);

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        Student student;

        ss.get(student.sid, 10, '\t'); //讀取學號
        ss.get(); // skip tab
        ss.get(student.sname, 10, '\t'); //讀取姓名
        ss.get(); // skip tab
        for(int i=0;i<6;++i){
            int score;
            ss >> score;
            student.scores[i] = static_cast<unsigned char>(score);
            ss.get(); // skip space or newline
        }
        ss >> student.average;
        //Data.push_back(student);
        outputFile.write(reinterpret_cast<char*>(&student), sizeof(student));
    }

    inputFile.close();
    outputFile.close();
  }
  else cout << endl << "### " << fileNum << " does not exist! ###" << endl << endl;
  
  return success;
}

bool ReadBinaryFile(string fileNum, vector<Student> &Data) {
	bool success = false;
	fileNum = "input" + fileNum + ".bin";
    ifstream inputFile(fileNum.c_str(), ios::binary);
    int i = 1;
    if (inputFile.is_open()) {
    	success = true;
        while (!inputFile.eof()) {
            Student student;
            if (inputFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
            	/*cout << "第" << i << "位" << endl; 
                cout << "SID: " << student.sid << endl;
                cout << "Name: " << student.sname << endl;
                cout << "Scores:";
                for (int i = 0; i < 6; ++i) {
                    cout << " " << static_cast<int>(student.scores[i]);
                }
                cout << "\nAverage: " << student.average << "\n\n";*/
            }
            
            //i++;
            Data.push_back(student);
        }
        inputFile.close();
    } 
	else cout << endl << "### " << fileNum << " does not exist! ###" << endl;
	
	return success;
}

int FindPrimeNumber( vector<Student> Data ) {
	int num = 1.3 * Data.size() ;
//	cout << num << endl ;
	bool prime = false ;
	
	while ( ! prime ) {
		num += 1 ;
		
		for ( int i = 2 ; i < num ; i += 1 ) {
			if ( num % i == 0 ) break ; //此數不是質數
			
			if ( i == num - 1 ) 
				prime = true ;
		}
	}
	
//	cout << Data.size() << endl ;
//	cout << "num = "<< num << endl ;
	return num ;
}

int FindRemainder( char sid [], int prime ) {
	int remainder = sid[0] % prime ;
//	cout << "sid[0] = " << sid[0] << " " << remainder << endl ;
	
	for ( int i = 1 ; i < 10 ; i += 1 ) {
		if ( sid[i] > 47 && sid[i] < 58 ) {
			remainder = ( sid[i] % prime ) * remainder ;
			remainder = remainder % prime ;
//			cout << "sid[" << i << "] = " << sid[i] << " " << remainder << endl ;
		}
		else break ;
	}
	
//	cout << "FindRemainder = " << remainder << endl ;
	return remainder ;
}

void In_to_txt( vector <HashEntry> double_hash, string fileNum ) {
	ofstream out ;
	out.open( ("double" + fileNum + ".txt").c_str(), ios::out|ios::trunc) ;
	out << "--- Hash table created by Double hashing ---" << endl ;
	
	for ( int i = 0 ; i < double_hash.size() ; i += 1 ) {
		if ( i <= 100 ) 
			out << "[ " << i << "] " ;
			
		else out << "[" << i << "] " ;
		
		if ( double_hash[i].occupied ) 
			out << double_hash[i].hvalue << ", " << double_hash[i].sid << ", " << double_hash[i].sname << ", " << double_hash[i].average << endl ;
		
		else 
			out << endl ;
	
	}
	
	out << "------------------------------------------------------" ;
}

int findNextPrime(int n) {
	n++;
    while (true) {
        bool isPrime = true;
        for (int i = 2; i <= sqrt(n); ++i) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) return n;
        ++n;
    }
}

int hashFunc(const string &key, int tableSize) {
    unsigned long long product = 1;
    for (int i = 0; i < key.length(); i++) {
    	char c = key[i];
        product *= c;
    }
    return product % tableSize;
}

void quadraticProbingInsert(vector<HashEntry> &hashTable, const Student& student){
    int originalIndex = hashFunc(student.sid, hashTable.size());
    int index = originalIndex;
    int h = 1;
    
    while (hashTable[index].occupied && h < hashTable.size()) {
        index = (originalIndex + h * h) % hashTable.size();
        h++;
    }
    
    if (!hashTable[index].occupied) {
        hashTable[index].sid = student.sid;
        hashTable[index].sname = string(student.sname);
        hashTable[index].average = student.average;
        hashTable[index].hvalue = hashFunc(student.sid, hashTable.size());
        hashTable[index].time = h;
        hashTable[index].occupied = true;
    }
}

float averageSearchTimes(const vector<HashEntry> &hashTable, const vector<Student> &studentData, int totalEntries, bool existing) {
  float loadFactor = 0;
  int count = 0;
  
  if (existing) { //存在 
    for (int i = 0; i < hashTable.size(); ++i) count = count + hashTable[i].time;
  } 
  else { // 不存在
    int emptySlots = 1;
    
    for (int i = 0; i < hashTable.size(); ++i) {
    	int j = i;
    	int h = 1;
    	while( hashTable[j].hvalue != -1 ){
          emptySlots++;
          j = ( i + h * h ) % totalEntries;
          h++;
        }
        
        count = count + emptySlots;
        emptySlots = 1;
    }
  }
  
  loadFactor = float(count) / totalEntries;
  return loadFactor;
}

int main(){

  int command = 0;
  int i = 0;
  string fileNum ;
  vector<Student> Data;
  vector <HashEntry> double_hash ; 
  int hash_table_size = 0, remainder = 0, highest_step = 0 ; 
  float compare = 0.000 ;
  float cmp = 0.000;
  bool exist = false;
  bool again = false;
  cout << endl;
  
  do{
	  cout << "******* Hash Table *****" << endl;
      cout << "* 0. QUIT              *" << endl;
   	  cout << "* 1. Quadratic probing *" << endl;
      cout << "* 2. Double hashing    *" << endl;
      cout << "************************" << endl;
      cout << "Input a choice(0, 1, 2): " ;
    
      cin >> command;
      cin.ignore( Max_LEN, '\n' );
      
      if ( command == 0 ) break;
      else if( command >= 3 ) cout << endl << "Command does not exist!" << endl << endl << endl;
      else if ( command != 1 && Data.empty() ) cout << "### Command 1 first. ###" << endl << endl << endl; //非由指令1開始
      else if ( command == 1 ) { 
        if ( i != 0 ){ //重複叫1就重置 
        	Data.clear() ;
        	double_hash.clear() ;
        	again = false;
		} 
      
        cout << endl << "Input a file number ([0] Quit): " ;
      	cin >> fileNum;
      	
      	if ( fileNum == "0" ) cout << endl;
      	else{
      		
      	  if( ReadBinaryFile(fileNum, Data) ) exist = true;
      	  else exist = Load(fileNum);
      	  
      	  if ( exist && Data.empty() ) ReadBinaryFile(fileNum, Data);
      	  if (exist){
      	    i = i + 1; //執行1的成功次數 
      	    Data.pop_back();
            
			int tableSize = findNextPrime(Data.size() * 1.3);
			vector<HashEntry> hashTable(tableSize);
			for (int i = 0; i < Data.size(); ++i) quadraticProbingInsert(hashTable, Data[i]);
			
			int h = 0;
			
			ofstream outputFile( ("quadratic" + fileNum + ".txt").c_str() );
			outputFile << "--- Hash table created by Quadratic probing ---" << endl;
            for (size_t i = 0; i < hashTable.size(); ++i) {
              HashEntry entry = hashTable[i];
              outputFile << "[" << i << "] ";
              if (entry.occupied) {
              	h = h + 1;
                outputFile << entry.hvalue << ", "
                           << entry.sid << ", "
                           << entry.sname << ", "
                           << entry.average << endl;
              }
              else outputFile << endl;
            }
            
            outputFile << "-----------------------------------------------";
            outputFile.close();
        
            float avgSearchExists = averageSearchTimes(hashTable, Data, h, true);
            float avgSearchNotExists = averageSearchTimes(hashTable, Data, tableSize, false);
            
            cout << endl << "Hash table has been successfully created by Quadratic probing" << endl;
            cout << fixed << setprecision(4);
            cout << "unsuccessful search: " << avgSearchNotExists-1 << " comparisons on average" << endl;
            cout << "successful search: " << avgSearchExists << " comparisons on average" << endl;
            //cout << tableSize << endl;
          }
        }
        
        cout << endl;
      }
      
      else if ( command == 2 ) {
      	if (!again){
      	  compare = 0.000 ;
	      hash_table_size = FindPrimeNumber( Data ) ; // 用來找hash table適合的大小
      	  double_hash.resize( hash_table_size ) ; // 重製vector的大小 
      	  highest_step = findNextPrime( Data.size() / 2 ) ;
		
      	  for ( int i = 0 ; i < Data.size() ; i += 1 ) {
      		remainder = FindRemainder( Data[i].sid, hash_table_size ) ; // 找餘數 
      		
      		if ( ! double_hash[remainder].occupied ) { // 該格沒被占用 
      			double_hash[remainder].sid = Data[i].sid ; // 填資料 
      			double_hash[remainder].sname = Data[i].sname ;
      			double_hash[remainder].average = Data[i].average ;
      			double_hash[remainder].hvalue = remainder ;
      			double_hash[remainder].occupied = true ;
      			compare += 1 ;
//      			cout << "沒撞到 " << double_hash[remainder].time << double_hash[remainder].sname << endl ;
			}
			
			else { // 該格被占用  
				int remainder_and_step = remainder ;
				int step = highest_step - FindRemainder( Data[i].sid, highest_step ) ;
				compare += 1 ;
				
				while ( double_hash[remainder_and_step].occupied ) {
					remainder_and_step = ( remainder_and_step + step ) % hash_table_size ;
					compare += 1 ;
				}
				
				double_hash[remainder_and_step].sid = Data[i].sid ; // 填資料 
      			double_hash[remainder_and_step].sname = Data[i].sname ;
      			double_hash[remainder_and_step].average = Data[i].average ;
      			double_hash[remainder_and_step].hvalue = remainder ;
      			double_hash[remainder_and_step].occupied = true ;
//      			cout << "  撞到 " << compare << " " << double_hash[remainder_and_step].sname << endl ;
			}	
		  }
		
		  In_to_txt( double_hash, fileNum ) ;
		  cmp = compare / Data.size() ;
		  again = true;
	    }
	    
	    cout << endl << "Hash table has been successfully created by Double hashing " << endl ;
	    cout << "successful search: " << cmp << " comparisons on average" << endl ;
		cout << endl;
      } 
      
  } while ( command != 0 );
    
  system("pause");
  return 0;
}

//以下為確認檔案從二進位檔讀入有正常 
            /*int j = 0;
            while (j < Data.size()){
            	cout << "第" << j+1 << "位" << endl; 
                cout << "SID: " << Data[j].sid << endl;
                cout << "Name: " << Data[j].sname << endl;
                cout << "Scores:";
                for (int k = 0; k < 6; ++k) {
                    cout << " " << static_cast<int>(Data[j].scores[k]);
                }
                cout << "\nAverage: " << Data[j].average << "\n\n";
            	j++;
			}
			cout << "Size: " << Data.size() << endl; */

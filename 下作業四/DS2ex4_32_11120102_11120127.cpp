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
#include <unordered_map>

using namespace std;

#define Max_LEN 8

struct Student{    
    char putID[12] ; //學號
    char getID[12] ; // 名字 
    float weight ; //平均分數 
} ; 

struct Num {
    string str ;
    int cns ; 
};

typedef map<string, map<string, float> > AdjList;
typedef unordered_map<string, bool> VisitMap ;

//typedef set<string> IDSet;

class AdjacencyLists{
private:
  vector<Student> Data;
  AdjList adjacencyList;
  set<string> allIDs;
  
  
  bool ReadBinaryFile(string fileNum) {
    bool success = false;
    fileNum = "pairs" + fileNum + ".bin";
    ifstream inputFile(fileNum.c_str(), ios::binary);
    int i = 0;
    
    if (inputFile.is_open()) {
        success = true;
        while (!inputFile.eof()) {
            Student student;
            if (inputFile.read(reinterpret_cast<char*>(&student), sizeof(Student))) {
                /*cout << "第" << i << "位" << endl; 
                cout << "putID: " << student.putID << endl;
                cout << "getID: " << student.getID << endl;
                cout << "weight: " << student.weight << "\n\n";*/
                Data.push_back(student);

                // 將資料加入相鄰串列
                //string putIDStr(student.putID, student.putID + 12);
                //string getIDStr(student.getID, student.getID + 12);
                string putIDStr = string(student.putID);
                string getIDStr = string(student.getID);
                adjacencyList[putIDStr][getIDStr] = student.weight;
                
                allIDs.insert(putIDStr);
                allIDs.insert(getIDStr);
            }         
        }
        inputFile.close();
    } 
    else cout << endl << "### " << fileNum << " does not exist! ###" << endl;
    
    for (set<string>::const_iterator it = allIDs.begin(); it != allIDs.end(); ++it) {
        const string& id = *it;

        // 檢查adjacencyList中是否已存在該學號
        if (adjacencyList.find(id) == adjacencyList.end()) {
            // 如果不存在，則插入新的元素並標記為未發送消息
            adjacencyList[id]["-1"] = 0;
        }
    }
    
    return success;
  }

  bool WriteAdjacencyListToFile(string baseFileName) {
    string outFileName = baseFileName + ".adj";
    ofstream outputFile(outFileName.c_str());

    int i = 0;
    int j = 1;
    
    outputFile << "<<< There are " << adjacencyList.size() << " IDs in total. >>>" << endl;

    for (AdjList::const_iterator it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
      i++;
      outputFile << "[" << i << "] " << it->first << ":" << endl << "\t"; // 發訊者學號
      
      while(it->second.find("-1") != it->second.end()) it++; // 沒發訊息學生就跳過 
      
      map<string, float> innerMap = it->second;
      for (map<string, float>::const_iterator innerIt = innerMap.begin(); innerIt != innerMap.end(); ++innerIt) {
      	outputFile << "(" << j << ")" ;
        outputFile << innerIt->first << ", \t" << innerIt->second << "\t"; // 收訊者學號及權重
        j++;
      }
      
      outputFile << endl;
      j = 1;
    }
    
    outputFile << "<<< There are " << Data.size() << " nodes in total. >>>";
    outputFile.close();
    
    cout << endl << "<<< There are " << i << " IDs in total. >>>" << endl << endl;
    return true;
  }
  
public:
  bool Load(string fileNum){
  	return ReadBinaryFile(fileNum);
  }
  
  bool write(string fileNum){
  	return WriteAdjacencyListToFile(fileNum);
  }
  
  void Clear(){
  	Data.clear();
  	adjacencyList.clear();
  	allIDs.clear();
  }
  
  bool empty(){
  	if (Data.empty()) return true;
  	else return false;
  }
  
  AdjList GetADJ() {
  	return adjacencyList ;
  }
  
  set <string> GetALLIDS() {
  	return allIDs ;
  }
  
  int size(){
  	return Data.size();
  }
};

class ConnectionCounts { 
private :
  vector < vector<string> > Data ; //有被連接到的 
  queue <string> q ;
  AdjList adjList ;
  VisitMap visitedMap;
   
public :
  
  void All_False() {
  	for (unordered_map<string, bool>::iterator entry = visitedMap.begin(); entry != visitedMap.end(); ++entry) {
        entry->second = false ;
    }
  }
  
  void Allstudent( string stid ) {
  	vector <string> temp ;
	string id = stid ;
//	cout << "First id = " << id << endl ;
	visitedMap[id] = true ;
	
	do {
		AdjList::const_iterator it = adjList.find( id ) ;
  		map<string, float> innerMap = it -> second ;
		for ( map<string, float>::const_iterator innerIt = innerMap.begin() ; innerIt != innerMap.end() ; ++innerIt ) {
			if (  ! visitedMap[innerIt -> first] ) {
//				cout << "push = " << innerIt -> first << endl ;
				temp.push_back( innerIt -> first ) ;
    			q.push( innerIt -> first ) ;	
    			visitedMap[innerIt -> first] = true ;
			}
		}
		
		if ( ! q.empty() ) {
			id = q.front() ;
//			cout << "pop = " << id << endl ;
			q.pop() ;
		}
		
		
	} while ( ! q.empty() ) ;
  	
  	Data.push_back( temp ) ;
  	All_False() ;
  }

  void DFS( string stid, float num, int i ) {
    string id = stid ;
    visitedMap[id] = true ;
    
    AdjList::const_iterator it = adjList.find( id ) ;
    map<string, float> innerMap = it->second ;
    for ( map<string, float>::const_iterator innerIt = innerMap.begin() ; innerIt != innerMap.end() ; ) {
        while( innerIt != innerMap.end() && visitedMap[innerIt->first] ) 
            innerIt++;
        
        if ( innerIt == innerMap.end() ) 
            break ;
        
        if ( innerIt->second >= num && !visitedMap[innerIt->first] ) {
            Data[i].push_back( innerIt->first ) ;
            DFS( innerIt->first, num, i ) ; 
        }
        
        ++innerIt; 
    }
  }

  
  void SetADJ( const AdjList& adjacencyList ) {
  	adjList = adjacencyList ;
  }
  
  void Sort( vector <Num> &student_id ) {
  	int num = Data.size() ;
  	for ( int i = 0 ; i < num ; i += 1 ) {
  		sort( Data[i].begin(), Data[i].end() ) ;
	}
  	
  	for(int i = 0; i < student_id.size(); ++i) {
    	student_id[i].cns = Data[i].size(); 
	}
  	
	for(int i = 1; i < num; ++i) {
	    Num key = student_id[i];
	    vector<string> data_key = Data[i];
	    int j = i - 1;
	    
	    while(j >= 0 && student_id[j].cns < key.cns) {
	        student_id[j + 1] = student_id[j];
	        Data[j + 1] = Data[j];
	        --j;
	    }
	    student_id[j + 1] = key;
	    Data[j + 1] = data_key;
	}
  } 
  
  int CountSizeNotZero() {
  	int count = 0 ;
  	for ( int i = 0 ; i < Data.size() ; i++ ) {
  		if ( Data[i].size() > 0 ) 
  			count += 1 ;
	}
	
	return count ;
  }
  
  void Writefile_cm2( string file, vector <Num> student_id ) {
  	ofstream fout ;
  	fout.open( ("pairs"+ file +".cnt").c_str(), ios::out|ios::trunc ) ;
  	fout << "<<< There are " << Data.size() << " IDs in total. >>>" << endl ;
  	int k = Data.size() ;
  	for( int i = 0 ; i < k ; i += 1 ) {
  		fout << "[" ;
  		if ( i + 1 < 10 ) 
  			fout << "  " << i + 1 << "] " ;
		else if ( i + 1 < 100 )
			fout << " " << i + 1 << "] " ;
		else
			fout << i + 1 << "] " ;
			
		fout << student_id[i].str << "(" << student_id[i].cns << "): " ;
		for ( int j = 0 ; j < Data[i].size() ; j += 1 ) {
			if ( j % 12 == 0 ) 
				fout << endl ;
			
			if ( j + 1 < 10 )
				fout << "\t" << "( " << j + 1 << ") " << Data[i][j] ;
			else 
				fout << "\t" << "(" << j + 1 << ") " << Data[i][j] ;
		}
		
		fout << endl ;
	}
	
  	fout.close();
  }
  
  void Writefile_cm3( string file, vector <Num> student_id ) {
  	int count = CountSizeNotZero() ;
  	ofstream fout ;
  	fout.open( ("pairs"+ file +".inf").c_str(), ios::out|ios::trunc ) ;
  	fout << "<<< There are " << count << " IDs in total. >>>" << endl ;
  	int k = Data.size() ;
  	for( int i = 0 ; i < k ; i += 1 ) {
  		if ( Data[i].size() == 0 ) 
			break ;
  		
  		fout << "[" ;
  		if ( i + 1 < 10 ) 
  			fout << "  " << i + 1 << "] " ;
		else if ( i + 1 < 100 )
			fout << " " << i + 1 << "] " ;
		else
			fout << i + 1 << "] " ;
			
		fout << student_id[i].str << "(" << student_id[i].cns << "): " ;
		for ( int j = 0 ; j < Data[i].size() ; j += 1 ) {
			
			if ( j % 12 == 0 ) 
				fout << endl ;
			
			if ( j + 1 < 10 )
				fout << "\t" << "( " << j + 1 << ") " << Data[i][j] ;
			else 
				fout << "\t" << "(" << j + 1 << ") " << Data[i][j] ;
		}
		
		fout << endl ;
	}
	
  	fout.close();
  }
  
  int CountTotal() {
  	VisitMap tt ;
  	int total = 0 ;
	for ( int temp = 0 ; temp < Data.size() ; temp++ ) {
		for ( int i = 0 ; i < Data[temp].size() ; i++ ) {
			if ( !tt[Data[temp][i]] ) {
				tt[Data[temp][i]] = true ;
				total ++ ;
			}	
		}	
	}
	
  	return total ;
  }
  
  bool MapEmpty() {
  	return visitedMap.empty() ;
  }
  
  int GetDataSize() {
  	return Data.size() ;
  }
  
  void DataPushVector() {
  	Data.push_back( vector <string> () );
  }
  
  void DataClear() {
  	Data.clear() ;
  }
  
  void Clear() {
  	Data.clear() ;
  	adjList.clear() ;
  	visitedMap.clear();
  	while (! q.empty() ) 
		q.pop() ; 
  }
  
} ;

int main(){

    int command = 0;
    int i = 0;
    int num = 0;
    int time = 0;
    float number = 0.0;
    string fileNum ;
    AdjacencyLists Database;
    ConnectionCounts All ;
    vector <Num> student_id ;
    bool exist = false;
    bool again = false;
    cout << endl;
    
    do{
        cout << "**** Graph data manipulation *****" << endl;
        cout << "* 0. QUIT                        *" << endl;
        cout << "* 1. Build adjacency lists       *" << endl;
        cout << "* 2. Compute connection counts   *" << endl;
        cout << "* 3. Estimate influence values   *" << endl;
        cout << "* 4. Probability-based influence *" << endl;
        cout << "**********************************" << endl;
        cout << "Input a choice(0, 1, 2, 3, 4): " ;
        
        cin >> command;
        cin.ignore( Max_LEN, '\n' );
        
        if ( command == 0 ) break;
        else if( command >= 5 ) cout << endl << "Command does not exist!" << endl << endl << endl;
        else if ( command != 1 && Database.empty() ) cout << "### There is no graph and choose 1 first. ###" << endl << endl; //非由指令1開始
        else if ( command == 1 ) { 
            if ( i != 0 ){ //重複叫1就重置 
                Database.Clear() ;
                All.Clear() ;
                student_id.clear() ;
                exist = false;
                again = false ;
            } 
            
            cout << endl << "Input a file number ([0] Quit): " ;
            cin >> fileNum;
            
            if ( fileNum == "0" ) cout << endl;
            else{
                if( Database.Load(fileNum) ) exist = true;
                
                if (exist){
                	i = i + 1;
                    if (Database.write("pairs" + fileNum)) {
                      cout << "<<< There are " << Database.size() << " nodes in total. >>>" << endl;
                    }
                }
            }
            
            cout << endl;
        }
        else if ( command == 2 ) {
        	if (!again) {
	        	All.SetADJ( Database.GetADJ() ) ;
	        	AdjList temp = Database.GetADJ() ;
				for ( AdjList::const_iterator it = temp.begin() ; it != temp.end() ; ++it ) {
					Num temp ;
					temp.str = it -> first ;
	  				student_id.push_back( temp ) ;
	  			} 
	  			
	  			
  				All.DataClear() ;
  				int k = student_id.size() ;
	  			for ( int j = 0 ; j < k ; j += 1 ) {
	  				All.Allstudent( student_id[j].str ) ;
				}

				All.Sort( student_id ) ;
				All.Writefile_cm2( fileNum, student_id ) ;
	  			again = true ;
	  		}
	  		
	  		cout << endl << "<<< There are " << All.GetDataSize() << " IDs in total. >>>" << endl << endl ;
        }
        else if ( command == 3 ) {
        	if ( ! again ) {
        		All.SetADJ( Database.GetADJ() ) ;
	        	AdjList temp = Database.GetADJ() ;
				for ( AdjList::const_iterator it = temp.begin() ; it != temp.end() ; ++it ) {
					Num temp ;
					temp.str = it -> first ;
	  				student_id.push_back( temp ) ;
	  			} 
			}
			
        	All.DataClear() ;
        	do {
        		cout << endl << "Input a real number in [0,1]: " ;
        		cin >> number ;
        		if ( number < 0 || number > 1 ) 
        			cout << endl << "### "<< number << " is NOT in [0,1] ###" << endl ;
				
			} while ( number < 0 || number > 1 ) ;
        	
			int k = student_id.size() ;
	  		for ( int m = 0 ; m < k ; m += 1 ) {
	  			All.DataPushVector() ;
	  			All.DFS( student_id[m].str, number, m) ;
	  			All.All_False() ;
//	  			cout << "------------------------------------------" << endl ;
			}
			
			All.Sort( student_id ) ;
			All.Writefile_cm3( fileNum, student_id ) ;
			cout << endl << "<<< There are " << All.CountSizeNotZero() << " IDs in total. >>>" << endl << endl ;
    	}
        else if ( command == 4 ) {
        	
        	cout << endl << "[Elapsed time] " << time << " ms" << endl << endl;
        	cout << "Input an integer to show top-K in [1, " << "]: ";
        	cin >> num;
        	cout << endl;
        }
            
    } while ( command != 0 );
    
    system("pause");
    return 0;
}

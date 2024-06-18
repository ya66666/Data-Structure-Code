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

struct Visit {
	string stid ;
	bool visited ;
	
	Visit() : stid(""), visited( false ) {}
};

typedef map<string, map<string, float> > AdjList;
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
  vector <Visit> all ;
  
public :
  
  void Initialize() {
  	for ( AdjList::const_iterator it = adjList.begin() ; it != adjList.end() ; ++it ) {
  		Visit temp ;
  		temp.stid = it -> first ;
  		all.push_back( temp ) ;
	}
  }
  
  void All_False() {
  	int j = all.size() ;
  	for( int i = 0 ; i < j ; i += 1 ) 
  		all[i].visited = false ;
  }
  
void FindID(string id) {
    int left = 0, right = all.size() - 1;  
    while (left <= right) {
        int middle = left + (right - left) / 2; 
        if (all[middle].stid < id) 
            left = middle + 1;  
        
		else if (all[middle].stid > id) 
            right = middle - 1; 

		else {
            all[middle].visited = true; 
            break;  
        }
    }
}

  
bool Visited_or_Not(string id) {
    int left = 0;
    int right = all.size() - 1;
    
    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (all[middle].stid < id) {
            left = middle + 1;
        } else if (all[middle].stid > id) {
            right = middle - 1;
        } else {
            return all[middle].visited; 
        }
    }

    return false; 
}

  
  void Allstudent( string stid ) {
  	vector <string> temp ;
	string id = stid ;
//	cout << "First id = " << id << endl ;
	FindID( id ) ;
	
	do {
		AdjList::const_iterator it = adjList.find( id ) ;
  		map<string, float> innerMap = it -> second ;
		for ( map<string, float>::const_iterator innerIt = innerMap.begin() ; innerIt != innerMap.end() ; ++innerIt ) {
			if (  ! Visited_or_Not( innerIt -> first ) ) {
//				cout << "push = " << innerIt -> first << endl ;
				temp.push_back( innerIt -> first ) ;
    			q.push( innerIt -> first ) ;	
    			FindID( innerIt -> first ) ;
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
  
  void SetADJ( const AdjList& adjacencyList ) {
  	adjList = adjacencyList ;
  	Initialize() ;
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
  
  void Writefile( string file, vector <Num> student_id ) {
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
				fout << endl << "\t" ;
			
			if ( j + 1 < 10 )
				fout << "( " << j + 1 << ") " << Data[i][j] << "\t" ;
			else 
				fout << "(" << j + 1 << ") " << Data[i][j] << "\t" ;
		}
		
		fout << endl ;
	}
	
  	fout.close();
  }
  
  int GetDataSize() {
  	return Data.size() ;
  }
  
  void Clear() {
  	Data.clear() ;
  	all.clear() ;
  	adjList.clear() ;
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

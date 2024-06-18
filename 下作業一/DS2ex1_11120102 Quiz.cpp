// 11120102 莊凱任
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

struct VectorInfo {
	int order_num ; //序號 
	string school_name ; //學校名稱 
	string subject_name ; //科系名稱
	string day_night ; // 日間∕進修別
	string level ; // 等級別
	int student_num ; //學生數 
};

class Info {
	private:
		vector< VectorInfo > info ;
	
	public:
		int parent, bottom, root = 0, grandparent, min_or_max, top = 1 ;
		
		void Push( VectorInfo push ) {
			info.push_back( push ) ;			
		}
		
		VectorInfo Element( int num ) {
			return info[num] ;
		}
		
		int Size() {
			return info.size() ;
		}
		
		void Toggle_Up() {
			bottom = info.size() - 1 ;
			parent = ( bottom - 1 ) / 2 ;
			
			while ( parent >= 0 && info[bottom].student_num < info[parent].student_num ) {
				swap( info[bottom], info[parent] ) ;
				bottom = parent ;
				parent = ( bottom - 1 ) / 2 ;
			}
		}
		
		void Compare() {
			min_or_max = log( info.size() ) / log(2) ;
			min_or_max %= 2 ; // 0 min, 1 max
			
			bottom = info.size() - 1 ;
			parent = ( bottom - 1 ) / 2 ;
			
			if ( min_or_max == 0 ) { // 現在在min
				if ( parent >= 0 && info[parent].student_num < info[bottom].student_num ) {
					swap( info[bottom], info[parent] ) ;
					bottom = parent ;
					parent = ( bottom - 1 ) / 2 ;
					min_or_max = 1 ;
				}
			}
			else {
				if ( parent >= 0 && info[parent].student_num > info[bottom].student_num ) { // 現在在max
					swap( info[bottom], info[parent] ) ;
					bottom = parent ;
					parent = ( bottom - 1 ) / 2 ;
					min_or_max = 0 ;
				}
			}
		}
		
		void Toggle_Up_GrandParent() {
			grandparent = ( parent - 1 ) / 2 ;
			
			if ( min_or_max == 0 ) { // 現在在min
				while ( bottom > 2 && info[bottom].student_num < info[grandparent].student_num ) {
					swap( info[bottom], info[grandparent] ) ;
					bottom = grandparent ;
					grandparent = ( bottom - 3 ) / 4 ;
				}
			}
			else { // 現在在max
				while ( bottom > 2 && info[bottom].student_num > info[grandparent].student_num ) {
					swap( info[bottom], info[grandparent] ) ;
					bottom = grandparent ;
					grandparent = ( bottom - 3 ) / 4 ;
				}
			}
		}
		
		void Delete() {
			int cur = Pop_Last() ;
			if ( info.size() > 3 ) 
				ReheapDown( cur ) ;
		}
		
		int Pop_Last() {
			bottom = info.size() - 1 ;
			int largest = Find_Largest() ;
			swap( info[largest], info[bottom] ) ;
			Print() ;
			info.pop_back() ;
			return largest ;
		}
		
		void ReheapDown( int cur ) {
			int leftchild = cur * 2 + 1, rightchild = cur * 2 + 2, grandchild = cur * 4 + 3, lar_gc = grandchild ;
			do {
				if ( rightchild < info.size() ) { // 右邊小於總數 
					Child_Swap( cur, leftchild, rightchild ) ;
					if ( grandchild < info.size() ) {
						for ( int i = 1 ; i < 4 ; i ++ ) { //找最大 
							if ( grandchild + i < info.size() && info[lar_gc].student_num < info[grandchild + i].student_num ) {
								lar_gc = grandchild + i ;
							}
						}
						
						if ( info[cur].student_num < info[lar_gc].student_num ) {
							swap( info[cur], info[lar_gc] ) ;
							cur = lar_gc ;
							grandchild = cur * 4 + 3 ;
							lar_gc = grandchild ;
							leftchild = cur * 2 + 1 ;
							rightchild = cur * 2 + 2 ;
						}
						
						else break ;
					}
					
					else break ;
				}
					
				else if ( leftchild < info.size() && rightchild >= info.size() ) {// 右邊大於總數 
					if ( info[leftchild].student_num > info[cur].student_num ) 
						swap( info[leftchild], info[cur] ) ;
						
					break ;
				}
				
				else break ;
				
			} while ( cur < info.size() ) ;
		}
		
		void Child_Swap( int cur, int leftchild, int rightchild ) {	
			if ( ( info[leftchild].student_num >= info[rightchild].student_num ) && ( info[leftchild].student_num > info[cur].student_num ) ) 
				swap( info[leftchild], info[cur] ) ;
			
			else if ( ( info[leftchild].student_num < info[rightchild].student_num ) && ( info[rightchild].student_num > info[cur].student_num ) ) 
				swap( info[rightchild], info[cur] ) ;
				
				
		}
		
		int Find_Largest() {
			if ( info.size() == 1 || info.size() == 2 ) 
				return info.size() - 1 ;
			
			else {
				if ( info[1].student_num >= info[2].student_num  ) 
					return 1 ;
				
				else if ( info[1].student_num < info[2].student_num )
					return 2 ;
			}
			
		}
		
		void Print() {
			
			if ( top < 10 ) 
				cout << "Top   " ;
			else if ( top >= 10 && top < 100 )
				cout << "Top  " ;
			else 
				cout << "Top " ;
			
			cout << top << ": [" << info[bottom].order_num << "] " << info[bottom].school_name ;
			cout << info[bottom].subject_name << ", " << info[bottom].day_night << ", " ;
			cout << info[bottom].level << ", " << info[bottom].student_num << endl ;
			top ++ ;
		}
		
		bool IsEmpty() {
			return info.empty() ;
		}
		
		void PrintAll() {
			for ( int i = 0 ; i < info.size() ; i ++ ) {
				cout << info[i].order_num << "    " << info[i].student_num << endl ;
			}
		}
		
		void ClearAll() {
			info.clear() ;
		}
}; 

class Procedure {
	public:
		ifstream fin ;
		int cmd ;
		Info min_heap, min_max_heap ; // all為初始資料 
		
		bool GetCmd() {
			while ( true ) {
				cout << endl << "********** Heap Construction **********" << endl ;
				cout << "* 0. QUIT                             *" << endl ;
				cout << "* 1. Build a min heap                 *" << endl ;
				cout << "* 2. Build a min-max heap             *" << endl ;
				cout << "* 3: Top-K maximums from min-max heap *" << endl ;
				cout << "***************************************" << endl ;
				cout << "Input a choice(0, 1, 2, 3): " ;
				cin >> cmd ; //讀指令 
				if ( cmd == 1 || cmd == 2 )
					cout << endl ;
				
				if ( cmd == 1 || cmd == 2 || cmd == 3 ) return true ;
				
				else if ( cmd == 0 ) return false ; // end
				
				else cout << "Command does not exist!" << endl ;
				
			} // while	
		} // GetCommand()
		
		bool ReadFile() {
			string file_num, garbage ;
			char ch ;
			int num = 1 ;
			do {
				cout << "Input a file number ([0] Quit): " ;
				cin >> file_num ;
				
				if ( file_num == "0" ) return false ;
				
				fin.open( "input" + file_num + ".txt" ) ;
				if ( fin.is_open() ) {
					for ( int i = 0 ; i < 3 ; i ++ ) 
						getline( fin, garbage ) ;
					
					getline( fin, garbage, '\t' ) ;
							
					while( ! fin.eof() ) {
						VectorInfo * package = new VectorInfo ;
						getline( fin, package -> school_name, '\t' ) ;
						getline( fin, garbage, '\t' ) ;
						getline( fin, package -> subject_name, '\t' ) ;
						getline( fin, package -> day_night, '\t' ) ;
						getline( fin, package -> level, '\t' ) ;
						
						fin >> ch ;
						package -> student_num = To_num( ch ) ; 
						package -> order_num = num ;
						num += 1 ;
						if ( cmd == 1 ) {
							min_heap.Push( *package ) ;
							min_heap.Toggle_Up() ;
						}
						else if ( cmd == 2 ) {
							min_max_heap.Push( *package ) ;
							min_max_heap.Compare() ;
							min_max_heap.Toggle_Up_GrandParent() ;
						}
						
						getline( fin, garbage ) ;
						getline( fin, garbage, '\t' ) ;
					}
					
					fin.close() ;
					return true ;
				}
				
				else {
					cout << endl << "### input" << file_num << ".txt does not exist! ###" << endl ;
				}
				
				cout << endl ;
			} while ( file_num != "0" ) ;
			
		}
		
		int To_num( char ch ) {
			int num = 0 ;
			
			while ( ch != '\t' ) {
				if ( ch == ',' ) {
					fin.get( ch ) ;
				}
				
				num *= 10 ;
				num += ch - 48 ;
				fin.get( ch ) ;
			}
			
			return num ;
		}
		
		void To_min_heap() {
			int bottom = 0, leftmost = 0, size = 0;
			
			bottom = min_heap.Size() - 1 ;
			size = min_heap.Size() ;
			leftmost = ( log( size ) / log(2) ) ;
			leftmost = pow( 2, leftmost ) - 1 ;
			cout << "<min heap>" << endl ;
			cout << "root: [" << min_heap.Element( min_heap.root ).order_num << "] " << min_heap.Element( min_heap.root ).student_num << endl ;
			cout << "bottom: [" << min_heap.Element( bottom ).order_num << "] " << min_heap.Element( bottom ).student_num << endl ;
			cout << "leftmost bottom: [" << min_heap.Element( leftmost ).order_num << "] " << min_heap.Element( leftmost ).student_num << endl ;
		}
		
		void To_min_max_heap() {
			int bottom = 0, leftmost = 0, size = 0;
			
//			min_max_heap.PrintAll() ;
			bottom = min_max_heap.Size() - 1 ;
			size = min_max_heap.Size() ;
			leftmost = ( log( size ) / log(2) ) ;
			leftmost = pow( 2, leftmost ) - 1 ;
			cout << "<min-max heap>" << endl ;
			cout << "root: [" << min_max_heap.Element( min_max_heap.root ).order_num << "] " << min_max_heap.Element( min_max_heap.root ).student_num << endl ;
			cout << "bottom: [" << min_max_heap.Element( bottom ).order_num << "] " << min_max_heap.Element( bottom ).student_num << endl ;
			cout << "leftmost bottom: [" << min_max_heap.Element( leftmost ).order_num << "] " << min_max_heap.Element( leftmost ).student_num << endl ;
		}
		
		void Cmd_3() {
			int num, repeat ;
			num = min_max_heap.Size() ;
			cout << endl << "Enter the value of K in [1," << num << "]: " ;
			cin >> repeat ;
			
			if ( repeat <= num && repeat >= 1 ) {
				for( int i = 0 ; i < repeat ; i ++ ) {
					min_max_heap.Delete() ;
					if ( ( i + 1 ) % 2000 == 0 )
						system( "pause" ) ;
				}
					
			}
		}
};

int main() {
	Procedure procedure ;
	bool press_2_before = false ;
	
	while ( procedure.GetCmd() ) {
		if ( procedure.cmd == 1 ) {
			if ( procedure.ReadFile() ) 
				procedure.To_min_heap() ;
				
			procedure.min_heap.ClearAll() ;	
		}
		
		else if ( procedure.cmd == 2 ) {
			procedure.min_max_heap.ClearAll() ;	
			
			if ( procedure.cmd == 2 && procedure.ReadFile() ) {
				procedure.To_min_max_heap() ;
				press_2_before = true ;
			}
		}
		
		else if ( procedure.cmd == 3 && press_2_before ) {
			if ( ! procedure.min_max_heap.IsEmpty() ) {
				procedure.Cmd_3() ;
				procedure.min_max_heap.top = 1 ;
			}
		}
		
	} // while
}


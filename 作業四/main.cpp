// 11120102莊凱任、11120103游舒涵 
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

struct Data {
	int school_code ;
	string school_name, department_code, department_name, day1, day2, day3, day4, students ;
	int teachers, graduation ;
	Data * next, * left, * right ;
};

class SortedList {
	private:
		Data * head = NULL, * tail = NULL ;
		int size = 0 ;
	
	public:
		Data * curr = head, * front = head ;
		
		void Push( Data * node ) {
			if ( head == NULL ) 
				head = tail = node ; 
			
			else {
				tail -> next = node ;
				tail = tail -> next ;
			}
			
			size += 1 ;
		}
		
		Data * GetCurr() {
			
			Data * duplicate = NULL ;
			if ( front == head ) {
				front = front -> next ;
				duplicate = new Data( *curr ) ;
				
				return duplicate ;
			}
			
			curr = front ;
			front = front -> next ;
			duplicate = new Data( *curr ) ;
			
			return duplicate ;
			
		}
		
//		Data * Copy() {
//			
//			Data * copy = new Data ;
//			copy -> school_code = curr -> school_code ;
//			copy -> school_name = curr -> school_name ;
//			copy -> department_code = curr -> department_code ;
//			copy -> department_name = curr -> department_name ;
//			copy -> day1 = curr -> day1 ;
//			copy -> day2 = curr -> day2 ;
//			copy -> day3 = curr -> day3 ;
//			copy -> day4 = curr -> day4 ;
//			copy -> students = curr -> students ;
//			copy -> teachers = curr -> teachers ;
//			copy -> graduation = curr -> graduation ;
//			copy -> next = NULL ;
//			copy -> right = NULL ;
//			copy -> left = NULL ;
//			
//			return copy ;
//		}
		
		void SetSize( int num ) {
			size = num ;
		}
		
		void SetHead( Data * &node ) {
			head = node ;
		}
		
		int Get_Graduation() {
			return curr -> graduation ;
		}
		
		Data * GetHead() {
			return head ;
		}
		
		int Get_Size() {
			return size ;
		}
		
		void Initialize_Curr() {
			curr = front = head ;
		}
		
		void Delete( Data * curr ) {
			
		}
		
		void DeleteAll( Data * &del ){
			if( del == NULL )
				return;
			
			DeleteAll(del->next);
			DeleteAll(del->left);
			DeleteAll(del->right);
			delete del;
			del = NULL;
		}
				
		void Initialize() {
			DeleteAll( head );
			head = tail = NULL ;
			curr = front = head ;
			size = 0;
		}

};

class Procedure{
	public:
		ifstream fin;
		int command = -1, fileNum, max = 0, length = 0 ;
		SortedList sorted, graduate_list, school_name_list ;
		
		int GetCommand(){
			while( true ){
				cout << endl;
				cout << "*** University Graduate Information System ***" << endl;
				cout << "* 0. Quit                                    *" << endl;
				cout << "* 1. Create Two Binary Search Trees          *" << endl;
				cout << "* 2. Search by Number of Graduates           *" << endl;
				cout << "* 3. Search by School Name                   *" << endl;
				cout << "* 4. Removal by School Name                  *" << endl;
				cout << "**********************************************" << endl; 
				cout << "Input a command(0, 1-4): ";
				
				string get;
				cin >> get;
				
				if(  command == -1 && ( stoi(get) == 2 || stoi(get) == 3 ) )
					cout << endl << "Please choose command 1 first!" << endl;
				else if( stoi(get) == 0 || stoi(get) == 1 || stoi(get) == 2 || stoi(get) == 3 || stoi( get ) == 4 ){
					command = stoi(get);
					return command;
				}
				else cout << "Command does not exist!" << endl;
			}
		} // GetCommand()


		bool ReadFile(){
			sorted.Initialize();
			graduate_list.Initialize();
			school_name_list.Initialize();
			max = length = 0;
			
			cout << endl << "Input a file number: ";
			string get;
			cin >> get; 
			
			fin.open( "input"+ get +".txt" );
			if( ! fin.is_open() ) { 
				cout << endl << "### input" << get << ".txt does not exist! ###" << endl;
				cout << endl << "There is no data!" << endl;
				command = -1;
				return false;
			}
			fileNum = stoi(get);

			string garbage;
			for ( int i = 0 ; i < 15 ; i++ ) 
				fin >> garbage ;
			
			while( ! fin.eof() ) {
				Data * get = new Data ;
				if ( fin >> get -> school_code != 0 ) {
					fin >> get -> school_name >> get -> department_code >> get -> department_name >> get -> day1 >> get -> day2 >> get -> day3 >> get -> day4 ;
					fin >> get -> students >> get -> teachers >> get -> graduation ;
					fin >> garbage >> garbage >> garbage >> garbage ;
					get -> next = NULL ;
					get -> right = NULL ;
					get -> left = NULL ;
//				 	cout << get -> school_code << get -> school_name << get -> department_code << get -> department_name << get -> day1 << get -> day2 << get -> day3 ;
//					cout << get -> day4 << get -> students << get -> teachers << get -> graduation ;
//				 	cout << endl ;
					sorted.Push( get ) ;
				}
				
			}

			fin.close();
			return true ;
		} // ReadFile()
		
		void Compare( int height ) {
			if ( height > length ) 
				length = height ;
		}

		void Establish() {
			sorted.Initialize_Curr() ;
			Data * temp = school_name_list.GetHead() ;
			int height = 0 ;
			for ( int i = 0 ; i < sorted.Get_Size() ; i += 1 ) {
				Binary_Tree_school_name( temp, sorted.GetCurr(), height ) ;
			}
			
			cout << "Tree height {School name} = " << length << endl ;
			school_name_list.SetHead( temp ) ;
//----------------------------------------------------------------------------------------
			temp = graduate_list.GetHead() ;
			sorted.Initialize_Curr() ;
			length = 0 ;
			for ( int i = 0 ; i < sorted.Get_Size() ; i += 1 ) {
				Binary_Tree_graduate( temp, sorted.GetCurr(), height ) ;
			}
			
			cout << "Tree height {Number of graduates} = " << length << endl ;
			graduate_list.SetHead( temp ) ;
			
//			sorted.Initialize() ;
//			if ( sorted.GetHead() == NULL ) 
//				cout << "sorted" << endl ;
//				
//			graduate_list.Initialize() ;
//			if ( graduate_list.GetHead() == NULL ) 
//				cout << "graduate" << endl ;
//				
//			school_name_list.Initialize() ;
//			if ( school_name_list.GetHead() == NULL ) 
//				cout << "school name" << endl ;
		}
		
//		void Count_Height( Data * root, int height ) {
//			
//			if ( root == NULL ) {
//				height += 1 ;
//				Compare( height ) ;
//				return ;
//			}
//			
//			if ( root -> school_code != -1 ) 
//				height += 1 ;
//			
//			Count_Height( root -> left, height ) ;
//			Count_Height( root -> right, height ) ;
//			
//		}
		
		void Binary_Tree_graduate( Data * &root, Data * node, int height ) {
			
			if ( root == NULL ) {
				root = node ;
				height += 1 ;
//				cout << height << endl ;
				Compare( height ) ;
				return ;
			}
			
			else if ( node -> graduation >= root -> graduation ) {
//				cout << height + 1 << endl ;
				Binary_Tree_graduate( root -> right, node, height + 1 ) ;
				
				
			}
			
			else if ( node -> graduation < root -> graduation ) {
//				cout << height + 1 << endl ;
				Binary_Tree_graduate( root -> left, node, height + 1 ) ;
			}
			
		}
		
		void Binary_Tree_school_name( Data * &root, Data * node, int height ) {
			
			if ( root == NULL ) {
				root = node ;
				height += 1 ;
//				cout << height << endl ;
				Compare( height ) ;
				return ;
			}
			
			else if ( node -> school_name.compare( root -> school_name ) >= 0 ) {
				Binary_Tree_school_name( root -> right, node, height + 1 ) ;
			}
			
			else if ( node -> school_name.compare( root -> school_name ) < 0 ) {
				Binary_Tree_school_name( root -> left, node, height + 1 ) ;
			}
			
		}
		
		bool IsNumetric( string str ){
			for( int i = 0; i < str.length() ; i++ ){
				if( ! isdigit(str[i]) )
					return false;
			}
			return true;
		} // IsNumetric()
		
		void Search(){
			string search;
			bool find = false;
			int i = 0;
			
			if( command == 2 ){
				cout << "Input the number of graduates: ";
				cin >> search;
				if( IsNumetric(search) )
					Search_NumOfGraduates(i,graduate_list.GetHead(),stoi(search),find);
				else cout << "### the input string " << search << "is not a decimal number! ###" << endl;
			}
				
			else if( command == 3 ){
				cout << "Input a school name: ";
				cin >> search;
				Search_SchoolName(i,school_name_list.GetHead(),search,find);
			}
			
			if( ! find )
				cout << "There is no match!" << endl;
		} // Search
		
		
		
		
		void Search_NumOfGraduates( int & i, Data * curr, int search, bool & find ){
			if( curr == NULL )
				return;	
			else if( curr->graduation >= search ){
				if( i == 0 )
					cout << "Search results:" << endl;
				cout << "[" << ++i << "]\t" << curr->school_name << "\t" << curr->department_name
					 << "\t" << curr->day1 << " " << curr->day2 << "\t" << curr->day3 << " " << curr->day4
					 << "\t" << curr->students << "\t" << curr->teachers << "\t" << curr->graduation << endl;
				find = true;
			}
			
			if( search < curr->graduation )
				Search_NumOfGraduates(i,curr->left,search,find);
			Search_NumOfGraduates(i,curr->right,search,find);
		} // Search_NumOfGraduates()
		
		
		
		
		void Search_SchoolName( int & i, Data * curr, string search, bool & find ){
			if( curr == NULL )
				return;
			else if( search.compare(curr->school_name) == 0 ){
				if( i == 0 )
					cout << "Search results:" << endl;
				cout << "[" << ++i << "]\t" << curr->school_name << "\t" << curr->department_name
					 << "\t" << curr->day1 << " " << curr->day2 << "\t" << curr->day3 << " " << curr->day4
					 << "\t" << curr->students << "\t" << curr->teachers << "\t" << curr->graduation << endl;
				find = true;
			}
					 
			if( search.compare(curr->school_name) < 0 )
				Search_SchoolName(i,curr->left,search,find);
			else
				Search_SchoolName(i,curr->right,search,find);			
		} // Search_SchoolName()
		
		void Del() {
			cout << "Input a school name: " ;
			string school ;
			cin >> school ;
			Data * h = sorted.GetHead() ;
			Data * curr = h, * curr_front = h ;
			int i = 0, size = sorted.Get_Size() ;
			while ( curr_front != NULL ) {
				if ( school.compare( h -> school_name ) == 0 ) {
					h = h -> next ; 
					curr_front = h ;
					curr -> next = NULL ;
					cout << "[" << ++i << "]\t" << curr->school_name << "\t" << curr->department_name
					<< "\t" << curr->day1 << " " << curr->day2 << "\t" << curr->day3 << " " << curr->day4
					<< "\t" << curr->students << "\t" << curr->teachers << "\t" << curr->graduation << endl;
					delete( curr ) ;
					curr = curr_front ;
					size -= 1 ;
				}
				else if ( school.compare( curr_front -> school_name ) == 0 ) {
					curr -> next = curr_front -> next ;
					curr_front -> next = NULL ;
					cout << "[" << ++i << "]\t" << curr_front->school_name << "\t" << curr_front->department_name
					<< "\t" << curr_front->day1 << " " << curr_front->day2 << "\t" << curr_front->day3 << " " << curr_front->day4
					<< "\t" << curr_front->students << "\t" << curr_front->teachers << "\t" << curr_front->graduation << endl;
					delete( curr_front ) ;
					curr_front = curr -> next ;
					size -= 1 ;
				}
				else {
					curr = curr_front ;
					curr_front = curr_front -> next ;
				}
			}
			
			sorted.SetHead( h ) ;
			sorted.SetSize( size ) ;
		}
		
} ; // class 

int main( int argc, char** argv ) {
	Procedure p;
	
	while( p.GetCommand() ){ // command為0會結束 
		
		if( p.command == 1 && p.ReadFile() ) {
			p.Establish() ;
		}
		
		else if( p.command == 2 || p.command == 3 )
			p.Search();
			
		else if( p.command == 4 ) {
			p.Del() ;
			p.Establish() ;
		}
	} // while
}









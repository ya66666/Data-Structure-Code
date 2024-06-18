/* 11120102 莊凱任 */
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <windows.h>

using namespace std;

ofstream fout;

void Bubble( vector <int> arr ) {
	int swapp = 0, pass = 1 ;
	bool swapp_equal_zero = false ;
	fout << "Bubble sorting" << endl ;
	
	clock_t timer1 = clock() ;
	for ( int num = arr.size() - 1 ; num > 0 ; num -= 1, pass += 1, swapp = 0 ) {
		for ( int i = 1 ; i <= num ; i += 1 ) {
			if ( arr[i-1] > arr[i] ) {
				swap( arr[i-1], arr[i] ) ;
				swapp += 1 ;
			}
		}
		
		if ( ! swapp_equal_zero ) {
			fout << "Pass = " << pass << ", " << "Swap = " << swapp << ", " << "State : " ;
			for ( int i = 0 ; i < arr.size() ; i += 1 ) 
				fout << arr[i] << " " ;
		
			fout << endl ;
			
			if ( swapp == 0 ) 
				swapp_equal_zero = true ;
		}
		
		else break ;
	}
	
	Sleep( 1000 ) ;
	clock_t timer2 = clock() ;
	fout << "執行時間 = " << ( double )( timer2 - timer1 - 1000 ) / CLOCKS_PER_SEC << " sec" << endl ;
	fout << endl ;
}

void Insertion( vector <int> arr ) {
	int swapp = 0, pass = 1 ;
	bool swapp_equal_zero = false ;
	clock_t timer1 = 0, timer2 = 0 ;
	
	fout << "Insertion sorting" << endl ;
	timer1 = clock() ;
	for ( int i = 1 ; i < arr.size() ; i += 1, pass += 1, swapp = 0 ) {
		for ( int j = i ; j > 0 ; j -= 1 ) {
			if ( arr[j-1] > arr[j] ) {
				swap( arr[j-1], arr[j] ) ;
				swapp += 1 ;
			}
		}
		
		fout << "Pass = " << pass << ", " << "Swap = " << swapp << ", " << "State : " ;
		for ( int i = 0 ; i < arr.size() ; i += 1 ) 
			fout << arr[i] << " " ;
		
		fout << endl ;
	}
	
	Sleep( 1000 ) ;
	timer2 = clock() ;
	fout << "執行時間 = " << ( double )( timer2 - timer1 - 1000 ) / CLOCKS_PER_SEC << " sec" << endl ;
}

int main(int argc, char** argv) {
	int random_num = rand() % 101 ; //產生亂數 
	vector <int> one, two ;
	one.push_back( random_num ) ;
	bool repeat = false ;
	
	while ( one.size() != 20 ) {
		random_num = rand() % 101 ;
		for( int i = 1 ; i < one.size() ; i += 1 ) { //檢查重複 
			if( one[i] == random_num ) 
				repeat = true ;
		}
		
		if ( ! repeat ) 
			one.push_back( random_num ) ;
		
		repeat = false ;
	} 
	
	two = one ;
	
	fout.open( "Output.txt", ios::out|ios::trunc ) ;
	Bubble( one ) ;
	Insertion( two ) ;
	fout.close() ;
}

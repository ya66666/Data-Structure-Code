#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

struct F {
	int i ;
	F * next ;
};

int main( int argc, char** argv ) {
	F * head = new F ;
	F * cur = head ;
	
	head -> i = 123 ;
	head -> next = new F ;
	head -> next -> i = 12345 ;
	head -> next -> next = NULL ;
	
	cur = head ;
	while ( cur != NULL ) {
		cout << cur -> i << endl ;
		cur = cur -> next ; 
	}
	
	cur = NULL ;
	if ( head == NULL ) {
		cout << "gggggg" << endl ;
	}
	while ( head != NULL ) {
		cout << head -> i << endl ;
		head = head -> next ; 
	}
}

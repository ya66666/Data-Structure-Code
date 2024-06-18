// 11120102 莊凱任 
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
ifstream ifs ;
static int length = 0, width = 0 ;
static int numOfGoal = 0 ;

class Block {
	
public :
	char state  ;
	Block * right ;
   	Block * down ;
  	Block * left ;
   	Block * up ;
};

Block * start = new Block() ; //接頭 
Block * run = start ; //跑迷宮用的 
bool goal = false ; //有沒有找到一個goal 
string filename ;

void Run_the_maze1(  ) ;
void Run_the_maze2(  ) ;
void Run_the_maze3(  ) ;
void Print_the_maze1( Block * ) ;
void Print_the_maze2( Block * ) ;
void Print_the_maze3( Block * ) ;
void Many_goal1( int ) ;
void Many_goal2( int ) ;
void Many_goal3( int ) ;
void Many_goal4( int ) ;


bool Input_file() {
	string num ;
	cout << "Input a file number: " ;
	cin >> num ;
	filename = "input" + num + ".txt" ; // 改成檔名的樣子 
	ifs.open( filename ) ;
	if ( !ifs.is_open() ) {
		cout << filename << " does not exist! " << endl << endl ;
		return false ;
	}
}

void Get_width_length() {
	// read 先 width -> 後 length
	ifs >> width >> length ;

}

void Length_create() {
	Block * connect = NULL ; // 用來連接左右的指標 
	Block * dconnect = NULL ; //用來連接上下的指標 
	connect = start, dconnect = start ;
	
	for ( int i = 0 ; i < length ; i += 1 ) {
		if ( i == length - 1 ) {
			dconnect -> down = NULL ; //最下面NULL 
		}
		
		for ( int j = 0 ; j < width ; j += 1 ) {
			
			connect -> right = new Block ;
			connect -> right -> left = connect ;//向左連接 
			connect = connect -> right ;
			
			if ( i == 0 ) 
				connect -> up = NULL ; //最上面NULL 
			
			if ( i != 0 ) {
				connect -> up = connect -> left -> up -> right ; // 向上連接 
				connect -> left -> up -> right -> down = connect ; // 向下連接 
			}
			
			if ( i == length - 1 ) 
				dconnect -> down = NULL ; //最下面NULL  
			
			if ( j == width - 1 ) 
				connect -> right = NULL ;//最右邊NULL 
				
			//read in character
			connect -> state = ifs.get() ;
			if ( connect -> state == '\n' )
				connect -> state = ifs.get() ;
		}
		
		dconnect -> down = new Block() ;
		dconnect -> down -> up = dconnect ;
		dconnect = dconnect -> down ;
		dconnect -> left = NULL ; //最左邊NULL 
		connect = dconnect ;
	}
}

void Maze_Establishment() {
	start -> up = NULL, start -> left = NULL ;
	Get_width_length() ;
	start -> state = ifs.get() ;
	if ( start -> state == '\n' )
		start -> state = ifs.get() ;
	
	Length_create() ;
}

//---------------------------------------------------------------------------------------

void Many_goal4( int numOfGoal ) {
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( numOfGoal == 0 ) 
	  return ;
	
	if ( run -> up -> state == 'E' || run -> up -> state == 'G' ) {
		run = run -> up ;
		if ( run -> up -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal4( numOfGoal ) ;
	}	
	
	if ( run -> right -> state == 'E' || run -> right -> state == 'G' ) {
		run = run -> right ;
		if ( run -> right -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal1( numOfGoal ) ;
	}
	
	if ( run -> down -> state == 'E' || run -> down -> state == 'G' ) {
		run = run -> down ;
		if ( run -> down -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal2( numOfGoal ) ;
	}	
	
	if ( run -> left -> state == 'E' || run -> left -> state == 'G' ) {
		run = run -> left ;
		if ( run -> left -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal3( numOfGoal ) ;
	}
}

void Many_goal3( int numOfGoal ) {
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( numOfGoal == 0 )
		return ;
	
	if ( run -> left -> state == 'E' || run -> left -> state == 'G' ) {
		run = run -> left ;
		if ( run -> left -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal3( numOfGoal ) ;
	}
	
	if ( run -> up -> state == 'E' || run -> up -> state == 'G' ) {
		run = run -> up ;
		if ( run -> up -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal4( numOfGoal ) ;
	}	
	
	if ( run -> right -> state == 'E' || run -> right -> state == 'G' ) {
		run = run -> right ;
		if ( run -> right -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal1( numOfGoal ) ;
	}
	
	if ( run -> down -> state == 'E' || run -> down -> state == 'G' ) {
		run = run -> down ;
		if ( run -> down -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal2( numOfGoal ) ;
	}	
	
}

void Many_goal2( int numOfGoal ) {
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( numOfGoal == 0 )
		return ;

	if ( run -> down -> state == 'E' || run -> down -> state == 'G' ) {
		run = run -> down ;
		if ( run -> down -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal2( numOfGoal ) ;
	}
	
	if ( run -> left -> state == 'E' || run -> left -> state == 'G' ) {
		run = run -> left ;
		if ( run -> left -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal3( numOfGoal ) ;
	}
	
	if ( run -> up -> state == 'E' || run -> up -> state == 'G' ) {
		run = run -> up ;
		if ( run -> up -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal4( numOfGoal) ;
	}	
	
	if ( run -> right -> state == 'E' || run -> right -> state == 'G' ) {
		run = run -> right ;
		if ( run -> right -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal1( numOfGoal ) ;
	}
}

void Many_goal1( int numOfGoal ) { //遇到G繼續直走  
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( numOfGoal == 0 )// 找到 一定數量的G或是跑太多次就結束 
		return ;
	
	if ( run -> right -> state == 'E' || run -> right -> state == 'G' ) {
		run = run -> right ;
		if ( run -> right -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal1( numOfGoal ) ;
	}
	
	if ( run -> down -> state == 'E' || run -> down -> state == 'G' ) {
		run = run -> down ;
		if ( run -> down -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal2( numOfGoal ) ;
	}
	
	if ( run -> left -> state == 'E' || run -> left -> state == 'G' ) {
		run = run -> left ;
		if ( run -> left -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal3( numOfGoal ) ;
	}
	
	if (  run -> up != NULL && run -> up -> state == 'E' || run -> up -> state == 'G' ) {
		run = run -> up ;
		if ( run -> up -> state == 'G' ) {
			numOfGoal -= 1 ;
		}
		Many_goal4( numOfGoal ) ;
	}	
}

//---------------------------------------------------------------------------------------

void Run_the_maze4() {
	cout << "up" << endl ;
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( run -> up != NULL && ( run -> up -> state == 'E' || run -> up -> state == 'G' ) && !goal ) {
		run = run -> up ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze4(  ) ;
	}
	
	if ( run -> right != NULL && ( run -> right -> state == 'E' || run -> right -> state == 'G' ) && !goal ) {
		run = run -> right ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze1(  ) ;
	}
	
	if ( run -> down != NULL && ( run -> down -> state == 'E' || run -> down -> state == 'G' ) && !goal ) {
		run = run -> down ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze2(  ) ;
	}
	
	if ( run -> left != NULL && ( run -> left -> state == 'E' || run -> left -> state == 'G' ) && !goal ) {
		run = run -> left ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze3(  ) ;
	}
}

void Run_the_maze3() {
	cout << "left" << endl ;
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( run -> left != NULL && ( run -> left -> state == 'E' || run -> left -> state == 'G' ) && !goal ) {
		run = run -> left ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze3(  ) ;
	}
	
	if ( run -> up != NULL && ( run -> up -> state == 'E' || run -> up -> state == 'G' ) && !goal ) {
		run = run -> up ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze4(  ) ;
	}
	
	if ( run -> right != NULL && ( run -> right -> state == 'E' || run -> right -> state == 'G' ) && !goal ) {
		run = run -> right ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze1(  ) ;
	}
	
	if ( run -> down != NULL && ( run -> down -> state == 'E' || run -> down -> state == 'G' ) && !goal ) {
		run = run -> down ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze2(  ) ;
	}
}

void Run_the_maze2() {
	cout << "down" << endl ;
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( run -> down != NULL && (run -> down -> state == 'E' || run -> down -> state == 'G' ) && !goal ) {
		run = run -> down ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze2() ;
	}
	
	if ( run -> left != NULL && ( run -> left -> state == 'E' || run -> left -> state == 'G' ) && !goal ) {
		run = run -> left ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze3() ;
	}
	
	if ( run -> up != NULL && ( run -> up -> state == 'E' || run -> up -> state == 'G') && !goal ) {
		run = run -> up ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze4() ;
	}
	
	if ( run -> right != NULL && ( run -> right -> state == 'E' || run -> right -> state == 'G' ) && !goal ) {
		run = run -> right ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze1() ;
	}
}

void Run_the_maze1() {
	cout << "right" << endl ;
	if ( run -> state == 'E' ) 
		run -> state = 'V' ;
	
	if ( run -> right != NULL && ( run -> right -> state == 'E' || run -> right -> state == 'G' ) && !goal ) {
		run = run -> right ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze1() ;
	}
	
	if ( run -> down != NULL && ( run -> down -> state == 'E' || run -> down -> state == 'G' ) && !goal ) {
		run = run -> down ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze2() ;
	}
	
	if ( run -> left != NULL && ( run -> left -> state == 'E' || run -> left -> state == 'G' ) && !goal ) {
		run = run -> left ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze3() ;
	}
	
	if ( run -> up != NULL && ( run -> up -> state == 'E' || run -> up -> state == 'G' ) && !goal ) {
		run = run -> up ;
		if ( run -> state == 'G' ) {
			goal = true ;
			return ;
		}
		Run_the_maze4() ;
	}	
}

//---------------------------------------------------------------------------------------

void Print_the_maze4( Block * change ) {
	change -> state = 'R' ;
	
	if ( change -> right -> state != 'V' && change -> down -> state != 'V' && change -> left -> state != 'V' && change -> up -> state != 'V' ) 
		return ;

	if ( change -> up -> state == 'V' ) {
		change = change -> up ;
		Print_the_maze4( change ) ;
	}
	
	if ( change -> right -> state == 'V' ) {
		change = change -> right ;
		Print_the_maze1( change ) ;
	}
		
	if ( change -> down -> state == 'V' ) {
		change = change -> down ;
		Print_the_maze2( change ) ;
	}
	
	if ( change -> left -> state == 'V' ) {
		change = change -> left ;
		Print_the_maze3( change ) ;
	}
}

void Print_the_maze3( Block * change ) {
	change -> state = 'R' ;
	
	if ( change -> right -> state != 'V' && change -> down -> state != 'V' && change -> left -> state != 'V' && change -> up -> state != 'V' ) 
		return ;

	if ( change -> left -> state == 'V' ) {
		change = change -> left ;
		Print_the_maze3( change ) ;
	}
	
	if ( change -> up -> state == 'V' ) {
		change = change -> up ;
		Print_the_maze4( change ) ;
	}
	
	if ( change -> right -> state == 'V' ) {
		change = change -> right ;
		Print_the_maze1( change ) ;
	}
		
	if ( change -> down -> state == 'V' ) {
		change = change -> down ;
		Print_the_maze2( change ) ;
	}
}

void Print_the_maze2( Block * change ) {
	change -> state = 'R' ;
	
	if ( change -> right -> state != 'V' && change -> down -> state != 'V' && change -> left -> state != 'V' && change -> up -> state != 'V' ) 
		return ;
	
	if ( change -> down -> state == 'V' ) {
		change = change -> down ;
		Print_the_maze2( change ) ;
	}
	
	if ( change -> left -> state == 'V' ) {
		change = change -> left ;
		Print_the_maze3( change ) ;
	}
	
	if ( change -> up -> state == 'V' ) {
		change = change -> up ;
		Print_the_maze4( change ) ;
	}
	
	if ( change -> right -> state == 'V' ) {
		change = change -> right ;
		Print_the_maze1( change ) ;
	}	
}

void Print_the_maze1( Block * change ) { // 把V改成R，寫最短路徑 
	change -> state = 'R' ;
	
	if ( change -> right -> state != 'V' && change -> down -> state != 'V' && change -> left -> state != 'V' && change -> up -> state != 'V' ) 
		return ;
	
	if ( change -> right -> state == 'V' ) {
		change = change -> right ;
		Print_the_maze1( change ) ;
	}
	
	if ( change -> down -> state == 'V' ) {
		change = change -> down ;
		Print_the_maze2( change ) ;
	}
	
	if ( change -> left -> state == 'V' ) {
		change = change -> left ;
		Print_the_maze3( change ) ;
	}
	
	if ( change -> up -> state == 'V' ) {
		change = change -> up ;
		Print_the_maze4( change ) ;
	}	
}

void Print_maze() {
	Block * print = start ;
	Block * change = start ; // V改成R 
	Block * under = start ;
	Print_the_maze1( change ) ;
	for ( int k = 0 ; k < length ; k += 1 ) { //印整張迷宮 
		for ( int l = 0 ; l < width ; l += 1 ) {
			cout << change -> state ;
			change = change -> right ;
		}
		
		
		under = under -> down ;
		change = under ;
	}
}

int main() {
	char cmd ;
	
	while ( true ) {
		cout << "*** Path Finding ***" << endl ;
		cout << "* 0. Quit          *" << endl ;
		cout << "* 1. One goal      *" << endl ;
		cout << "* 2. More goals    *" << endl ;
		cout << "********************" << endl ;
		cout << "Input a command(0, 1, 2): "  ;
		cin >> cmd ;
		
		bool ifsopen = true ; //有沒有開成功檔案 
		goal = false ;  
		
		if ( cmd == '0' )
			break ;
		else if ( cmd == '1' ) {
			ifsopen = Input_file() ;
			if ( ifsopen ) {
				Maze_Establishment() ;
				Run_the_maze1(  ) ;
				if ( !goal ) {
					cout << "No goal! " << endl ;
				}
				else {
					Print_maze() ;
				} 
				
				delete( start ) ;
				start = new Block() ;
				run = start ;
			}
		}
		else if ( cmd == '2') {
			ifsopen = Input_file() ;
			if ( ifsopen ) {
				cout << "Number of G(goals): " ;
				cin >> numOfGoal ;
				Maze_Establishment() ;
				Many_goal1( numOfGoal ) ;
				Print_maze() ;
				delete( start );
				start = new Block() ;
				run = start ;
			}
		}
		else {
			cout << "Command does not exist!" << endl ;
		}
	}

	ifs.close() ;
	return 0;
}



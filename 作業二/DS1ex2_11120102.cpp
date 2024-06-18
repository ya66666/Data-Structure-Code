// 11120102 莊凱任 
#include <iostream>
#include <string>

using namespace std;

// 數字->+-*/)
// +-*/(->數字, )->)+-*/ 

class list {
	
public :
	char value  ;
	list * next ;
};

class Op {
	
public :
	char value ;
	Op * next ;
	Op * back ;
};

class Num {

public :
	int value ;
	Num * next ;
	Num * back ;
};

Op * infix = NULL ;

bool Illegal_char( list * illegal_char ) { // 檢查字元是否違法，是就TRUE，不是就FALSE 
	list * test = illegal_char ;
	
	if ( test -> value == '(' || test -> value == ')' || test -> value == '+' || test -> value == '-' 
		|| test -> value == '*' || test -> value == '/' || test -> value == ' ' ) {
			
		return false ;
	}
	
	else if ( test -> value < 58 && test -> value > 47 ) {
		return false ;
	}
	
	else {
		cout << "Error 1: " ;
		cout << test -> value ;
		cout << " is not a legitimate character."  ;
		return true ;
	}
}

bool IfError1( list * head ) { // 48~57, ()+-*/, 檢查是否有不合法字元 
	list * illegal_char = head ;
	
	while ( illegal_char != NULL ) {
		if ( Illegal_char( illegal_char ) ) { //違法就TRUE，直到NULL 
			return true ;
		}
		
		illegal_char = illegal_char -> next ;
	}
	
	return false ;
}

bool IfError2( list * head ) { //檢查左右括號 
	int left_parenthese ;
	int right_parenthese ;
	list * check_parentheses = head ;
	
	while ( check_parentheses != NULL ) {
		if ( check_parentheses -> value == '(' ) {
			left_parenthese += 1;
		}
		
		if ( check_parentheses -> value == ')' ) {
			right_parenthese += 1;
		}
		
		check_parentheses = check_parentheses -> next ;
	}
	
	if ( left_parenthese == right_parenthese ) {
		return false ;
	}
	else if ( left_parenthese > right_parenthese ) {
		cout << "Error 2: there is one extra open parenthesis." ;
		return true ;
	}
	else if ( left_parenthese < right_parenthese ) {
		cout << "Error 2: there is one extra close parenthesis." ;
		return true ;
	}
	
}

bool Extra_Operator( list * check ) {  //Error3 多一個符號 
	if ( check -> value == '+' || check -> value == '-' || check -> value == '*' || check -> value == '/' ) {
		check = check -> next ;
		
		if ( check == NULL ) {
			cout << "Error 3: there is one extra operator."  ;
			return true ;
		}
		
		if ( check -> value == '+' || check -> value == '-' || check -> value == '*' || check -> value == '/'  ) {
			cout << "Error 3: there is one extra operator." ;
			return true ;	
		}
	}
	
	return false ;
}

bool Not_Infix( list * check ) { // 不是中序式 
	if ( check -> value == '(' ) {
		if ( check -> next != NULL && ( check -> next -> value == '+' || check -> next -> value == '-' 
			|| check -> next -> value == '*' || check -> next -> value == '/' || check -> next -> value == ')' ) ) {
			
			cout << "Error 3: it is not infix in the parentheses." ;
			return true ;
		}
	}
		
	if ( check -> value == '+' || check -> value == '-' || check -> value == '*' || check -> value == '/' ) {
		if ( check -> next != NULL && check -> next -> value == ')' ) {
			cout << "Error 3: it is not infix in the parentheses." ;
			return true ;
		}
	}
		
	return false ;
}

bool Extra_Operand( list * check ) { // 括號前後沒有運算符就寫數字 
	if ( check -> value < 58 && check -> value > 47 ) {
		if ( check -> next != NULL && check -> next -> value == '(' ) {
			cout << "Error 3: there is one extra operand." << endl ;
			return true ;
		}
	}
	
	if ( check -> value == ')' ) {
		if ( check -> next != NULL && ( check -> next -> value < 58 && check -> value > 47 ) ) {
			cout << "Error 3: there is one extra operand." << endl ;
			return true ;			
		}
	}
	
	return false ;
}
//Error 3: there is one extra operator.  ++
//Error 3: it is not infix in the parentheses. ( -> -=*/)
//Error 3: there is one extra operand. 數字 -> ( , ) -> 數字 , )(

// 數字 -> +-*/)
// +-*/( -> 數字, ) -> )+-*/ 

bool IfError3( list * head ) { // Error3的順序 
	list * check = head ;
	
	while ( check != NULL ) {
		if ( Extra_Operator( check ) ) return true ;
		check = check -> next ;
	}
	
	check = head ;
	while ( check != NULL ) {
		if (  Not_Infix( check ) ) return true ;
		check = check -> next ;
	}
	
	check = head ;
	while ( check != NULL ) {
		if (  Extra_Operand( check ) ) return true ;
		check = check -> next ;
	}
	
	return false ;
}

void Put( Op * &put_operator, char val ) { // 放入 put_operator  
	Op * temp = put_operator ;
	
	if ( put_operator == NULL ) { // 頭 
		put_operator = new Op ;
		put_operator -> value = val ;
		put_operator -> next = NULL ;
		put_operator -> back = NULL ;
	}
	
	else {
		while ( temp -> next != NULL ) { // 中間 
			temp = temp -> next ;
		}
		
		temp -> next = new Op ;
		temp -> next -> back = temp ;
		temp = temp -> next ;
		temp -> value = val ;	
		temp -> next = NULL ;
	}
}

void Pop_Out( Op * &put_operator ) { //拿出 put_operator 的最後一個運算符 
	Op * pre ;
	Op * now = put_operator ;
	Op * temp ;
	
	if ( now -> next != NULL ) { // 中間還沒到頭 
		while ( now -> next != NULL ) {  
			now = now -> next ;
		}
	
		pre = now -> back ;
		cout << ", " ;
		cout << now -> value ;
		Put( infix, now -> value ) ;
		pre -> next = NULL ;
		now -> back = NULL ;
		delete( now ) ;
	} 
	else {
		if ( put_operator -> value != '(' ) {  //最後一個拿出來的 
			cout << put_operator -> value ;
			Put( infix, now -> value ) ;
			cout << ", " ;
		}
		
		delete( put_operator ) ; //只會刪掉最後一個'(' ,在最前端的'(' 
		put_operator = NULL ;
	}
	
}

char Head_to_Tail( Op * put_operator ) { // 從頭找最後一個 
	Op * cur = put_operator ;
	
	if ( cur != NULL ) {
		while ( cur -> next != NULL ) {
			cur = cur -> next ; 
		}
	}
	
	if ( cur == NULL ) return '0' ;
	else return cur -> value ;
}

bool Compare( Op * put_operator, char value ) { // 比較, larger -> true, smaller -> false
	char cmp = '0' ;
	
	cmp = Head_to_Tail( put_operator ) ;
	if ( cmp == '0' ) {
		return true ;
	}
	
	if ( value == '+' || value == '-' ) {
		if ( cmp == '(' ) {
			return true ;
		}
		
		else return false ;
	}
	else if ( value == '*' || value == '/' ) {
		if ( cmp == '(' || cmp == '+' || cmp == '-' ) {
			return true ;
		}
		
		else return false ;
	}
}

void Postfix( list * head ) {
	Op * put_operator = NULL ; // 存放運算符 
	list * run = head ;
	Op * temp ;
	
	while ( run != NULL ) {
		if ( run -> value < 58 && run -> value > 47  ) { //數字直接輸出 
			Put( infix, run -> value ) ;
			cout << run -> value ;
		}
		
		if ( run -> value == '(' ) { //直接放入 put_operator裡面 
			Put( put_operator, run -> value ) ;
		}
			
		if ( run -> value == '+' || run -> value == '-' || run -> value == '*' || run -> value == '/' ) {
			cout << ", " ;
			if ( Compare( put_operator, run -> value ) ) {
				Put( put_operator, run -> value ) ;
			}
			
			else {
				while ( ! Compare( put_operator, run -> value ) ) { // 運算符的先後順序 
					Pop_Out( put_operator ) ;  
				}
				
				Put( put_operator, run -> value ) ;
			}
			
		}
		
		if ( run -> value == ')' ) {
			while ( Head_to_Tail( put_operator ) != '(' ) {
				Pop_Out( put_operator ) ;
			}
			
			// 刪中間或最前面的'(',不用pop是因為沒有要拿出來只要刪掉而且pop沒辦法處理中間的'(' 
			temp = put_operator ; 
			while ( temp != NULL && temp -> next != NULL ) {
				temp = temp -> next ; 
			}
			
			if ( put_operator != temp ) {
				temp -> back -> next = NULL ;
				temp -> back = NULL ;
				delete( temp ) ;
				temp = NULL ;
			}
			else {
				delete( put_operator ) ;
				put_operator = NULL ;
			}
		}
		
		run = run -> next ;
	}
	
	temp = put_operator ;
	if ( put_operator != NULL ) { //把剩下運算符拿出來 
		while ( temp != NULL ) {
			if ( temp -> next == NULL ) //到最後一個 
				break ; 
			
			temp = temp -> next ;
		}
		
		while ( temp != NULL ) { //從後面往前拿出 
			cout << ", " ;
			cout << temp -> value ;
			Put( infix, temp -> value ) ;
			temp = temp -> back ;
		}
	}
	
	delete( put_operator ) ;
	put_operator = NULL ;
}

void Atoi( int &sum, char val ) {
	sum = sum + val - 48 ;
	sum *= 10 ;
}

void Put_Num( int sum, Num * &num ) {
	Num * temp = num ;
	
	if ( num == NULL ) { // 頭 
		num = new Num ;
		num -> value = sum ;
		//cout << num -> value << endl ;
		num -> next = NULL ;
		num -> back = NULL ;
	}
	
	else {
		while ( temp -> next != NULL ) { // 中間 
			temp = temp -> next ;
		}
		
		temp -> next = new Num ;
		temp -> next -> back = temp ;
		temp = temp -> next ;
		temp -> value = sum ;	
		//cout << temp -> value << endl ;
		temp -> next = NULL ;
	}
	
}

// (29+103)*13/3
int Take_Num( Num * &num ) { // 拿第一個數字 
	Num * assist = num ;
	int a = 0 ;
	if ( num != NULL ) {
		a = num -> value ;
		num = num -> next ;
		assist -> next = NULL ;
		if ( num != NULL )
			num -> back = NULL ;
		
		delete( assist ) ;
		assist = num ;
	}
	
	//cout << a << endl ;
	return a ;
}

int Pop_num( Num * &second_num ) {
	Num * temp = second_num ;
	int i = 0 ;
	if ( temp -> next == NULL ) {
		i = temp -> value ;
		delete( second_num ) ;
		second_num = NULL ;
	}
	
	else {
		while ( temp -> next != NULL ) 
			temp = temp -> next ;
		
		i = temp -> value ;
		temp -> back -> next = NULL ;
		temp -> back = NULL ;
		delete( temp ) ;
		temp = NULL ;
	}
	
	//cout << i << endl ;
	return i ;
}

//(29+103)*13/3
void Count( Num * &num ) {
	Num * second_num = NULL ; // 第二個輔助運算的 
	Num * test = NULL ;
	int sum = 0, number_1 = 0, number_2 = 0 ;
	bool zero = false ;
	if ( num -> next == NULL )
		cout << num -> value ;
		
	else {
		number_1 = Take_Num( num ) ;
		//cout << number_1 << endl ;
		
		while ( num != NULL ) {
			
			// cout << number_1 << endl ;
			if ( number_1 != -1 && number_1 != -2 && number_1 != -3 && number_1 != -4 ) {
				//cout << number_1 << endl ;
				Put_Num( number_1, second_num ) ;
			}
			
			else {
				sum = Pop_num( second_num ) ;
				number_2 = Pop_num( second_num ) ;
				
				if ( number_1 == -1 ) { // +
					sum = sum + number_2 ;
					//cout << sum << endl ;
				}
				
				else if ( number_1 == -2 ) { // -
					sum = number_2 - sum ;
					//cout << sum << endl ;
				}
				
				else if ( number_1 == -3 ) { // *
					sum = sum * number_2 ;
					//cout << sum << endl ;
				}
				
				else if ( number_1 == -4 ) { // /
					if ( sum == 0 ) {
						cout << "### Error: Divided by ZERO! ###" << endl ;
						cout << "### It cannot be successfully evaluated! ###" << endl ;
						zero = true ;
					}
					
					else 
						sum = number_2 / sum ;
					//cout << sum << endl ;
				}
				
				if ( !zero )
					Put_Num( sum, second_num ) ;
			}
			if ( !zero )
				number_1 = Take_Num( num ) ;
				
			else break ;
		}
		
		if ( !zero )  {
			sum = Pop_num( second_num ) ;
			number_2 = Pop_num( second_num ) ;
			if ( number_1 == -1 )  // +
				sum = sum + number_2 ;
				
			else if ( number_1 == -2 )  // -
				sum = number_2 - sum ;
				
			else if ( number_1 == -3 )  // *
				sum = sum * number_2 ;
				
			else if ( number_1 == -4 )  // /
				sum = number_2 / sum ;
			
			Put_Num( sum, second_num ) ;
			number_1 = Pop_num( second_num ) ;
			cout << number_1 ;
		}
		
	}
	
	
	delete( second_num ) ;
	second_num = NULL ;
}

// (29+103)*13/3
void Answer( int digit[20] ) {
	Op * cur = infix ; // 現在位置 
	Num * num = NULL ; // infix num
	Num * test = NULL ; //測試用 
	int i = 0, j = 0, sum = 0 ;
	
	while ( cur != NULL ) {
		if ( cur != NULL && cur -> value < 58 && cur -> value > 47 ) {
			while ( i < digit[j] ) {
				Atoi( sum, cur -> value ) ;	
				cur = cur -> next ;
				i += 1 ;
			}
			
			sum /= 10 ;
			Put_Num( sum, num ) ;
			j += 1, i = 0, sum = 0 ;
		}

		else if ( cur != NULL && ( cur -> value == '+' || cur -> value == '-' || cur -> value == '/' || cur -> value == '*' ) ) {
			if ( cur -> value == '+' )
				sum = -1 ;
			
			else if ( cur -> value == '-' )
				sum = -2 ;

			else if ( cur -> value == '*' ) 
				sum = -3 ;

			else if ( cur -> value == '/' ) 
				sum = -4 ;

			Put_Num( sum, num ) ;
			cur = cur -> next ;
			sum = 0 ;
		}
	}
	
	
	Count( num ) ;
	delete( num ) ;
	delete( infix ) ;
	num = NULL ;
	infix = NULL ;
	
//	test = num ;
//	while ( test != NULL ) {
//		cout << test -> value << endl ;
//		test = test -> next ; 
//	}
	
}

// (29+103)*13/3
void Digit( list * head, int ( &digit )[20] ) {
	list * cur = head ;
	int i = 0, j = 0 ;
	
	while ( i < 20 ) {
		digit[i] = -99 ;
		i += 1 ;
	}
	
	i = 0 ;
	while ( cur != NULL ) {
		while ( cur != NULL && cur -> value > 57 || cur -> value < 48 ) 
			cur = cur -> next ;
		
		while ( cur != NULL && cur -> value < 58 && cur -> value > 47 ) {
			i += 1 ;
			cur = cur -> next ;
		}
		
		digit[j] = i ;
		j += 1, i = 0 ;
	}
	
	i = 0 ;
	while ( i < 20 ) {
		if ( digit[i] == 0 ) {
			digit[i] = -99 ;
		}
		
		i += 1 ;
	}
	
//	i = 0 ;
//	while ( i < 20 ) {
//		cout << digit[i] << endl ;
//		i+=1;
//	}
}

void IfError( list * head, int ( &digit )[20] ) { // 有沒有Error 
	if ( IfError1( head ) ) ;
	else if ( IfError2( head ) ) ;
	else if ( IfError3( head ) ) ;
	else {
		cout << "It is a legitimate infix expression." << endl ; 
		cout << "Postfix expression: " ;
		Digit( head, digit ) ;
		Postfix( head ) ;
		cout << endl ;
		cout << "Answer: " ;
		Answer( digit ) ;
	}
}

void Delete_Space( list * &head ) { //刪除輸入的空白 
	list * pre ;
	list * now = head ;
	
	while ( now -> next != NULL ) {
		if ( head -> value == ' ' ) {
			head = now -> next ;
			now -> next = NULL ;
			delete( now ) ;
			now = head ;
		}
		else {
			pre = now ;
			now = now -> next ;
			if ( now -> value == ' ' ) {
				pre -> next = now -> next ;
				now -> next = NULL ;
				delete( now ) ;
				now = pre ; 
			}
		}
	}
	
	if ( now -> value == ' ' ) {
		pre -> next = NULL ;
		delete( now ) ;
	}
	
}

void Input( list * &head ) {
	list * run ;
	int size = 0 ;
	string formula ;
	
	// cin.ignore() ; //讀掉'/n' 
	getline( cin, formula ) ;
	size = formula.size() ;
	for ( int i = 0 ; i < size ; i++ ) {
		if ( head == NULL ) {
			head = new list ;
			run = head ;
			run -> value = formula.at( i ) ;
		}

		else {
			run -> next = new list ;
			run = run -> next ;
			run -> value = formula.at( i ) ;
		}
	}
	
	run -> next = NULL ; 
	Delete_Space( head ) ;
	
//	run = head ;
//	while ( run != NULL ) {
//		cout << run -> value << endl ;
//		run = run -> next ; 
//	}
}

int main() {
	string cmd ;
	list * head = NULL ;
	int i = 0 ;
	int digit[20] ;
	
	do {
		cout << "* Arithmetic Expression Evaluator *" << endl ;
		cout << "* 0. QUIT                         *" << endl ;
		cout << "* 1. Infix2postfix Transformation *" << endl ;
		cout << "***********************************" << endl ;
		cout << "Input a choice(0, 1): " ;
		getline( cin, cmd ) ;
		// cin >> cmd ;
		
		while ( cmd.at( i ) == ' ' )
			i ++ ;
		
		if ( cmd.size() - i  == 1 && cmd.at( i ) == '0'  )  return 0 ;
		
		else if ( cmd.size() - i > 1 && cmd.at( i ) == '0' && ( cmd.at( i + 1 ) > 57 || cmd.at( i + 1 ) < 48 ) ) return 0 ;
		
		else if ( cmd.size() - i == 1 && cmd.at( i ) == '1' ) {
			cout << endl ;
			cout << "Input an infix expression: " ;
			Input( head ) ;
			IfError( head , digit ) ;
			cout << endl << endl ;
		}
		
		else if ( cmd.size() - i > 1 && cmd.at( i ) == '1' && ( cmd.at( i + 1 ) > 57 || cmd.at( i + 1 ) < 48 ) ) {
			cout << endl ;
			cout << "Input an infix expression: " ;
			Input( head ) ;
			IfError( head, digit ) ;
			cout << endl << endl ;
		}
			
		else cout << "Command does not exist!" << endl << endl ;
		
		delete( head ) ;
		head = NULL ;
		
	} while ( cmd != "0" ) ;
	
	return 0 ;
}

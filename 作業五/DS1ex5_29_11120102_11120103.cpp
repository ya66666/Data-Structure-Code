// 11120102莊凱任、11120103游舒涵 
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


struct VectorNode{
	int pNum;
	int number;
	string name;
	string type1;
	int hp;
	int attack;
	int defense;
};



struct TreeNode{
	int hp;
	VectorNode * dataAddress;
	TreeNode * left;
	TreeNode * right;
	TreeNode * next;
};




class BinarySearchTree{
	private:
		TreeNode * head = NULL;
		
	public:
		TreeNode * leftmost = NULL, * rightmost = NULL;
		int height = 0;
		
		TreeNode * GetHead(){
			return head;
		} // GetHead()
		
		
		void Insert( TreeNode * insert, TreeNode * &root, int h ){
			if( root == NULL ){
				root = insert;
					
				if( h == 1 )
					head = leftmost = rightmost = insert;	
				else if( insert->hp < leftmost->hp )
					leftmost = insert;
				else if( insert->hp > rightmost->hp )
					rightmost = insert;									 
						
				if( h > height )
					height = h;	
			} // if
			
			else if( insert->hp < root->hp )
				Insert(insert,root->left,++h);
			else if( insert->hp > root->hp )
				Insert(insert,root->right,++h);
			
			else{
				TreeNode * curr = root;
				while( curr->next != NULL )
					curr = curr->next;
				curr->next = insert;
			}
			
			
		} // Insert() 
		
		
		
		void DeleteAll( TreeNode * root ){
			if( root == NULL )
				return;
			DeleteAll(root->left);
			DeleteAll(root->right);
			delete root;
		} // DeleteAll()
		
				
		
		void Initialize(){
			DeleteAll(head);
			head = leftmost = rightmost = NULL;
			height = 0;
		}
}; // class BinarySearchTree



class Heap{
	private:
		vector<VectorNode> heap ;
		
	public:
		vector<VectorNode> GetVector(){
			return heap;
		}
		
		
		void Push( VectorNode push ){
			heap.push_back(push);			
		}
		
		void Set_vector( vector<VectorNode> heaap ) {
			heap.swap( heaap ) ;
		}
		
		void PrintCertain( TreeNode * c ){			
			cout << "[" << c->dataAddress->pNum << "]" << "\t" << c->dataAddress->number << "\t" << c->dataAddress->name;
			PrintSpace(32-c->dataAddress->name.length());
			cout << c->dataAddress->type1;
			PrintSpace(16-c->dataAddress->type1.length());
			cout << c->dataAddress->hp << "\t" << c->dataAddress->attack << "\t" << c->dataAddress->defense << endl;
			
			if( c->next != NULL )
				PrintCertain(c->next);
		} // PrintCertain()
		
		void PrintCertain( int num ){
 			cout << "[" << num << "]" << "\t" << heap[num].number << "\t" << heap[num].name;
 			PrintSpace(32-heap[num].name.length());
 			cout << heap[num].type1;
 			PrintSpace(16-heap[num].type1.length()); // test
 			cout << heap[num].hp << "\t" << heap[num].attack << "\t" << heap[num].defense << endl;
		} // PrintCertain()
		
		void PrintSpace( int i ){
			for( int j = 0 ; j < i ; j++ )
				cout << " ";
		}
		
		void PrintAll( int j ){
			for( int i = 0 ; i < heap.size() ; i++ ){
				cout << "[";
				if( i + j < 10 )
					cout << " ";
				cout << i + j << "]" << "\t" << heap.at(i).number << "\t" << heap.at(i).name;
				PrintSpace(32-heap.at(i).name.length());
				cout << heap.at(i).type1;
				PrintSpace(16-heap.at(i).type1.length());
				cout << heap.at(i).hp << "\t" << heap.at(i).attack << "\t" << heap.at(i).defense << endl; 
			}
		} // PrintAll()
		
		
		void Initialize(){
			if( ! heap.empty() )
				heap.clear();
		}
}; // class Heap




class Procedure{
	public:
		ifstream fin;
		int command;
		BinarySearchTree bst;
		Heap heap;
		
	
	
	
	
		bool IsNumber( string str ){
			for( int i = 0 ; i < str.length() ; i++ ){
				if( ! isdigit(str.at(i)) )
					return false;
			}
			return true;
		} // IsNumber()
		
		
	
	
		int GetCommand(){
			string get;
			while( true ){
				cout << endl << "***** Pokemon Tree and Heap *****" << endl;
				cout << "* 0. QUIT                       *" << endl;
				cout << "* 1. Read a file to build BST   *" << endl;
				cout << "* 2. Transform it into Max Heap *" << endl;
				cout << "*********************************" << endl;
				cout << "Input a choice(0, 1, 2): ";
				cin >> get;
			
				if( IsNumber(get) && ( stoi(get) == 0 || stoi(get) == 1 || stoi(get) == 2 ) ){
					command = stoi(get);
					return command;
				}
				cout << "Command does not exist!" << endl;
			} // while	
		} // GetCommand()
		
		
		
		
		
		string GetInput(){
			char ch;
			string get;	
			for( fin.get(ch) ; ch != '\t' ; fin.get(ch) )
				get += ch;
			return get;
		} // GetInput
		
		
		
		
		bool ReadFile(){
			bst.Initialize();
			heap.Initialize();
			
			string get;
			while( true ){
				cout << endl << "Input a file number [0: quit]: ";
				cin >> get;
				
				if( get == "0" )
					return false;
					
				fin.open("input"+get+".txt");
				if( fin.is_open() ){
					string garbage;
					char ch;
										
					getline(fin,garbage);
					for( int i = 1; ! fin.eof() ; i++ ){
						//#	Name	Type 1	Type 2	Total	HP	Attack	Defense	Sp. Atk	Sp. Def	Speed	Generation	Legendary
						fin >> ch;
						if( fin.fail() )
							break;
						VectorNode * push = new VectorNode;	
						push->pNum = i;
						push->number = stoi(ch+GetInput());
						push->name = GetInput();
						push->type1 = GetInput();
						GetInput(); // type2
						GetInput(); // total
						push->hp = stoi(GetInput());
						push->attack = stoi(GetInput());
						push->defense = stoi(GetInput()); 
						getline(fin,garbage);
						heap.Push(*push);
					}
					fin.close();
					return true;
				} // if
					
				cout << endl << "### input" << get << ".txt does not exist! ###" << endl;
			} // while
		} // ReadFile
		
		
		
		
		void ToTree(){
			vector<VectorNode> h = heap.GetVector();
			for( int i = 0 ; i < h.size() ; i++ ){
				TreeNode * insert = new TreeNode;
				insert->hp = h.at(i).hp;                                                    
				insert->dataAddress = &h.at(i);
				insert->left = insert->right = insert->next = NULL;
				TreeNode * bstHead = bst.GetHead();
				bst.Insert(insert,bstHead,1);
			}
			
			Print();
		} // ToTree
	
		
		void CheckHeap( vector<VectorNode> &heaap, int &cur, int size ) {
			if ( cur * 2 + 1 > size ) { //沒有子樹 
//				cout << "00" << endl ;
				return ;
			}
			else {
				if ( cur * 2 + 2 <= size ) { //兩個子樹 
					if ( heaap[cur * 2 + 1].hp >= heaap[cur * 2 + 2].hp ) { //左邊大 
						if ( heaap[cur].hp < heaap[cur * 2 + 1].hp ){
							swap( heaap[cur], heaap[cur * 2 + 1] ) ;
//							cout << "true" << endl ;
						}
							
							
						cur = cur * 2 + 1 ;
					}	
					else if ( heaap[cur * 2 + 1].hp < heaap[cur * 2 + 2].hp ) { //右邊大 
						if ( heaap[cur].hp < heaap[cur * 2 + 2].hp ) {
							swap( heaap[cur], heaap[cur * 2 + 2] ) ;
						}
						
							
						cur = cur * 2 + 2 ;
					}
				} // if
				
				else if ( cur * 2 + 2 > size && cur * 2 + 1 <= size ) { //一個子樹 
					if ( heaap[cur].hp < heaap[cur * 2 + 1].hp ) {
						swap( heaap[cur], heaap[cur * 2 + 1] ) ;
					}
						
						
					cur = cur * 2 + 1 ;
				} // else if
				
				CheckHeap( heaap, cur, size ) ;
			} // else
			
		} // CheakHeap
		
		void LeftTree( vector<VectorNode> heaap, int &left_tree, int &height ) {
			
			for ( int i = 0 ; i < heaap.size() ; i = i * 2 + 1 ) {
				left_tree = i ;
				height += 1 ;
			}
		}
		
		void ToHeap() {
			vector<VectorNode> heaap = heap.GetVector();
			int size = heaap.size(), right_tree = 0, left_tree = 0, height = 0 ;
			for ( int cur = ( size - 2 ) / 2 ; cur >= 0 ; cur -= 1 ) {
				int temp_cur = cur ;
				CheckHeap( heaap, temp_cur, size - 1 ) ;
			} // for
			
			right_tree = 0, left_tree = 0, height = 0 ;
			heap.Set_vector( heaap ) ;
			heap.PrintAll(0) ;
			LeftTree( heaap, left_tree, height ) ; 
			cout << "HP heap height = " << height << endl ;
			right_tree = heaap.size() - 1 ;
			cout << "Leftmost node:" << endl ;
			cout << "        #       Name                            Type 1          HP      Attack  Defense" << endl ;
			heap.PrintCertain( left_tree ) ;
			cout << "Bottom:" << endl ;
			cout << "        #       Name                            Type 1          HP      Attack  Defense" << endl ;
			heap.PrintCertain( right_tree ) ;
			size = 0 ;
		} // ToHeap
		
		
		void Print(){
			cout << "\t#" << "\tName" << "\t\t\t\tType 1" << "\t\tHP" << "\tAttack" << "\tDefense" << endl;
			heap.PrintAll(1);
			
			cout << "HP tree height = " << bst.height << endl;
			
			cout << "Leftmost node:" << endl;
			heap.PrintCertain(bst.leftmost);
			
			cout << "Rightmost node:" << endl;
			heap.PrintCertain(bst.rightmost);			
		} // Print
		
	
}; // class Procedure



 
int main(){
	Procedure p;	
	while( p.GetCommand() ){
		if( p.command == 1 ){
			if( p.ReadFile() )
				p.ToTree();
		}
		
		else if( p.command == 2 ) 
			p.ToHeap() ;
			
	} // while
	
} // main()

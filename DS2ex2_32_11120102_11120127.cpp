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

struct DataFake{	
	char SN[100] ; //校名 
	char SB[100] ; // 科系 
	char DAN[100] ; //日夜 
	char GA[100] ; //等級 
	char ST[100] ; //學生數
	char TE[100] ; //教師數
	int GR ; //上學年度畢業生數
	int student ;
	int num ; //編號 
} ; 

struct Data{
	int num;
	int student;
};

struct Node {	
    vector<Data> data;
    Node* left;
    Node* middle;
    Node* right;
    Node* parent; 

    Node( const Data input ) {
	  data.push_back( input ) ; 
	  left = NULL;
	  middle = NULL;
	  right = NULL;
	}
}; 

struct TreeAVL {
	DataFake info ;
	TreeAVL * left ;
	TreeAVL * right ;
	TreeAVL * side ;
};

class Tree23{
	
private:
  vector<Data> ALL ; //只存學生數跟序號 
  vector<DataFake> AllData ; //存完整內容 
  Node * ATree = NULL; 
  
  bool load(string fileName) {
	bool success = false;
     
    if ( fileName == "0" ){
        cout << endl;
        return false;
    }
    fileName = "input"+ fileName +".txt";
    FILE *infile = NULL;
    infile = fopen(fileName.c_str(), "r");

    if (infile == NULL){
        cout << endl << "### " << fileName << " does not exist! ###" << endl;
        cout << endl << "Input a file number ([0] Quit): " ;
          cin >> fileName;
          success = load(fileName);
    } 
    else{
      string line;
      char trash[500];
      int i = 0;
    
      while(i < 3){ //除掉前三行拉基 
        fgets(trash, sizeof(trash), infile);
        i++;
      }
      
      DataFake temp;
      Data useful;
      char En[100]; //日 
      char St[100]; // 學士 
      string GRA; // 畢業生數 
      string FXK;
      int number = 1;
      while( fscanf(infile,"%s %s %s %s %s", trash, &temp.SN[0], trash, &temp.SB[0], &temp.DAN[0]) != EOF ){
      	if( isupper(temp.DAN[0]) ) { //D日沒搞事 
      	  fscanf(infile,"%s %s", &En[0], &temp.GA[0] );
      	  FXK.erase();
		  FXK = temp.GA;
      	  
      	  if( isupper(temp.GA[0]) ){ //GA沒漏  
      	    fscanf(infile,"%s %s %s %d %s %s %s %s",  &St[0], &temp.ST[0], &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash);
            strcat(temp.GA, " ");
            strcat(temp.GA, St);
		  }
		  else if ( (temp.GA[0] == '5' || temp.GA[0] == '2' ) && FXK.length() == 1 ){
		  	fscanf(infile,"%s %s %s %d %s %s %s %s",  &St[0], &temp.ST[0], &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash);
            strcat(temp.GA, " ");
            strcat(temp.GA, St);
		  }
		  else{ // 搞事 
		    fscanf(infile, "%s %d %s %s %s %s", &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash );
		    strcpy(temp.ST, temp.GA);
		    
		    memset(temp.GA, 0, sizeof(temp.GA));

		  }
		  
		  strcat(temp.DAN, " ");
          strcat(temp.DAN, En);
		} // if
      	else { //D日搞事 
      	  fscanf(infile, "%s %d %s %s %s %s", &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash );
		  strcpy(temp.ST, temp.DAN);
		  
		  memset(temp.DAN, 0, sizeof(temp.DAN));
		  memset(temp.GA, 0, sizeof(temp.GA));
		}
      	
      	char tea[100];
      	if (temp.ST[0] == '"'){
      	  int i = 0;
      	  int j = 0;
      	  while ( i < sizeof(temp.ST) - 1 ){
      	    if (temp.ST[i] != '"' && temp.ST[i] != ','){
      	      tea[j] = temp.ST[i];
      	      j = j + 1;
			}
			
			i = i + 1;
		  }
		  
		  strcpy(temp.ST, tea);
		}
		
		useful.num = number;
		temp.num = number;
		
		useful.student = atoi(temp.ST);
        ALL.push_back(useful);
        
        temp.student = atoi(temp.ST);
		AllData.push_back(temp);
		
        number = number + 1;
      }
    
      success = true;
      fclose(infile);
    }

    return success;
  } // Read 
  
  void show(){
	int i = 0 ;	
	int j = 1;
	cout << ALL.size() << endl;
	while( i < ALL.size() ){
		cout << "[" << ALL[i].num << "]" << "\t";
		
		cout << ALL[i].student << endl;
		i = i + 1 ;
		j = j + 1 ;
	}
  }
  
public:
  bool Load(string fileNum){
  	return load(fileNum);
  }
  
  void Show(){
  	show();
  }
  
  bool isEmpty(){
  	if (ALL.empty()) return true;
  	else return false;
  }
  
  vector<DataFake> GetData() {
  	return AllData;
  }
  
  void Clear(){
  	ALL.clear();
  	AllData.clear() ;
  }
  
};

class AVLTree{
	
private:
  vector<Data> ALL ; //只存學生數跟序號 
  vector<DataFake> AllData ; //存完整內容 
  
  void show(){
	int i = 0 ;	
	int j = 1;
	cout << AllData.size() << endl;
	while( i < AllData.size() ){
		cout << "[" << AllData[i].num << "]" << "\t";
		
		cout << AllData[i].student << endl;
		i = i + 1 ;
		j = j + 1 ;
	}
  }

public:
  TreeAVL * cur = NULL ;
  int now_num = 0, height = -99,right_height = 0, left_height = 0, number_of_nodes = 1 ;
  
  void recieve(vector<DataFake> Copy){
  	AllData = Copy;
  }	
  
  void Compare( int h ) {
  	if ( h > height ) {
  		height = h ;
	  }	
  }
  
  void Right_Left_Height( TreeAVL * node, int h ) { // tree height
    if ( node == NULL ) {
    	h -= 1 ;
		Compare( h ) ;
    	return ;
	}
    	
    Right_Left_Height( node -> left, h + 1 ) ;                                 
	Right_Left_Height( node -> right, h + 1 ) ;
  }
  
  void How_many_nodes( TreeAVL * head ) {
  	if ( head == NULL ) {
  		number_of_nodes -= 1 ;
  		return ;
	}
	
    number_of_nodes += 1 ;
    How_many_nodes( head -> left ) ;
    number_of_nodes += 1 ;
	How_many_nodes( head -> right ) ;
  }
  
  void Initial() {
	cur = new TreeAVL ;
  	cur -> info = AllData[now_num] ;
//  	cout << "Data student = " << cur -> info.student << endl ;
  	cur -> left = NULL ;
	cur -> right = NULL ;
	cur -> side = NULL ; 		
  	now_num += 1 ;
  }
  
  void Insert_Height( TreeAVL * cur ) {
  	int h = 1 ;
  	
  	Right_Left_Height( cur -> right, h ) ;
  	right_height = height ;
//  	cout << "right height = " << right_height << endl ;
  	height = -99 ;
  	
  	Right_Left_Height( cur -> left , h ) ;
  	left_height = height ;
//  	cout << "left height = " << left_height << endl ;
  	height = -99 ;
  }
  
  void LL_Rotation( TreeAVL * &first ) { 
  	TreeAVL * second = first -> left ;
  	first -> left = second -> right ;
  	second -> right = first ;
  	first = second ;
  }
  
  void RR_Rotation( TreeAVL * &first ) {
  	TreeAVL * second = first -> right ;
  	first -> right = second -> left ;
  	second -> left = first ;
  	first = second ;
  }
  
  void RL_Rotation( TreeAVL * &first ) {
  	TreeAVL * second = first -> right ;
  	TreeAVL * third = second -> left ;
  	second -> left = third -> right ;
  	first -> right = third -> left ;
	third -> left = first ;
	third -> right = second ;
	first = third ;
  }
  
  void LR_Rotation( TreeAVL * &first ) {
  	TreeAVL * second = first -> left ;
  	TreeAVL * third = second -> right ;
  	second -> right = third -> left ;
  	first -> left = third -> right ;
	third -> right = first ;
	third -> left = second ;
	first = third ;
  }
  
  void Rotation( TreeAVL * &head ) {
	if ( left_height - right_height >= 2 ) { // L up
		Insert_Height( head -> left ) ;
		if ( left_height > right_height ) { // L down
//			cout << "LL" << endl ;
			LL_Rotation( head ) ;
		}
		
		else if ( left_height < right_height ) { // R down
//			cout << "LR" << endl ;
			LR_Rotation( head ) ;
		}
	}
	else if ( left_height - right_height <= -2 ) { // R up
		Insert_Height( head -> right ) ;
		if ( left_height < right_height ) { // R down
//			cout << "RR" << endl ;
			RR_Rotation( head ) ;
		}
		
		else if ( left_height > right_height ) { // L down
//			cout << "RL" << endl ;
			RL_Rotation( head ) ;
		}
	} 
  }
  
  void Insert( TreeAVL * &head, TreeAVL * cur ) {
	int right_height = 1, left_height = 1 ;
	
	if ( head == NULL ) { //insert
		head = cur ;
//		cout << "NULL" << endl ;
		return ;
	}
	
	else if ( cur -> info.student == head -> info.student ) { //insert
//		cout << "side" << endl ;
		Insert( head -> side, cur ) ;
	}
		
	else if ( cur -> info.student > head -> info.student ) { //insert
//		cout << "right" << endl ;
		Insert( head -> right, cur ) ;
	}
	
	else if ( cur -> info.student < head -> info.student ) { //insert
//		cout << "left" << endl ;
		Insert( head -> left, cur ) ;
	}
	
	Insert_Height( head ) ;
	Rotation( head ) ;
  }
  
  void Delete( TreeAVL * root ) {
	if ( root != NULL ) {
        Delete( root -> left );
        Delete( root -> side );
        Delete( root -> right );
        delete root;
        root = NULL;
    }
  }
  
  void Remake() {
  	cur = NULL ;
	now_num = 0, height = 0,right_height = 0, left_height = 0, number_of_nodes = 1 ;
	
  }
  
  void Procedure() {
	TreeAVL * head = NULL ;
  	int tree_height = 1, j = 1 ;
  	for ( int i = 0 ; i < AllData.size() ; i += 1 ) {
  		Initial() ;
//  		cout << "num = " << now_num << endl ;
//		cout << "student = " << cur -> info.student << endl ;
  		Insert( head, cur ) ;
	}	
	
//--------------------------------------------------------------------------------------
	Right_Left_Height( head, tree_height ) ;
	tree_height = height ;
  	cout << "Tree height = " << tree_height << endl ;
  	height = -99 ;
  	
  	How_many_nodes( head ) ;
  	cout << "Number of nodes = " << number_of_nodes << endl ;
//--------------------------------------------------------------------------------------  	
  	for ( TreeAVL * temp = head ; temp != NULL ; temp = temp -> side, j += 1 ) {
  		cout << j << ": [" << temp -> info.num << "] " << temp -> info.SN << ", " << temp -> info.SB << ", " << temp -> info.DAN << ", " << temp -> info.GA << ", " << temp -> info.ST << ", " << temp -> info.GR << endl ;
	}
	
	Remake() ;
	Delete( head ) ;
	Clear() ;
  }
  
  bool isEmpty(){
  	if (ALL.empty()) return true;
  	else return false;
  }
  
  void Show(){
	show();
  }
  
  void Clear(){
  	ALL.clear();
  	AllData.clear() ;
  }
};

int main(){

  int command = 0;
  int i = 0;
  string fileNum ;
  bool again = false ;
  
  Tree23 Database1 ;
  AVLTree Database2; 
  cout << endl;
  
  do{
	  cout << "**** Balanced Search Trees ****" << endl;
      cout << "* 0. QUIT                     *" << endl;
   	  cout << "* 1. Build 23 tree            *" << endl;
      cout << "* 2. Build AVL tree           *" << endl;
      cout << "*******************************" << endl;
      cout << "Input a choice(0, 1, 2): " ;
    
      cin >> command;
      cin.ignore( Max_LEN, '\n' );
      
      if ( command == 0 ) break;
      else if( command >= 3 ) cout << endl << "Command does not exist!" << endl << endl;
      else if ( command != 1 && Database1.isEmpty() ) cout << endl << "### Choose 1 first. ###" << endl << endl; //非由指令1開始
      else if ( command == 1 ) { 
        if ( i != 0 ){ //重複叫1就重置 
        	Database1.Clear();
        	again = false ; 
		} 
      
        cout << endl << "Input a file number ([0] Quit): " ;
      	cin >> fileNum;
      	
      	if ( Database1.Load(fileNum) ) {
      	  i = i + 1; //執行1的成功次數 
          //Database1.Show(); 
      	  cout << endl;
        }
        
      }
      
      else if ( command == 2 ) {
		
		if ( again ) {
			cout << "### AVL tree has been built. ###" << endl ;
		}
		
		Database2.recieve(Database1.GetData()); //把任務一讀檔資料抓過來 
		//Database2.Show(); 
		Database2.Procedure() ;
		cout << endl;
		again = true ;
		
      } 
      
  } while ( command != 0 );
    
  system("pause");
  return 0;
}

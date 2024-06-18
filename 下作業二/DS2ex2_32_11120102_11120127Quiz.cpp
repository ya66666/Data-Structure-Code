//11120102 ���ͥ�
//11120127 ���ɥ�

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
	char SN[100] ; //�զW 
	char SB[100] ; // ��t 
	char DAN[100] ; //��] 
	char GA[100] ; //���� 
	char ST[100] ; //�ǥͼ�
	char TE[100] ; //�Юv��
	int GR ; //�W�Ǧ~�ײ��~�ͼ�
	int student ;
	int num ; //�s�� 
} ; 

struct Data{
	int num;
	int student;
};

struct Node {	
    vector<Data> data;
    vector<Data> LeftSameData;
    vector<Data> MiddleSameData;
    vector<Data> RightSameData;
    Node* children[4]; //�����k�Ȧs 
    Node* parent; 

    Node( Data input ) {
	  data.push_back( input ) ; 
	  children[0] = NULL;
	  children[1] = NULL;
	  children[2] = NULL;
	  children[3] = NULL;
	  parent = NULL;
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
  vector<Data> ALL ; //�u�s�ǥͼƸ�Ǹ� 
  vector<DataFake> AllData ; //�s���㤺�e 
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
    
      while(i < 3){ //�����e�T��԰� 
        fgets(trash, sizeof(trash), infile);
        i++;
      }
      
      DataFake temp;
      Data useful;
      char En[100]; //�� 
      char St[100]; // �Ǥh 
      string GRA; // ���~�ͼ� 
      string FXK;
      int number = 1;
      while( fscanf(infile,"%s %s %s %s %s", trash, &temp.SN[0], trash, &temp.SB[0], &temp.DAN[0]) != EOF ){
      	if( isupper(temp.DAN[0]) ) { //D��S�d�� 
      	  fscanf(infile,"%s %s", &En[0], &temp.GA[0] );
      	  FXK.erase();
		  FXK = temp.GA;
      	  
      	  if( isupper(temp.GA[0]) ){ //GA�S�|  
      	    fscanf(infile,"%s %s %s %d %s %s %s %s",  &St[0], &temp.ST[0], &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash);
            strcat(temp.GA, " ");
            strcat(temp.GA, St);
		  }
		  else if ( (temp.GA[0] == '5' || temp.GA[0] == '2' ) && FXK.length() == 1 ){
		  	fscanf(infile,"%s %s %s %d %s %s %s %s",  &St[0], &temp.ST[0], &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash);
            strcat(temp.GA, " ");
            strcat(temp.GA, St);
		  }
		  else{ // �d�� 
		    fscanf(infile, "%s %d %s %s %s %s", &temp.TE[0], &temp.GR, &trash, &trash, &trash, &trash );
		    strcpy(temp.ST, temp.GA);
		    
		    memset(temp.GA, 0, sizeof(temp.GA));

		  }
		  
		  strcat(temp.DAN, " ");
          strcat(temp.DAN, En);
		} // if
      	else { //D��d�� 
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
		
		useful.student = temp.GR;
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
  
  void InsertItem( Node*& root, Data& newData ) {
	if (root == NULL) {
	  root = new Node( newData ) ; // first data
    } 
    else if (root->data.size() == 2){ //��e�`�I��2����� 
      if ( newData.student < root->data[0].student) {
      	if (root->children[0] == NULL){
      		root -> data.push_back( newData );
      		swap(root->data[0], root->data[2]);
      		swap(root->data[1], root->data[2]);
      		Split(root); 
		}
        else InsertItem(root->children[0], newData);
      } 
	  else if ( newData.student > root->data[0].student && newData.student < root->data[1].student ){
	  	if (root->children[1] == NULL){
	  		root -> data.push_back( newData );
	  		swap(root->data[1], root->data[2]);
      		Split(root);
	    }
        else InsertItem(root->children[1], newData);
      }
      else if ( newData.student > root->data[1].student ){
      	if (root->children[2] == NULL){
      		root -> data.push_back( newData );
      		Split(root);
        }
        else InsertItem(root->children[2], newData);
      }
	}
	else if (root->data.size() == 1){ //  ��e�`�I��1����� 
	  if ( newData.student < root->data[0].student) {
	  	if (root->children[0] == NULL){
	  		root -> data.push_back( newData );
	  		swap(root->data[0], root->data[1]);
	    }
        else InsertItem(root->children[0], newData);
      } 
	  else if ( newData.student > root->data[0].student ){
	  	if (root->children[2] == NULL){
	  		root -> data.push_back( newData );
	    }
        else InsertItem(root->children[2], newData);
      }
    }
    
    return;
  }
  
  void Split(Node*& node){ // ��3��Ƹ`�I�ǹL�� 
    Node* p;
    Node* NodeParent = node->parent;
    Node* left;
	Node* right; 

    if (NodeParent == NULL) { //�����N�Oroot 
        p = new Node(node->data[1]); //�W�� 
        left = new Node(node->data[0]); // �������� 
        right = new Node(node->data[2]); // �������k
        
        if (NodeParent != NULL) {
        	NodeParent->children[0] = NULL;
        	NodeParent->children[2] = NULL;
		}
		
        //���W����3�`�I 
        p->children[0] = left;
        p->children[2] = right;
        
        //�]�w�p�Ĥ��`�I��p
        left->parent = p;
        right->parent = p;
        
        //��node���p�ī��� 
        left->children[0] = node->children[0]; //��node���p�ī��� 
        left->children[2] = node->children[1];  
        right->children[0] = node->children[2];
        right->children[2] = node->children[3];
		delete node;
        node = p; // ��s root �`�I
    } 
	else {
      left = new Node(node->data[0]); // �������� 
      right = new Node(node->data[2]); // �������k
        
        // �N������ƶ����J���`�I
      if (NodeParent->data.size() == 1) {
        if (node->data[1].student < NodeParent->data[0].student) {
            NodeParent->data.insert(NodeParent->data.begin(), node->data[1]); //�[�J���`�I �Ĥ@����
            //if (node->parent->children[0] != NULL) cout << "True";
            /*cout << NodeParent->data[0].student << "���`�I" << endl ; 
            cout << NodeParent->data[1].student << "���`�I" << endl; 
            cout << NodeParent->children[0]->data[0].student << "���`�I�����l�`�I" << endl; 
            cout << NodeParent->children[0]->data[1].student << "���`�I�����l�`�I" << endl; 
            cout << NodeParent->children[0]->data[2].student << "���`�I�����l�`�I" << endl; 
            cout << NodeParent->children[2]->data[0].student << "���`�I���k�l�`�I" << endl; */
            NodeParent->children[0] = NULL;
            NodeParent->children[1] = NULL;
            NodeParent->children[0] = left;
            NodeParent->children[1] = right;
        } 
        else if (node->data[1].student > NodeParent->data[0].student){
        	NodeParent->data.push_back(node->data[1]); //�[�J���`�I �ĤG���� 
        	NodeParent->children[1] = NULL;
            NodeParent->children[2] = NULL;
            NodeParent->children[1] = left;
            NodeParent->children[2] = right;
		}
		//cout << "Hi";
		//�]�w���`�I
        left->parent = NodeParent;
        right->parent = NodeParent;
            
        //��node���p�ī��� 
        left->children[0] = node->children[0]; //��node���p�ī��� 
        left->children[2] = node->children[1];  
        right->children[0] = node->children[2];
        right->children[2] = node->children[3];
		delete node;
        node = NodeParent; // ��s root �`�I
      }
      else if (NodeParent->data.size() == 2) {
      	if (node->data[1].student < NodeParent->data[0].student) {
            NodeParent->data.insert(NodeParent->data.begin(), node->data[1]); //�[�J���`�I �Ĥ@���� 
            NodeParent->children[3] = NodeParent->children[2];
            NodeParent->children[2] = NULL;
            NodeParent->children[2] = NodeParent->children[1];
            NodeParent->children[1] = NULL;
            NodeParent->children[0] = left;
            NodeParent->children[1] = right;
        } 
        else if (node->data[1].student > NodeParent->data[0].student && node->data[1].student < NodeParent->data[1].student){
        	NodeParent->data.push_back(node->data[1]);
        	swap(NodeParent->data[1], NodeParent->data[2]); //��D���� 
        	NodeParent->children[3] = NodeParent->children[2];
        	NodeParent->children[2] = NULL;
        	NodeParent->children[1] = left;
            NodeParent->children[2] = right;
		}
        else if (node->data[1].student > NodeParent->data[1].student){
        	NodeParent->data.push_back(node->data[1]); //�[�J���`�I �ĤT���� 
            NodeParent->children[2] = left;
            NodeParent->children[3] = right;
		}
		
		//�]�w���`�I
        left->parent = NodeParent;
        right->parent = NodeParent;
        
        //��node���p�ī��� 
        left->children[0] = node->children[0]; //��node���p�ī��� 
        left->children[2] = node->children[1];  
        right->children[0] = node->children[2];
        right->children[2] = node->children[3];
		delete node;
        node = NodeParent; // ��s root �`�I
	  }
	}	
     // �ˬd���`�I�O�_���F�A�p�G���F�A�h���k����
    if (node->data.size() == 3) {
      Split(node);
    }
  	
  }
  
  int getHeight(Node* root) {
    if (root == NULL) {
        return 0; // �ž𪺰��׬� 0
    }

    // ���j�p�⥪�l��B�����l��M�k�l�𪺰���
    int leftHeight = getHeight(root->children[0]);
    int middleHeight = getHeight(root->children[1]);
    int rightHeight = getHeight(root->children[2]);

    // ��^���l��B�����l��M�k�l�𤤳̤j�����סA�å[ 1 ��ܥ]�A��e�`�I
    return max(max(leftHeight, middleHeight), rightHeight) + 1;
  }
  
  
  int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }

    // ���j�p�⥪�l��B�����l��M�k�l�𪺸`�I��
    int leftCount = countNodes(root->children[0]);
    int middleCount = countNodes(root->children[1]);
    int rightCount = countNodes(root->children[2]);

    // ��^���l��B�����l��B�k�l��M�ڸ`�I���`�M
    return leftCount + middleCount + rightCount + 1;
  }
  
  void PrintData( Node* root ){
  	int i = 0;
  	while( i < root->data.size() ){
  		//cout << root->data.size() <<endl;
  		//cout << i+1 << ": [" << root->data[i].num << "] " << root->data[i].SN << ", " << root->data[i].SB << ", "  <<  root->data[i].DAN << ", " << root->data[i].GA << ", " << root->data[i].ST << ", " << root->data[i].student << endl;
  		/*cout << root->children[0]->data.size() << endl;
  		cout << i+1 << ": [" << root->children[2]->parent->data[0].num << "]" << root->children[2]->parent->data[0].student << endl;
  		cout << i+1 << ": [" << root->children[2]->data[i].num << "]" << root->children[2]->data[i].student << endl;*/
  		i = i + 1;
	}
  }
  
  void clr(Node*& root) {
    if (root != NULL) {
        clr(root->children[0]);
        clr(root->children[1]);
        clr(root->children[2]);
        clr(root->children[3]);
        delete root;
        root = NULL;
    }
  }
  
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
  
  void Insert(){
  	int j = 0;
  	while ( j < ALL.size() ){
      InsertItem( ATree, ALL[j]);
      j = j + 1;
	} 
  }
  
  vector<DataFake> GetData() {
  	return AllData;
  }
  
  int Height(){
  	return getHeight( ATree );
  }
  
  int Count(){
  	return countNodes( ATree );
  }
  
  void Print(){
  	PrintData( ATree );
  }
  
  int Size(){
  	return AllData.size();
  } 
  
  void Clear(){
  	ALL.clear();
  	AllData.clear();
  	clr(ATree);
  }
  
};

class AVLTree{
	
private:
  vector<Data> ALL ; //�u�s�ǥͼƸ�Ǹ� 
  vector<DataFake> AllData ; //�s���㤺�e 
  
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
  TreeAVL * cur = NULL, * top = NULL ;
  int now_num = 0, height = -99, right_height = 0, left_height = 0, number_of_nodes = 1, tree_height = 1, print_node = 0, order = 1 ; ;
  
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
  
  void Delete( TreeAVL * &root ) {
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
	now_num = 0, height = -99, right_height = 0, left_height = 0, number_of_nodes = 1, tree_height = 1, order = 1 ;
	
  }
  
  void Procedure() {
	TreeAVL * head = NULL ;
	int j = 1 ;
  	for ( int i = 0 ; i < AllData.size() ; i += 1 ) {
  		Initial() ;
//  		cout << "num = " << now_num << endl ;
//		cout << "student = " << cur -> info.student << endl ;
  		Insert( head, cur ) ;
	}	
//--------------------------------------------------------------------
	Right_Left_Height( head, tree_height ) ;
	tree_height = height ;
  	cout << "Tree height = " << tree_height << endl ;
  	height = -99 ;
  	
  	How_many_nodes( head ) ;
  	cout << "Number of nodes = " << number_of_nodes << endl ;
//--------------------------------------------------------------------
  	for ( TreeAVL * temp = head ; temp != NULL ; temp = temp -> side, j += 1 ) {
  		cout << j << ": [" << temp -> info.num << "] " << temp -> info.SN << ", " << temp -> info.SB << ", " << temp -> info.DAN << ", " << temp -> info.GA << ", " << temp -> info.ST << ", " << temp -> info.GR << endl ;
	}
	
	top = head ;
	head = NULL ;
  }
  
  void Print_First() {
  	int j = 1 ;
  	for ( TreeAVL * temp = top ; temp != NULL ; temp = temp -> side, j += 1 ) {
  		cout << j << ": [" << temp -> info.num << "] " << temp -> info.SN << ", " << temp -> info.SB << ", " << temp -> info.DAN << ", " << temp -> info.GA << ", " << temp -> info.ST << ", " << temp -> info.GR << endl ;
	}
  }
  
  void Print_Node( TreeAVL * node ) {
  	TreeAVL * temp = node ;
  	if ( print_node > 0 ) {
  		do {
  			cout << order << ": [" << temp -> info.num << "] " << temp -> info.SN << ", " << temp -> info.SB << ", " << temp -> info.DAN << ", " << temp -> info.GA << ", " << temp -> info.ST << ", " << temp -> info.GR << endl ;
  			print_node -= 1 ;
  			temp = temp -> side ;
  			order += 1 ;
		} while ( temp != NULL && print_node > 0 ) ;
	}
  }
  
  void Search_Print( TreeAVL * node ) {
  	if ( node == NULL ) return ;
  			
  	Search_Print( node -> right ) ;
  	Print_Node( node ) ;
  	Search_Print( node -> left ) ;
  }
  
  void Search() {
  	int num = 0 ;
  	cout << endl << "Enter K in [1," << AllData.size() << "]: " ;
  	cin >> num ;
  	if ( num > 0 && num <= AllData.size() ) {
  		print_node = num ;
  		Search_Print( top ) ;
	}
  }
  
  bool isEmpty(){
  	if (AllData.empty()) return true;
  	else return false;
  }
  
  void Show(){
	show();
  }
  
  int Size(){
  	return AllData.size();
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
      cout << "* 3. Top-K search on 23 tree  *" << endl;
      cout << "* 4. Top-K search on AVL tree *" << endl;
      cout << "*******************************" << endl;
      cout << "Input a choice(0, 1, 2, 3, 4): " ;
    
      cin >> command;
      cin.ignore( Max_LEN, '\n' );
      
      if ( command == 0 ) break;
      else if( command >= 5 ) cout << endl << "Command does not exist!" << endl << endl;
      else if ( command != 1 && Database1.isEmpty() ) cout << "### Choose 1 first. ###" << endl << endl; //�D�ѫ��O1�}�l
      else if ( command == 1 ) { 
        if ( i != 0 ){ //���ƥs1�N���m 
        	Database1.Clear();
        	again = false ;
        	Database2.Remake() ;
			Database2.Clear() ;
			Database2.Delete( Database2.top ) ;
		} 
      
        cout << endl << "Input a file number ([0] Quit): " ;
      	cin >> fileNum;
      	
      	if ( Database1.Load(fileNum) ) {
      	  i = i + 1; //����1�����\���� 
          //Database1.Show(); 
          /*Database1.Insert();
          int h = Database1.Height() ;
      	  cout << "Tree height = " << h << endl;
      	  int c = Database1.Count() ;
      	  cout << "Number of nodes = " << c << endl;
      	  Database1.Print(); */
      	  cout << endl;
        }
        
      }
      
      else if ( command == 2 ) {
		
		if ( again ) {
			cout << "### AVL tree has been built. ###" << endl ;
			cout << "Tree height = " << Database2.tree_height << endl ;
			cout << "Number of nodes = " << Database2.number_of_nodes << endl ;
			Database2.Print_First() ;
		}
		else {
			Database2.recieve(Database1.GetData()); //����Ȥ@Ū�ɸ�Ƨ�L�� 
			Database2.Procedure() ;
		}
		
		//Database2.Show(); 
	    cout << endl << endl;
		again = true ;
		
      } 
      else if(command == 3){
      	
      	if( ! Database1.isEmpty()){
			cout << endl << "Enter K in [1," << Database1.Size() << "]: ";
			cin >> i;
			
			//if (i > 0 && i <= Database2.Size() ){
			//	Database1.Search(i);
			//}
			
			cout << endl;
		}
		else cout << "### Choose 1 first. ###" << endl << endl;
      }
      
	  else if(command == 4){
      	
      	if( ! Database2.isEmpty()){
			Database2.Search() ;
			cout << endl;
		}
		else cout << "### Choose 2 first. ###" << endl << endl;
	  }
      
  } while ( command != 0 );
    
  system("pause");
  return 0;
}

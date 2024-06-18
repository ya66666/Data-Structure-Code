#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Weight {
	int w ;
	bool choosen ;
	
	Weight(): w(INT_MAX), choosen(false) {} //初始化 
};

int FindSmallest( Weight weight[] ) { //回傳最小值且之前未被選擇過的index 
	int smallest = INT_MAX, index = -1 ;
	
	for ( int i = 0 ; i < 6 ; i++ ) {
		if ( weight[i].w < smallest && ! weight[i].choosen ) {
			smallest = weight[i].w ;
			index = i ;
		}
	}
	
	weight[index].choosen = true ;
	
	return index ;
}

void Dijkstra( vector <int> &VertexSet, Weight weight[], int adjList[6][7] ) {
	weight[0].w = 0 ; // 設定數字 
	int smallest_index = FindSmallest( weight ) ; // 找最小的index 
	VertexSet.push_back( smallest_index ) ; // push in VertexSet 
	int currentSmallestNum = weight[smallest_index].w ; // 更新最小值 
	
	for ( int i = 1 ; i < 6 ; i++ ) { // 重複五次(因點6不用做) 
		for ( int j = 1 ; j < 7 ; j++ ) {
			if ( adjList[smallest_index+1][j] != 0 && weight[j-1].w > currentSmallestNum + adjList[smallest_index+1][j] ) 
				weight[j-1].w = currentSmallestNum + adjList[smallest_index+1][j] ; // 設定數字 
		} 
		
		smallest_index = FindSmallest( weight ) ; // 找最小的index 
		VertexSet.push_back( smallest_index ) ; // push in VertexSet 
		currentSmallestNum = weight[smallest_index].w ; // 更新最小值 
	}
}

int main() {
	int adjList[6][7] = {
		{ 0, 1, 2, 3, 4, 5, 6 },
		{ 1, 0, 1, 12, 0, 0, 0 },
		{ 2, 0, 0, 9, 3, 0, 0 },
		{ 3, 0, 0, 0, 0, 5, 0 },
		{ 4, 0, 0, 4, 0, 13, 15 },
		{ 5, 0, 0, 0, 0, 0, 4 }
	} ;

	vector <int> VertexSet ;
	Weight weight[6] ;
	Dijkstra( VertexSet, weight, adjList ) ;
	
	cout << "起點到終點的路徑結果：" << VertexSet[0] + 1 ;
	for ( int i = 1 ; i < VertexSet.size() ; i++ ) {
		cout << "->" << VertexSet[i] + 1 ;
	}
	
	cout << endl << "最短路徑長度：" << weight[5].w ;
	return 0 ;
}

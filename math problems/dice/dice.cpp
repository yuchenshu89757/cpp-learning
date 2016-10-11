/**
 * Subject: 扔色子问题
 * Detail: 同时仍n个色子，出现的点数组合共有多少组
 * Note: n = 2时，<1,2>和<2,1>视为同一种结果
 */
#include <iostream>
#include <vector>
using namespace std;

//色子的个数
int n;
//结果记录集
vector<vector<int>> res; 

void dfs(vector<int> path, int level)
{
	if(level == n)
		res.push_back(path);
	else{
		for(int i = 1; i < 7; ++i){
			if(!path.empty() && path.back() >  i)continue;
			path.push_back(i);
			dfs(path, level + 1);
			path.pop_back();
		}
	}
}

void printResult()
{
	cout << "n = " << n << endl;
	cout << "result count = " << res.size() << endl;
	
	for(size_t i = 0; i < res.size(); ++i){
		cout << res[i][0];
		for(size_t j = 1; j < res[i].size(); ++j)
			cout << " " << res[i][j]; 
		cout << endl;
	}
	cout << endl;
}

int main(void)
{
	freopen("in.txt","r", stdin);
	freopen("out.txt", "w", stdout);

	while(cin >> n){
		res.clear();
		vector<int> path;
		dfs(path, 0);
		printResult();
	}
	return 0;
}
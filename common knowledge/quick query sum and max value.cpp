/*
*   Copyright (C) 2016 All rights reserved.
*   
*   filename:        main.cpp
*   author:          macityor
*   e-mail:          yuchenshu89757@163.com
*   create time:     2016-10-28 16:00:31
*   last modified:   2016-10-28 17:00:31
*/

/*
Class QuickQuery is used to query the sum and maximum value in the range denoted by i, j quickly.
*/
#include <bits/stdc++.h>
using namespace std;

class QuickQuery{
public:
    QuickQuery(int*, int);
    QuickQuery(const QuickQuery&) = delete;
    QuickQuery &operator=(const QuickQuery&) = delete;
    ~QuickQuery();
    int QuerySum(int, int)const;
    int QueryMax(int, int)const;
private:
    void CheckRange(int, int)const;
private:
    int *_sum;
    int **_max;
    int length;
};

QuickQuery::QuickQuery(int *a, int n){
    assert(a != NULL && n > 0);
    length = n;
    _sum = new int[length];
    _max = new int*[length];
    for(int i = 0; i < length; ++i)
        _max[i] = new int[length];

    _sum[0] = a[0];
    for(int i = 1; i < length; ++i)
        _sum[i] = _sum[i-1] + a[i];
    for(int i = 0; i < length - 1; ++i){
        int max_index = i;
        _max[i][i] = a[i];
        for(int j = i + 1; j < length; ++j){
            if(a[j] > a[max_index])
                max_index = j;
            _max[i][j] = a[max_index];
        }
    }
}

QuickQuery::~QuickQuery(){
    for(int i = 0; i < length; ++i)
        delete []_max[i];
    delete []_sum;
    delete []_max;
}

void QuickQuery::CheckRange(int i, int j)const{
    assert(i >= 0 && j < length && i <= j);
}

int QuickQuery::QuerySum(int i, int j)const{
    CheckRange(i, j);
    if(i == 0)return _sum[j];
    return _sum[j] - _sum[i-1];
}

int QuickQuery::QueryMax(int i, int j)const{
    CheckRange(i, j);
    return _max[i][j];
}

int main(void)
{
    int a[] = {3,1,4,1,4,5,2,6,3,7,4,8};
    QuickQuery qq(a, 12);
    int i, j;
    while(cin >> i >> j){
        cout << qq.QuerySum(i, j) << endl;
        cout << qq.QueryMax(i, j) << endl;
    }
    return 0;
}
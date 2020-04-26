#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

const double eps = 1e-9;
int Gauss(vector<vector<double>> a) {
    int n = (int)a.size(), m = (int)a[0].size();
    double det = 1; int rank = 0;
    for(int col = 0, row = 0; col < m && row < n; ++col) {
        int mx = row;
        for(int i = row; i < n; i++) if(fabs(a[i][col]) > fabs(a[mx][col])) mx = i;
        if(fabs(a[mx][col]) < eps) {det = 0; continue;}
        for(int i = col; i < m; i++) swap(a[row][i], a[mx][i]);
    	if (row != mx) det = -det;
    	det *= a[row][col];
        for(int i = 0; i < n; i++) {
            if(i != row && fabs(a[i][col]) > eps) {
                double c = a[i][col] / a[row][col];
                for(int j = col; j < m; j++) a[i][j] -= a[row][j] * c;
            }
        }
        ++row; ++rank;
    }
    return det;
}
int main() {
    int n, m; cin >> n >> m;
    vector< vector<double> > v(n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            double x; cin >> x; v[i].push_back(x);
        }
    }
    cout << Gauss(v) << '\n';
    return 0;
}

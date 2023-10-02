/*
  Author Lakshay Goel
  Github profile: https://github.com/MrLakshay
  Problem link: https://www.geeksforgeeks.org/program-to-find-lcm-of-two-numbers/
  Given are two numbers we need to find their LCM
*/
#include<bits/stdc++.h>
using namespace std;
unsigned long long int gcd(unsigned long long int num1,unsigned long long int num2){
    if(num2 == 0){
        return num1;
    }
    else{
        gcd(num2,num1%num2);
    }
}
unsigned long long int lcm(int a,int b){
    unsigned long long int x=gcd(a,b);
    unsigned long long int result=((a/x)*(b));
    return result;
}
int main()
{
    unsigned long long int n1,n2;cin>>n1>>n2;
    cout<<lcm(n1,n2);
    return 0;
}

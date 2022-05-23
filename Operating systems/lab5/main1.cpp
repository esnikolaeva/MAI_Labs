#include <iostream>
using namespace std;

//extern "C" для использования float в работе с С++

extern "C" float SinIntegral(float A, float B, float e);
extern "C" float Derivative(float A, float deltaX);

int main()
{
    int flag;
    cout << "Please enter the flag:\n";
    while (scanf("%d",&flag) > 0 ){
        if (flag == 1){
            cout<<"Please enter your dates:\n";
            float A, B, e;
            cin>>A>>B>>e;
            cout<<"SinIntegral("<< A<<","<<B<<","<< e<<")="<<SinIntegral(A, B, e)<< endl;
        }
        else if (flag == 2){
            cout<<"Please enter your dates:\n";
            float A, deltaX;
            cin>>A>>deltaX;
            cout<< "Derivative("<<A<<","<<deltaX<<")="<<Derivative(A,deltaX)<<endl;
        }
    }
    return 0;
}

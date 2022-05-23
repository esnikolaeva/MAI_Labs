#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

using namespace std;

int main()
{
    void* adres = NULL;//адресс для доступа к библиотеке
    
    //указатели на функции 1 и 2
    float (*SinIntegral)(float A, float B, float e);
    float (*Derivative)(float A, float deltaX);
    
    const char* libeary_mas[]={"libd1.so", "libd2.so"};
    int curlib;
    int statl;
    cout<<"Enter the start library:"<<endl;
    cout << '\t' << "1 for first library" <<endl;
    cout << '\t' << "2 for second library" <<endl;
    cin>>statl;
    bool flag = 1;
    while(flag){
        if (statl==1){
            curlib = 0;
            flag = 0;
        }
        else if(statl==2){
            curlib=1;
            flag=0;
        }
        else{
            cout<<"You enter the wrong number. Enter again!"<<endl;
            cin>>statl;
        }
    }
    adres = dlopen(libeary_mas[curlib],RTLD_LAZY);//RTLD_LAZY выполняется поиск только тех символов, на которые есть ссылки из кода
    if (!adres){
        cout<<"Error";
        exit(EXIT_FAILURE);
    }
    SinIntegral = (float(*)(float, float, float))dlsym(adres,"SinIntegral");//возвращаем адрес функции из памяти библиотеки /dlsym присваивает указателю на функцию, объявленному в начале, ее адрес в библиотеке
    Derivative = (float(*)(float, float))dlsym(adres, "Derivative");
    int command;
    cout<<"Please read the nexst ruls? before you enter the command";
    cout << '\t' << "0 for changing the contract;" << std:: endl;
    cout << '\t' << "1 for calculating the SinIntegral; " << std:: endl;
    cout << '\t' << "2 for calculating the Derivative; " << std:: endl;
    while(printf("Please enter your command: ") && (scanf("%d", &command)) != EOF){
        if (command==0){
            dlclose(adres);//освобождает указатель на библиотеку и программа перестает ей пользоваться
            if (curlib==0){
                curlib=1;
                adres=dlopen(libeary_mas[curlib],RTLD_LAZY);
                if(!adres){
                    cout<<"Error";
                    exit(EXIT_FAILURE);
                }
                SinIntegral = (float(*)(float, float, float))dlsym(adres,"SinIntegral");//возвращаем адрес функции из памяти библиотеки /dlsym присваивает указателю на функцию, объявленному в начале, ее адрес в библиотеке
                Derivative = (float(*)(float, float))dlsym(adres, "Derivative");
   
            }
            else if(curlib==1){
                curlib=0;
                adres = dlopen(libeary_mas[curlib],RTLD_LAZY);
                if(!adres){
                    cout<<"Error";
                    exit(EXIT_FAILURE);
                }
                SinIntegral = (float(*)(float, float, float))dlsym(adres,"SinIntegral");//возвращаем адрес функции из памяти библиотеки /dlsym присваивает указателю на функцию, объявленному в начале, ее адрес в библиотеке
                Derivative = (float(*)(float, float))dlsym(adres, "Derivative");
 
            }
            cout << "You have changed contracts!" << std:: endl;
        }
        else if (command==1){
            float A, B, e;
            cin>>A>>B>>e;
            float  sinintegral =SinIntegral(A,B,e);
            if (sinintegral==-1){
                cout<<"please enter again\n";
            }
            else{
                cout<<"SinIntegral("<< A<<","<<B<<","<< e<<")="<<SinIntegral(A, B, e)<< endl; 
            }
        }
        else if (command==2){
            float A, deltaX;
            cin>>A>>deltaX;
            float  derivative =Derivative(A, deltaX);
            if (derivative==-1){
                cout<<"please enter again\n";
            }
            else{
                cout<< "Derivative("<<A<<","<<deltaX<<")="<<Derivative(A,deltaX)<<endl;
            }
        }
        else{
            cout<<"You had to enter only 0, 1 or 2!" << std:: endl;
        }
    }
    dlclose(adres);
    return 0;
}


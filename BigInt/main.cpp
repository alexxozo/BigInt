#include <iostream>
#include "BigInt.h"

using namespace std;

int main()
{

    BigInt a, b;
    int power;
    /*cout<<"Introdu doua numere mari:";
    cin>>a;
    cin>>b;*/
    a = "123123134534532";
    b = "567567345344";
    cout<<"Suma:"<<a+b;
    cout<<"Diferenta:"<<a-b;
    cout<<"Inmultire:"<<a*b;
    cout<<"Impartire:"<<a/b;
    cout<<"Ridica numarul a la puterea:";
    cin>>power;
    cout<<a.pow(power);
    if(a.isEven() == 1) cout<<"Numarul a e par.\n";
    else cout<<"Numarul "<<a<<" e impar.\n";
    if(a.isPalindrom() == 1) cout<<"Numarul a e palindrom.\n";
    else cout<<"Numarul a nu e palindrom.\n";
    cout<<"Suma cifrelor numarului a este "<<a.sumOfDigits()<<".";




}

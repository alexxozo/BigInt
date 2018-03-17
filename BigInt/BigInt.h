#ifndef BIGINT_H_INCLUDED
#define BIGINT_H_INCLUDED

#include <iostream>

class BigInt {

    short *number;                ///vectorul in care vom memora numarul
    short sign;                   ///semnul numarului 0/-1
    int dim;                      ///dimensiunea numarului

    public:
        ///---Constructors Destructor---///
        BigInt();
        BigInt(long long int x);
        BigInt(char *s);
        BigInt(const BigInt &b);
        ~BigInt();

        ///---Main functions---///
        BigInt operator + (const BigInt &b);
        BigInt operator - (const BigInt &b);
        BigInt operator * (const BigInt &b);
        BigInt operator *= (const BigInt &b);
        BigInt operator / (const BigInt &b);
        BigInt operator = (char *s);
        BigInt operator = (long long int x);
        BigInt pow(int power);
        short isEven();
        short isPalindrom();
        long long int sumOfDigits();

        ///---Helper Functions---///
        friend BigInt add(BigInt a, BigInt b);
        friend BigInt subtract(BigInt a, BigInt b);
        friend int isSmaller(BigInt a, BigInt b);
        friend BigInt multiply(BigInt a, BigInt b);
        friend BigInt divide(BigInt a, BigInt b);
        friend std::ostream& operator << (std::ostream& os, const BigInt& x);
        friend std::istream& operator >> (std::istream& is, BigInt& x);
};


#endif // BIGINT_H_INCLUDED

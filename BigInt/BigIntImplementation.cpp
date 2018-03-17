#include "BigInt.h"
#include<string.h>
using namespace std;

///---Constructors Destructor---///
BigInt::BigInt() {
    //Initializam cu 0 si pointerul arata spre null
    sign = 0;
    dim = 0;
    number = NULL;
}

BigInt::~BigInt() {
    //Eliberam memoria si setam cu 0 semn si dimensiune
    sign = 0;
    dim = 0;
    delete [] number;
}

BigInt::BigInt(long long int x) {
    //Salvam numarul in ordine inversa a cifrelor pt a face operatii mai usor
    //Facem o copie a lui x pt a afla dimensiunea
    long long cx = x;
    dim = 0;
    while(cx) {
        dim++;
        cx /= 10;
    }
    //Aflam semnul lui x
    sign = (x < 0) ? -1 : 0;
    //Daca x este negativ atunci il facem pozitiv pt a memora cifrele lui in vector
    x = (sign == -1) ? -x : x;
    number = new short[dim];
    for(int i = 0; i < dim; i++) {
        number[i] = x % 10;
        x /= 10;
    }
}

BigInt::BigInt(char* s) {
    //Aflam semnul lui s si dimensiunea
    sign = (s[0] == '-') ? -1 : 0;
    dim = strlen(s) + sign;
    number = new short[dim];
    //Indexul de la care incepem sa copiem cifrele
    //o sa fie 0 sau 1, daca avem nr negativ incepem de la pozitia 1
    //pentru a sari peste "-"
    int i = -sign;
    for(i; i < dim - sign; i++) {
        number[dim - sign - 1 - i] = s[i] - '0';
    }
}

BigInt::BigInt(const BigInt &b) {
    //Copiem valorile lui b in (*this)
    sign = b.sign;
    dim = b.dim;
    number = new short[dim];
    for(int i = 0; i < dim; i++) {
        number[i] = b.number[i];
    }
}

///---Helper functions---///
BigInt add(BigInt a, BigInt b) {
    short carry = 0, biggerHolder = 0, smallerHolder = 0, sum = 0;
    BigInt bigger = a, smaller = b;

    //Daca a si b sunt de dimensiune diferite le vom
    //memora corespunzator pt a ajuta la adunare
    if(a.dim != b.dim) {
        bigger = (a.dim > b.dim) ? a : b;
        smaller = (a.dim < b.dim) ? a : b;
    }

    //Parcurgem cele doua numere pana la sfarsitul celui
    //mai mic ca dimensiune
    for(int i = 0; i < smaller.dim; i++) {
        //Aceste variabile vor memora fiecare cifra
        biggerHolder = bigger.number[i];
        smallerHolder = smaller.number[i];

        //Facem suma cifrelor de pe pozitia i + transport
        sum = smallerHolder + biggerHolder + carry;
        carry = sum / 10;

        //Memoram suma in vectorul numarului mai mare ca dimensiune
        bigger.number[i] = sum % 10;
    }

    //Parcurgem ce a mai ramas din nr mai amre ca dimensiune
    //si adaugam transportul ramas
    for(int i = smaller.dim; i < bigger.dim; i++) {
        sum = bigger.number[i] + carry;
        bigger.number[i] = sum % 10;
        carry = sum / 10;
    }

    //Daca a mai ramas transport vom adauga un spatiu in plus in
    //vector si il vom memora
    if(carry > 0) {
        short *temp = new short[bigger.dim+1];

        ///memcpy(temp, bigger.number, bigger.dim * sizeof(short) + 1);  ///Are uneori problema in codeblocks

        for(int i = 0; i < bigger.dim; i++) {
            temp[i] = bigger.number[i];
        }

        temp[bigger.dim++] = carry;
        delete [] bigger.number;
        bigger.number = temp;
    }

    //Semnul rezultatului va fi semnul numarului mai mare
    bigger.sign = a.sign;

    return bigger;
}

BigInt subtract(BigInt a, BigInt b) {
    short carry = 0, biggerHolder = 0, smallerHolder = 0, sub = 0;
    BigInt bigger = a, smaller = b, result;

    //Memoram numerele in fucntie de care este mai mare
    if(isSmaller(a, b) == 1) {
        bigger = b;
        smaller = a;
    }

    //Alocam memorie pentru vectorul in care o sa emmoram rezultatul
    result.number = new short[bigger.dim];
    result.dim = bigger.dim;

    //Parcurgem vectorii pana la dimensiunea celui mai mic
    for(int i = 0; i < smaller.dim; i++) {
        biggerHolder = bigger.number[i];
        smallerHolder = smaller.number[i];

        //Facem scadere intre cifrele numarului
        sub = biggerHolder - smallerHolder - carry;

        //Daca diferenta intre cifre este negativa atunci adunam 10
        //si tinem minte in transport 1 la fel ca la matematica
        if(sub < 0) {
            sub = sub + 10;
            carry = 1;
        } else {
            carry = 0;
        }

        //Memoram pe pozitia i diferenta
        result.number[i] = sub;
    }

    //Continuam parcurgerea pana la sfaristul vectorului mai mare si repetam
    //acelasi algoritm
    for(int i = smaller.dim; i < bigger.dim; i++) {
        sub = bigger.number[i] - carry;
        if(sub < 0) {
            sub = sub + 10;
            carry = 1;
        } else {
            carry = 0;
        }
        result.number[i] = sub;
    }

    //Daca rezultatul are 0-uri la inceput, le eliminam
    //insa daca toate sunt 0 inseamna ca rezultatul va fi 0
    int counter = result.dim;
    while(result.number[counter - 1] == 0) {
        counter--;
    }

    if(counter == 0) {
        result.dim = 1;
    } else {
        result.dim = counter;
    }


    //Setam semnul corespunzator
    result.sign = bigger.sign;

    return result;
}

int isSmaller(BigInt a, BigInt b) {
    //Aceasta este o functie ajutatoare cu care ne putem da
    //seama daca un numbar e mai mic ca altul
    long long int aLength, bLength;
    aLength = a.dim;
    bLength = b.dim;

    if(aLength < bLength) return 1;
    if(aLength > bLength) return -1;

    for(int i = aLength - 1; i >= 0; i--) {
        if(a.number[i] < b.number[i]) return 1;
        else if(a.number[i] > b.number[i]) return -1;
    }

    return 0;
}

BigInt multiply(BigInt a, BigInt b) {
    short carry = 0;
    int aSize = a.dim;
    int bSize = b.dim;
    int firstIndex = 0, secondIndex = 0;
    BigInt result, first, second;

    //Daca incercam sa inmultim doua numere nule
    if(aSize == 0 || bSize == 0) {
        result.dim = 1;
        result.number[0] = 0;
        result.sign = 0;
        return result;
    }

    //Alocam spatiu pentru rezultat
    result.number = new short[aSize + bSize];
    result.dim = aSize + bSize;

    first = a;
    second = b;

    //Parcurgem vectorii inmultind fiecare cifra a primului
    //cu fiecare a celui de al doilea
    for(int i = 0; i < aSize; i++) {
        carry = 0;
        secondIndex = 0;
        //Cifra de pe pozitia i din primul numar
        int firstHolder = first.number[i];

        for(int j = 0; j < bSize; j++) {
            //Cifra de pe pozitia j din al doilea numar
            int secondHolder = second.number[j];

            //Calculam suma ca la matematica
            int sum = (firstHolder * secondHolder) + result.number[firstIndex + secondIndex] + carry;
            carry = sum / 10;
            result.number[firstIndex + secondIndex] = sum % 10;

            secondIndex++;
        }

        //Daca ne ramane ceva in transport il adaugam pe ultima pozitie (care de fapt este prima in scrierea normala)
        if(carry > 0) {
            result.number[firstIndex + secondIndex] += carry;
        }

        firstIndex++;
    }

    //Daca rezultatul are 0-uri la inceput, le eliminam
    //insa daca toate sunt 0 inseamna ca rezultatul va fi 0
    int counter = result.dim;
    while(result.number[counter - 1] == 0) {
        counter--;
    }

    if(counter == 0) {
        result.dim = 1;
    } else {
        result.dim = counter;
    }
    result.sign = (first.sign - second.sign);

    //Setam semnul corespunzator
    if(second.sign == -1 && first.sign == 0) result.sign = -1;

    return result;
}

BigInt divide(BigInt a, BigInt b) {
    BigInt dividend, divisor, result = "1", rest;
    //Stocam rezultatul intr un int si la sfarsit il transformam in bigint pentru usurinta
    int resInt = 0;

    //Deimpartit si impartitior
    dividend = a;
    divisor = b;

    //Daca impartitorul este mai mare ca deimpartitul atunci rezultatul e 0
    if(isSmaller(dividend, divisor) == 1) {
        result = "0";
        return result;
    }

    //Impartim prin scaderi repetate
    do {
        rest = dividend - divisor;
        resInt++;
        dividend = rest;
    } while(isSmaller(divisor, rest) != -1);

    result = resInt;

    //Setam semnul corespunzator
    result.sign = (a.sign - b.sign);
    if(b.sign == -1 && a.sign == 0) result.sign = -1;

    return result;
}

///---Operators---///
BigInt BigInt::operator + (const BigInt &b) {
    BigInt result;

    //Logica semnelor
    if(sign == b.sign) {
        result = add((*this), b);
    } else {
        result = subtract((*this), b);
    }

    return result;
}

BigInt BigInt::operator - (const BigInt &b) {
    BigInt result;
    //Avem nevoie de o variabila noua pt b deoarece daca avem cazul
    //a - b cu b pozitiv si b > a semnul rezultatului va fi minus asa ca dam
    //lui b semnul minus
    BigInt newB = b;
    newB.sign = (newB.sign == -1) ? 0 : -1;

    if(newB.sign == sign) {
        result = add((*this), newB);
    } else {
        result = subtract((*this), newB);
    }

    return result;
}

BigInt BigInt::operator * (const BigInt &b) {
    return multiply((*this), b);
}

BigInt BigInt::operator *= (const BigInt &b) {
    return multiply((*this), b);
}

BigInt BigInt::operator / (const BigInt &b) {
    return divide((*this), b);
}

BigInt BigInt::operator = (char *s) {
    //Aflam semnul lui s si dimensiunea
    sign = (s[0] == '-') ? -1 : 0;
    dim = strlen(s) + sign;
    number = new short[dim];
    //Indexul de la care incepem sa copiem cifrele
    //o sa fie 0 sau 1, daca avem nr negativ incepem de la pozitia 1
    //pentru a sari peste "-"
    int i = -sign;
    for(i; i < dim - sign; i++) {
        number[dim - sign - 1 - i] = s[i] - '0';
    }
}

BigInt BigInt::operator = (long long int x) {
    //Salvam numarul in ordine inversa a cifrelor pt a face operatii mai usor
    //Facem o copie a lui x pt a afla dimensiunea
    long long cx = x;
    dim = 0;
    while(cx) {
        dim++;
        cx /= 10;
    }
    //Aflam semnul lui x
    sign = (x < 0) ? -1 : 0;
    //Daca x este negativ atunci il facem pozitiv pt a memora cifrele lui in vector
    x = (sign == -1) ? -x : x;
    number = new short[dim];
    for(int i = 0; i < dim; i++) {
        number[i] = x % 10;
        x /= 10;
    }
}

std::ostream& operator << (std::ostream& os, const BigInt& x) {
    //Afisarea se va face de la sfarsitul vectorului intrucat memoram numarul in ordine
    //inversa a cifrelor
    if(x.sign == -1) os<<"-";
    for(int i = x.dim - 1; i >= 0; i--) {
        os<<x.number[i];
    }
    os<<"\n";
    return os;
}

std::istream& operator >> (std::istream& is, BigInt& x) {
    //Citim numarul ca char[] si apoi il transformam in BigInt
    char *temp;
    temp = new char[1000];
    is>>temp;
    x = temp;
    delete [] temp;
}

///---Other Functions---///
BigInt BigInt::pow(int power) {
    //Inmultiri repetate de power ori
    BigInt result = *this;
    for(int i = 1; i < power; i++) {
        result = result * (*this);
    }
    return result;
}

short BigInt::isEven() {
    //Verificam ultima cifra
    if(dim >= 1) {
        if(number[0] % 2 == 0) {
            return 1;
        } else return 0;
    }
}

short BigInt::isPalindrom() {
    //verificam daca i si complementul lui i tin aceleasi cifre
    for(int i = 0; i < dim; i++) {
        if((*this).number[i] != (*this).number[dim - 1 - i]) return 0;
    }
    return 1;
}

long long int BigInt::sumOfDigits() {
    //Adunam fiecare cifra a nuamrului
    long long int sum = 0;

    for(int i = 0; i < dim; i++) {
        sum += number[i];
    }

    return sum;
}

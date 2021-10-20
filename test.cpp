#include <iostream>

using namespace std;

int main()
{
    int prize = 80000;
    double count = 0.0871993;
    double tmp, suma = 0, sellPercent = 0.05;

    for(int i = 0; i < 30; i++)
    {
        
        tmp = count * sellPercent;
        count -= tmp; 
        suma += prize * tmp;
        cout << i + 1 << ". iterace \n";
        cout << "cena: " << prize << "\n";
        cout << "zbyvajici mnozstvi: " << count << "\n";
        cout << "prodane mnozstvi: " << tmp << "\n";
        cout << "celkove prodane mnozstvi: " << 0.0871993 - count << "\n";
        cout << "vydelek: " << prize*tmp << "\n";
        cout << "celkem vydelano: " << suma << "\n";

        prize += 10000;
    }

    cout << sellPercent << "\n\n" << (count/0.0871993)*100 << "\n" << 0.0871993 - count << "\n";
}
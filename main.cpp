#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

#include <openssl/aes.h>
#include <openssl/evp.h>

using namespace std;


int main(int argc, char const *argv[])
{
    //overeni poctu argumentu
    if (argc == 2)
    //cast pro poslouchani a ukladani souboru nebo vypsani napovedy
    {
        if(strcmp("-l", argv[1]) == 0 || strcmp("-L", argv[1]) == 0)
        {
            cout << "Enter to listen mode..\n";
        }
        else if (strcmp("-h", argv[1]) == 0 || strcmp("-H", argv[1]) == 0)
        {
            cout << "Help..\n";
        }
        else
        {
            cout << "You gave invalid parameters\nTry help -h|-H\n";
            return 1;
        }
    }
    else if (argc == 5)
    //cast pro sifrovani a posilani souboru
    {
        //overeni spravnosti prepinacu
        if(strcmp("-r", argv[1]) == 0 || strcmp("-s", argv[3]) == 0)
        {
            cout << "Enter to sender mode..\n";
            
            //kontrola souboru
            fstream file (argv[2]);
            if(!file.is_open())
            {
                cout << "You gave invalid file\n";
                return 1;
            }
            
            string line;  
            while (getline(file,line) )
            {
                cout << line << '\n';
            }

            file.close();



        }
        else
        {
            cout << "You gave invalid parameters\nTry help -h|-H\n";
            return 1;
        }
    }
    else
    {
        printf("You gave invalid parameters\nTry help -h|-H\n");
        return 1;
    }
    return 0;
}
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <memory>
#include <limits>
#include <stdexcept>

#include <openssl/evp.h>
#include <openssl/conf.h>

using namespace std;

unsigned char* key = (unsigned char*)"147258369fedcba0";


int AES_encrypt(unsigned char* text, int text_len, unsigned char* cipher)
{
    int cipher_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        cout << "EVP_CIPHER_CTX_new failed\n";
        exit(2);
    }

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        cout << "EVP_EncryptInit_ex failed\n";
        exit(2);
    }

    if(!EVP_EncryptUpdate(ctx, cipher, &len, text, text_len))
    {
        cout << "EVP_EncryptUpdate failed\n";
        exit(2);
    }

    cipher_len += len;
    cout << cipher_len << "\n";
 
    if(!EVP_EncryptFinal_ex(ctx, cipher + len, &len))
    {
        cout << "EVP_EncryptFinal_ex failed\n";
        exit(2);
    }

    cipher_len += len;
    cout << cipher_len << "\n";

    EVP_CIPHER_CTX_free(ctx);
    
    return cipher_len;
}

int AES_decrypt(unsigned char* cipher, int cipher_len, unsigned char* text)
{
    int text_len = 0;
    int len = 0;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        cout << "EVP_CIPHER_CTX_new failed\n";
        exit(3);
    }

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
    {
        cout << "EVP_DecryptInit_ex failed\n";
        exit(3);
    }

    if(!EVP_DecryptUpdate(ctx, text, &len, cipher, cipher_len))
    {
        cout << "EVP_DecryptUpdate failed\n";
        exit(3);
    }

    text_len += len;

    if(!EVP_DecryptFinal_ex(ctx, text + len, &len))
    {
        cout << "EVP_DecryptFinal_ex failed\n";
        exit(3);
    }

    text_len += len;
    
    EVP_CIPHER_CTX_free(ctx);
    
    return text_len;
}


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
            unsigned char* cipher;
            cipher = (unsigned char*)malloc(128);  
            unsigned char* text;
            text = (unsigned char*)malloc(128);
            unsigned char* decrypted_line;
            decrypted_line = (unsigned char*)malloc(128);

            while (getline(file,line))
            {              
                memset(cipher, 0, 128);                
                memset(text, 0, 128);              
                memset(decrypted_line, 0, 128);

                strcpy((char*)text, line.c_str());
                printf("vstup = %s\n", text);
                printf("vstup-|%d, %c|-|%d, %c|-\n", text[0], text[0], text[line.length()-1], text[line.length()-2]);

                int cipher_len = AES_encrypt(text, line.length(), cipher);
                cout << "zasifrovano = \n";
                cout << cipher << "\n";
                for (int i = 0; i < cipher_len; i++)
                {
                    printf("%02x", (int)cipher[i]); 
                }
                cout << "\n";
                cout << "delka sifry = " << cipher_len << "\n";
            
                int dec_len = AES_decrypt(cipher, cipher_len, decrypted_line);
                cout << "desifrovano = \n";
                cout << decrypted_line << "\n";
                printf("vystup-|%d, %c|-|%d, %c|-\n", decrypted_line[0], decrypted_line[0], decrypted_line[line.length()-1], decrypted_line[line.length()-2]);
                for (int i = 0; i < dec_len; i++)
                {
                    printf("%c|", decrypted_line[i]);
                }
                cout << "\n";
                cout << "delka textu = " << dec_len << "\n\n";
            }


            free(cipher);
            free(text);
            free(decrypted_line);
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
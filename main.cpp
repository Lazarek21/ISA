#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <memory>

#include <openssl/evp.h>
#include <openssl/conf.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

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
 
    if(!EVP_EncryptFinal_ex(ctx, cipher + len, &len))
    {
        cout << "EVP_EncryptFinal_ex failed\n";
        exit(2);
    }

    cipher_len += len;

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

char * getIPaddrByHostname(char *addr_host, struct sockaddr_in *addr_con)
{
    printf("\nResolving DNS..\n");
    struct hostent *host_entity;
    char *ip=(char*)malloc(NI_MAXHOST*sizeof(char));
    int i;
  
    if ((host_entity = gethostbyname(addr_host)) == NULL)
    {
        // No ip found for hostname
        return NULL;
    }
      
    //filling up address structure
    strcpy(ip, inet_ntoa(*(struct in_addr *)
                          host_entity->h_addr));
  
    addr_con->sin_family = host_entity->h_addrtype;
    addr_con->sin_port = htons(P);
    addr_con->sin_addr.s_addr  = *(long*)host_entity->h_addr;
  
    return ip;
      
}


int main(int argc, char const *argv[])
{
    //overeni poctu argumentu
    if (argc == 2)
    //cast pro poslouchani a ukladani souboru nebo vypsani napovedy
    {           //listener mode
        if(strcmp("-l", argv[1]) == 0 || strcmp("-L", argv[1]) == 0)
        {       //listening
            cout << "Enter to listen mode..\n";

            //initialization 
            string line;
            unsigned char* cipher;
            cipher = (unsigned char*)malloc(128);  
            unsigned char* decrypted_line;
            decrypted_line = (unsigned char*)malloc(128);
            int cipher_len;  

            memset(cipher, 0, 128);
            memset(decrypted_line, 0, 128);    

            int dec_len = AES_decrypt(cipher, cipher_len, decrypted_line);
            
            for (int i = 0; i < dec_len; i++)
            {
                cout << decrypted_line[i]; 
            }
            cout << "\n";
        
            free(cipher);
            free(decrypted_line);
        
        }
        else if (strcmp("-h", argv[1]) == 0 || strcmp("-H", argv[1]) == 0)
        {       //help section
            cout << "Help..\n";
        }
        else
        {       //err
            cout << "You gave invalid parameters\nTry help -h|-H\n";
            return 1;
        }
    }
    else if (argc == 5)
    {           //sender mode
        if(strcmp("-r", argv[1]) == 0 || strcmp("-s", argv[3]) == 0)
        {
            cout << "Enter to sender mode..\n";
            
            //check file
            fstream file (argv[2]);
            if(!file.is_open())
            {
                cout << "You gave invalid file\n";
                return 1;
            }
            
            //check addr


            //initialization 
            string line;
            unsigned char* cipher;
            cipher = (unsigned char*)malloc(128);  
            unsigned char* text;
            text = (unsigned char*)malloc(128);

            while (getline(file,line))
            {              
                memset(cipher, 0, 128);                
                memset(text, 0, 128);              

                strcpy((char*)text, line.c_str());

                int cipher_len = AES_encrypt(text, line.length(), cipher);          
            }
            
            int sockfd;
            char *ip_addr, *reverse_hostname;
            struct sockaddr_in addr_con;
            int addrlen = sizeof(addr_con);
            char net_buf[NI_MAXHOST];

            int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
            if(sock < 0)
            {
                cout << "nepodarilo se inicializovat socket\n";
                pclose(sock);
                return 4;
            }


            free(cipher);
            free(text);
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
/*

 Simple echo client with input parameters

 Usage: echo-client1 <server IP address/domain name>  <server port>

 (c) Petr Matousek, 2016

 last update: Sept 2019
*/

#include<stdio.h> 
#include<stdlib.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<err.h>
#include<pwd.h>

#define BUFFER 1024              // buffer length 

int main(int argc, char *argv[])
{
  int sock, msg_size, i;
  socklen_t len;
  struct sockaddr_in local, server;
  struct hostent *servent;       // a pointer to the server addresses
  char buffer[BUFFER];
  uid_t uid;                     
  struct passwd *uname;
  

  // read program arguments
 
  if(argc != 3)                  // two parameters expected
    errx(1,"Usage: %s <address> <port>",argv[0]);
  
  memset(&server,0,sizeof(server)); // erase the server structure
  memset(&local,0,sizeof(local));   // erase the local address structure

  server.sin_family = AF_INET;                   

  // make DNS resolution of the first parameter using gethostbyname()
  if ((servent = gethostbyname(argv[1])) == NULL) // check the first parameter
    errx(1,"gethostbyname() failed\n");

  // copy the first parameter to the server.sin_addr structure
  memcpy(&server.sin_addr,servent->h_addr,servent->h_length); 

  server.sin_port = htons(atoi(argv[2]));        // server port (network byte order)
  
  if ((sock = socket(AF_INET , SOCK_STREAM , 0)) == -1)   //create a client socket
    err(1,"socket() failed\n");
  
  printf("* Socket successfully created\n");
  uid = getuid();
  uname = getpwuid (uid);
     
  // connect to the remote server
  // client port and IP address are assigned automatically by the operating system

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    err(1,"connect() failed");
     
  // obtain the local IP address and port using getsockname()
  len = sizeof(local);
  if (getsockname(sock,(struct sockaddr *) &local, &len) == -1)
    err(1,"getsockname() failed");

  printf("* Client successfully connected from %s, port %d (%d) to %s, port %d (%d)\n", inet_ntoa(local.sin_addr),ntohs(local.sin_port),local.sin_port,inet_ntoa(server.sin_addr),ntohs(server.sin_port), server.sin_port);

  strcpy(buffer,uname->pw_name);  // send a login name to the server
  
  i = write(sock,buffer,strlen(uname->pw_name));
  if (i == -1){
    err(1,"initial write() failed");
  }

  if ((i = read(sock,buffer,BUFFER)) == -1){  // read an initial string
    err(1,"initial read() failed");
  } else {
    printf("%.*s\n",i,buffer);
  }
  
  //keep communicating with server
  while((msg_size=read(STDIN_FILENO,buffer,BUFFER)) > 0) 
      // read input data from STDIN (console) until end-of-line (Enter) is pressed
      // when end-of-file (CTRL-D) is received, msg_size == 0
  { 
    i = write(sock,buffer,msg_size);             // send data to the server
    if (i == -1)                                 // check if data was sent correctly
      err(1,"write() failed");
    else if (i != msg_size)
      err(1,"write(): buffer written partially");
    
    if ((i = read(sock,buffer, BUFFER)) == -1)   // read the answer from the server
      err(1,"read() failed");
    else if (i > 0)
      printf("%.*s",i,buffer);                   // print the answer
  } 
  // reading data until end-of-file (CTRL-D)

  if (msg_size == -1)
    err(1,"reading failed");
  close(sock);
  printf("* Closing the client socket ...\n");
  return 0;
}

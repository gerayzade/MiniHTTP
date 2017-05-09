/*
  Copyright Heydar Gerayzade 2017
  
  Simple Web Server
  cmd: args: <PORT NO.> (9000 by default if no args)
  
  Credits to Linux manual pages and stackoverflow
*/

#include <stdio.h>     
#include <unistd.h>   
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>   
#include <sys/stat.h>   
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* font styles */
#define GREEN "\033[92m"
#define DARKCYAN "\033[36m"
#define BOLDGREEN "\033[1m\033[92m"
#define BOLDRED "\033[1m\033[91m"
#define RESET "\033[0m"
   
int main(int argc, char** argv) {
  int port;
  
  /* assigning value to port no. from command line / default aka 9000 */
  if (argc > 1)
    port = atoi(argv[1]);
  else 
    port = 9000;
  
  /* closing a port manually befor start */
  char * closePort = malloc(1024);
  sprintf(closePort, "fuser -k -n tcp %d", port);
  system(closePort);
  
  char * localhost;
  localhost = "127.0.0.1";
  char * webroot;
  webroot = getenv("PWD");
  
  int socketfd, newsocketfd; 
  
  /* creating an endpoint for communication */
  if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) > 0){   
    fprintf(stdout, "\n%sSocket created..%s\n", BOLDGREEN, RESET);
  } else {
    fprintf(stderr, "%sSocket create error%s\n", BOLDRED, RESET);
  }
   
  struct sockaddr_in address;
  
  address.sin_family = AF_INET;   
  address.sin_addr.s_addr = inet_addr(localhost);   
  address.sin_port = htons(port);   
  
  /* assigning the address to the socket */
  if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == 0){   
    fprintf(stdout, "%sBinding socket..%s\n\n", BOLDGREEN, RESET);
  } else {
    fprintf(stderr, "%sSocket bind error%s\n", BOLDRED, RESET);
  }   
   
  while (1) { 
    
    /* marking the socket as a passive one */
    if (listen(socketfd, 10) < 0) {   
      fprintf(stderr, "%sSocket listen error%s\n", BOLDRED, RESET);   
      exit(1);   
    }
  
    socklen_t addrlen;   
   
    /* creating a new connected socket */
    if ((newsocketfd = accept(socketfd, (struct sockaddr *) &address, &addrlen)) < 0) {   
      fprintf(stderr, "%sSocket accept error%s\n", BOLDRED, RESET);   
      exit(1);   
    }
    
    char *socketmsg = malloc(1024 * 1024); 
    
    /* receiving message from the socket */
    recv(newsocketfd, socketmsg, 1024 * 1024, 0);
    fprintf(stdout, "%s%s%s\n", DARKCYAN, socketmsg, RESET);
    
    int fd, readb, datab;
    datab = 1024 * 1024;
    char * filedata = malloc(datab);
    char path[1024];
    
    char * token[2];
    token[0] = strtok(socketmsg, " \t\n");
    
    /* looking for GET request */
    if (strncmp(token[0], "GET\0", 4) == 0) {
      token[1] = strtok(NULL, " \t");
      
      /* defining default request */
      if (strncmp(token[1], "/\0", 2) == 0)
        token[1] = "/index.php";

       strcpy(path, webroot);
       strcpy(&path[strlen(webroot)], token[1]);
       
       /* processing the requested file */
       fd = open(path, O_RDONLY);
      
       /* checking whether the requested file was found or not */
       if (fd > 0) {
         send(newsocketfd, "HTTP/1.0 200 OK\n\n", 17, 0);
         while ((readb = read(fd, filedata, datab)) > 0)
           write(newsocketfd, filedata, readb);
       } else {
         send(newsocketfd, "HTTP/1.0 404 Not Found\n\n", 24, 0);
         write(newsocketfd, "<h1>404 Not Found</h1>", 22);
       }
    }
    
    close(newsocketfd);   
  }   
  
  close(socketfd);
  
  return 0;   
}
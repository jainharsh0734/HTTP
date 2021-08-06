#include<windows.h>
#include<stdio.h>
#include<string.h>
int main()
{
 WORD   vers;
 char request[5000];
 char response[5000];
 WSADATA wsaData;
 int serverSocketDescriptor;
 int clientSocketDescriptor;
 int successCode;
 int len;
 struct sockaddr_in serverSocketInformation;
 struct sockaddr_in clientSocketInformation;
 vers=MAKEWORD(1,1);
 WSAStartup(vers,&wsaData);    //socket library Intialized
 serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
 if(serverSocketDescriptor<0)
 { 
  printf("Unable to create socket\n");   
  return 0;
 }


 serverSocketInformation.sin_family=AF_INET;
 serverSocketInformation.sin_port=htons(5050);
 serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
 successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(struct sockaddr));
 if(successCode<0)
 {
  printf("Unable to bind socket to port 5050\n");
  WSACleanup;
  return 0;
 }
 listen(serverSocketDescriptor,10);
 printf("TMServer is ready to accept request on port 5050\n");
 len=sizeof(clientSocketInformation);
 clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
 if(clientSocketDescriptor<0)
 {
  printf("Unable to accept client connection\n");
  closesocket(serverSocketDescriptor);
  WSACleanup();
  return 0;
 }
 successCode=recv(clientSocketDescriptor,request,sizeof(request),0);
 if(successCode>0)
 {
  printf("Request Arrived\n");
  printf("%s\n",request);
 }

 strcpy(response,"<!DOCTYPE HTML>");
 strcat(response,"<html lang='en'>");
 strcat(response,"<head>");
 strcat(response,"<title>Whatever</title>");
 strcat(response,"</head>");
 strcat(response,"<body>");
 strcat(response,"<h1>Welcome to harsh jain server</h1>");
 strcat(response,"</body>");
 strcat(response,"</html>");
 char header[5000];
 strcpy(header,"HTTP/1.1 200 OK\n");
 strcat(header,"Content-Type: text/html\n");
 strcat(header,"Content-Length:");
 char responseLength[11];
 sprintf(responseLength,"%d\n",strlen(response)+1);
 strcat(header,responseLength);
 strcat(header,"\n");  //a one line gap
 successCode=send(clientSocketDescriptor,header,strlen(header)+1,0);
 successCode=send(clientSocketDescriptor,response,strlen(response)+1,0);
 closesocket(clientSocketDescriptor);
 closesocket(serverSocketDescriptor);
 WSACleanup();
 return 0;
}
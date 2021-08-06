#include<tmwp>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace tmwp;
string Request::get(string name)
{
string val;
int i,e;
for(i=0;i<this->dataCount;i++)
{
for(e=0;data[i][e]!='\0' && data[i][e]!='=';e++);
if(data[i][e]!='=') continue; //back to next cycle
if(strncmp(data[i],name.c_str(),e)==0)break;
}
if(i==this->dataCount)
{
val="";
}
else
{
val=string(data[i]+(e+1));
} 
return val;
}
void Request::forward(string forwardTo)
{
this->forwardTo=forwardTo;
}
Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
this->isClosed=false;
this->headerCreated=false;
}
void Response::createHeader()
{
char header[51];
strcpy(header,"HTTP/1.1 200 OK\nContent-Type:text/html\n\n");
send(clientSocketDescriptor,header,strlen(header),0);
this->headerCreated=true;
}
void Response::write(const char *str)
{
if(str==NULL) return;
int len=strlen(str);
if(len==0) return;
if(!this->headerCreated) createHeader();
send(clientSocketDescriptor,str,len,0);
}
void Response::close()
{
if(this->isClosed)return;
closesocket(this->clientSocketDescriptor);
}

int extensionEquals(const char *left,const char *right)
{
 char a,b;
 while(*left && *right)
 {
  a=*left;
  b=*right; 
  if(a>=65 && a<=90)a+=32;
  if(b>=65 && b<=90)b+=32;
  if(a!=b)return 0;
  left++;
  right++;
 }
 return *left==*right;
}
char * getMIMEType(char *resource)
{
 char *mimeType;
 mimeType=NULL;
 int len=strlen(resource);
 if(len<4) return mimeType;
 int lastIndexOfDot=len-1;
 while(lastIndexOfDot>0 &&resource[lastIndexOfDot]!='.')lastIndexOfDot--;
 if(lastIndexOfDot==0)return mimeType;
 if(extensionEquals(resource+lastIndexOfDot+1,"html"))
 {
  mimeType=(char *)malloc(sizeof(char)*10);
  strcpy(mimeType,"text/html");
 }
 if(extensionEquals(resource+lastIndexOfDot+1,"css"))
 {
  mimeType=(char *)malloc(sizeof(char)*9);
  strcpy(mimeType,"text/css");
 }
 if(extensionEquals(resource+lastIndexOfDot+1,"js"))
 {
  mimeType=(char *)malloc(sizeof(char)*16);
  strcpy(mimeType,"text/javascript");
 }
 if(extensionEquals(resource+lastIndexOfDot+1,"jpg"))
 {
  mimeType=(char *)malloc(sizeof(char)*11);
  strcpy(mimeType,"image/jpeg");
 }
if(extensionEquals(resource+lastIndexOfDot+1,"jpeg"))
 {
  mimeType=(char *)malloc(sizeof(char)*11);
  strcpy(mimeType,"image/jpeg");
 }
 if(extensionEquals(resource+lastIndexOfDot+1,"png"))
 {
  mimeType=(char *)malloc(sizeof(char)*10);
  strcpy(mimeType,"image/png");
 }
 if(extensionEquals(resource+lastIndexOfDot+1,"ico"))
 {
  mimeType=(char *)malloc(sizeof(char)*13);
  strcpy(mimeType,"image/x-icon");
 }
 return mimeType;
}
char isClientSideResource(char *resource)
{
 int i;
 for(i=0;resource[i]!='\0'&&resource[i]!='.';i++);
 if(resource[i]=='\0')return 'N';
 return 'Y';
}
Request * parseRequest(char *bytes)
{
 char method[11];
 char  resource[1001];
 int i,j;
 for(i=0;bytes[i]!=' ';i++) method[i]=bytes[i];
 method[i]='\0';
 i+=2;

 char **data=NULL;
 int dataCount=0;
 if(strcmp(method,"GET")==0)
 { 
  //whatever?sdfsdf=xczcbnbxb&snjnqwje=sabxnbx&nbxzbxn=snasnja 
 for(j=0;bytes[i]!=' ';j++,i++)
 {
 if(bytes[i]=='?')break;
 resource[j]=bytes[i];
 }
 resource[j]='\0';
 if(bytes[i]=='?')
 { 
 i++;
 int si=i;
 dataCount=0;
 while(bytes[i]!=' ')
 {
 if(bytes[i]=='&')dataCount++;
 i++;
 }
 dataCount++;
 data=(char **)malloc(sizeof(char *)*dataCount);
 int *pc=(int *)malloc(sizeof(int)*dataCount);
 i=si; 
 int j=0;
 while(bytes[i]==' ')
 {
 if(bytes[i]=='&')
 {
 pc[j]=i;
 j++;
 }
 i++;
 }
 pc[j]=i;
 i=si;
 j=0;
 int howManyToPick;
 while(j<dataCount)
 {
 howManyToPick=pc[j]-i;
 data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
 strncpy(data[j],bytes+i,howManyToPick);
 data[j][howManyToPick]='\0';
 i=pc[j]+1;
 j++;
 }
 }
 }//method is of GET type
 printf("Request arrived for %s\n",resource);
// Request *request=(Request *)malloc(sizeof(Request));
Request *request=new Request;//vvvvv important change.
 request->dataCount=dataCount;
 request->data=data;
 request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
 strcpy(request->method,method);
 if(resource[0]=='\0')
 {
 request->resource=NULL;
 request->isClientSideTechnologyResource='Y';
 request->mimeType=NULL;
 }
 else
 {
 request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
 strcpy(request->resource,resource);
 request->isClientSideTechnologyResource=isClientSideResource(resource);
 request->mimeType=getMIMEType(resource);
 }
 return request;
}
TMWebProjector::TMWebProjector(int portNumber)
{
 this->portNumber=portNumber;
 this->url=NULL; 
 this->ptrOnRequest=NULL;
}
TMWebProjector::~TMWebProjector()
{
 if(this->url) delete [] this->url;
}
void TMWebProjector::onRequest(const char *url,void (*ptrOnRequest)(Request &,Response &))
{ 
 if(this->url) delete []this->url;
 this->url=NULL;
 this->ptrOnRequest=NULL;
 if(url==NULL || ptrOnRequest==NULL)return;
 this->url=new char[strlen(url)+1];
 strcpy(this->url,url);
 this->ptrOnRequest=ptrOnRequest;
}
void TMWebProjector::start()
{
 FILE *f;
 int length;
 char g;
 int i,rc;
 WORD   ver;
 char requestBuffer[8192]; //8192 1024*8
 char responseBuffer[1024]; //A cunk of 1024 + 1 space for string terminator
 WSADATA wsaData;
 int bytesExtracted;
 int serverSocketDescriptor;
 int clientSocketDescriptor;
 int successCode;
 int len;
 struct sockaddr_in serverSocketInformation;
 struct sockaddr_in clientSocketInformation;
 ver=MAKEWORD(1,1);
 WSAStartup(ver,&wsaData);    //socket library Intialized
 serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
 if(serverSocketDescriptor<0)
 { 
  printf("Unable to create socket\n");   
  return;
 }


 serverSocketInformation.sin_family=AF_INET;
 serverSocketInformation.sin_port=htons(this->portNumber);
 serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
 successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(struct sockaddr));
 char message[101];
 if(successCode<0)
 {
 sprintf(message,"Unable to bind socket to port %d",this->portNumber);
 printf("%s\n",message);
 WSACleanup();
 return;
 }
 listen(serverSocketDescriptor,10);
 len=sizeof(clientSocketInformation);
 while(1)//loop to accept conncection
 {
 sprintf(message,"TMServer is ready to accept request on port %d",this->portNumber);
 printf("%s\n",message);
 clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
 if(clientSocketDescriptor<0)
 {
  printf("Unable to accept client connection\n");
  closesocket(serverSocketDescriptor);
  WSACleanup();
  return;
 }

 bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
 if(bytesExtracted<0)
 {
  //what to do yet not been decided
 }else
 if(bytesExtracted==0)
 {
 //what to do yet to be  decided
 }
 else
 {
 requestBuffer[bytesExtracted]='\0'; 
 Request *request=parseRequest(requestBuffer);  
 while(1)  //infinite loop to enable the forwarding feature
 {
 if(request->isClientSideTechnologyResource=='Y')
 {
 if(request->resource==NULL)
 { 
 f=fopen("index.html","rb");
 if(f!=NULL) printf("Sending index.html\n");
 if(f==NULL)
 {
 f=fopen("index.htm","rb");
 if(f!=NULL) printf("Sending index.htm \n");
 }
 if(f==NULL)
 {
 strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:163\nConnection:  close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource/not found</h2></body></html>");
 send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
 printf("Sending 404 page\n");
 closesocket(clientSocketDescriptor);
 break; //introduce beacuse of forwarding feature
 }
 else
 { 
 fseek(f,0,2);
 length=ftell(f);
 fseek(f,0,0);
 sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:  close\n\n",length);
 send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
 i=0;
 while(i<length)
 {
 rc=length-i;
 if(rc>1024)rc=1024;
 fread(&responseBuffer,1024,1,f);
 send(clientSocketDescriptor,responseBuffer,rc,0);
 i+=1024;
 }
 fclose(f);
 closesocket(clientSocketDescriptor);
 break; //introduce beacuse of forwarding feature
 }
 }
 else 
 { 
 f=fopen(request->resource,"rb");
 if(f!=NULL) printf("Sending %s\n",request->resource);
 if(f==NULL)
 {
 //something need to be done over here
 printf("Sending 404 page\n");
 char tmp[501];
 sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
 sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:  close\n\n",strlen(tmp));
 strcat(responseBuffer,tmp);
 send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
 break; //introduce beacuse of forwarding feature
 }
 else
 { 
 fseek(f,0,2);
 length=ftell(f);
 fseek(f,0,0);
 sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection:  close\n\n",request->mimeType,length);
 send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
 i=0;
 while(i<length)
 {
 rc=length-i;
 if(rc>1024)rc=1024;
 fread(&responseBuffer,1024,1,f);
 send(clientSocketDescriptor,responseBuffer,rc,0);
 i+=1024;
 }
 fclose(f);
 closesocket(clientSocketDescriptor);
 break; //introduce beacuse of forwarding feature
 }
 }
 }
 else
 {
  //more changes reqires in context to server side resource
 if(this->url==NULL || this->ptrOnRequest==NULL) 
 {
 printf("Sending 404 page\n");
 char tmp[501];
 sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
 sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:  close\n\n",strlen(tmp));
 strcat(responseBuffer,tmp);
 send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0); 
 break; //introduce beacuse of forwarding feature
 }
 else
 {
 int ii=0;
 if(this->url[0]=='/')ii=1;
 if(strcmp(this->url+ii,request->resource)==0)
 {
 Response response(clientSocketDescriptor);
 this->ptrOnRequest(*request,response);

 if(request->forwardTo.length()>0)
 {
 free(request->resource);
 request->resource=(char *)malloc((sizeof(char)*request->forwardTo.length())+1);
 strcpy(request->resource,request->forwardTo.c_str());
 request->isClientSideTechnologyResource=isClientSideResource(request->resource);
 request->mimeType=getMIMEType(request->resource);
 continue;
 //change the resource part against request and reprocess the request
 }

 if(request->data!=NULL)
 {
 for(int k=0;k<request->dataCount;k++) free(request->data[k]);
 free(request->data);
 }
 break; //intoduced beacuse of the forwarding feature
 }
 else
 {
 printf("Sending 404 page\n");
 char tmp[501];
 sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM WEB PROJECTOR</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
 sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:  close\n\n",strlen(tmp));
 strcat(responseBuffer,tmp);
 send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0); 
 closesocket(clientSocketDescriptor);
 break;//introduced because of the forwarding feature
 }
 }
 }
 
 }// the infinite loop introducedd because of the forwarding feature ends here
}
}//the infinite loop related to accept method ends here
/* if(successCode>0)
 {
  printf("Response Sent\n");
 }
 else
 {
  printf("Unable to send response\n");
 }
*/
 closesocket(clientSocketDescriptor);
 closesocket(serverSocketDescriptor);
 WSACleanup();
 return;
}
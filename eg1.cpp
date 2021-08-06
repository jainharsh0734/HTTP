#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
/*void doSomething(int dataCount,char *data[])
{
 cout<<"great,this code got executed on server side"<<endl;
 if(dataCount>0) cout<<"Data that Arrived"<<endl;
 for(int r=0;r<dataCount;r++)
 { 
 cout<<data[r]<<endl;
 } 
}
*/
void doSomething(Request &request,Response &response)
{
 String n=request.get("nm");
 String c=request.get("ct");
 response.write("<!DOCTYPE HTML>");
 response.write("<HTML lang='en'>");
 response.write("<head>");
 response.write("<TITLE>Whatever</title>");
 response.write("<meta charset='utf-8'>");
 response.write("</head>");
 response.write("<body>");
 response.write("HELLO");
 response.write(n);
 response.write("<BR>");
 response.write("Data Saved");
 response.write("</body>");
 response.write("</html>"); 
}
int main()
{
 TMWebProjector server(8080);
 server.onRequest("/register",doSomething);
 server.start();
 return 0;
}
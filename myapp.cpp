#include<tmwp>
#include<iostream>
using namespace std;
using namespace tmwp;
void addStud(Request &request,Response &response)
{
 
}
void getAllStudents(Request &request,Response &response)
{
}
void sendStudentEditForm(Request &request,Response &response)
{
/*
<!DOCTYPE html>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Harsh Jain School,Ujjain<title>
</head>
<body>
Student (Edit Module)
<form method='get' action='updateStudent'>
Roll Number:101
<input type='hidden' id='rl' name='rl' value='101'><br>
Name <input type='text' id='nm' name='nm' value='Sameer'><br>
<button type='submit'>Update</button>
</form>
<a href='index.html'>Home</a><br>
</body>
</html>
*/
}
void updateStudentData(Request &request,Response &response)
{
/*
<!DOCTYPE html>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Harsh Jain School,Ujjain<title>
</head>
<body>
Student (Edit Module)
<h3>Student Updated</h3>
<form method='get' action='index.html'>
<button type='submit'>OK</button>
</form>
</body>
</html>
*/
}
void sendStudentDeleteConfirmationsForm(Request &request,Response &response)
{
/*
<!DOCTYPE html>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Harsh Jain School,Ujjain<title>
</head>
<body>
Student (Delete Module)
<form method='get' action='DeleteStudent'>
Roll Number:101
<input type='hidden' id='rl' name='rl' value='101'><br>
Name: Sameer<br>
<button type='submit'>Delete</button>
</form>
<a href='index.html'>Home</a><br>
</body>
</html>
*/
}
void deleteStudentData(Request &request,Response &response)
{
/*
<!DOCTYPE html>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Harsh Jain School,Ujjain<title>
</head>
<body>
Student (Delete Module)
<h3>Student Deleted</h3>
<form method='get' action='index.html'>
<button type='submit'>OK</button>
</form>
</body>
</html>
*/
}






int main()
{
 TMWebProjector server(8080);
 server.onRequest("/addStudent",addStud);
 server.onRequest("/getStudents",getAllStudents);
 server.onRequest("/editStudents",sendStudentEditForm);
 server.onRequest("/updateStudent",updateStudentData);
 server.onRequest("/confirmDeleteStudent",sendStudentDeleteConfirmationsForm);
 server.onRequest("/deleteStudent",deleteStudentData);
 server.start();
 return 0;
}
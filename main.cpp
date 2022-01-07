#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std; 

const char* hostname  = "localhost";
const char* username  = "root";
const char* password = "aaronsy10";
const char* database = "sentibot";
unsigned int port = 3306;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connectdatabase(){
	MYSQL*conn;
	conn = mysql_init(0);
	conn = mysql_real_connect (conn, hostname, username, password, database, port, unixsocket, clientflag);
	if (conn){
		cout<<"Connected to database"<<endl;
		return conn;
	}
	else{
		cout<<"Failed to connect to database"<<endl;
		return conn;
	}
}

insert (MYSQL* conn){
	int qstate = 0;
	stringstream ss;
	string userQuerydb, botResponsedb;
	cout<<"Enter userQuery";
	cin>>userQuerydb;
	cout<<"Enter botResponse";
	cin>>botResponsedb;
	ss <<"INSERT INTO convo (userQuery, botResponse) VALUES ('"+userQuerydb+"','"+botResponsedb+"')";
	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn,q);
	if(qstate == 0){
		cout<<"Record inserted"<<endl;
	}
	else{
		cout<<"Failed to insert"<<endl;	
	}
}

display(MYSQL* conn,string userChat){
	MYSQL_ROW row;
	MYSQL_RES* res;
	//string userChat;
	string choice;
	int responseCounter;
	//cout<<"User: ";
	//getline(cin>>ws, userChat);
	stringstream ss;
	ss<<"SELECT botResponse FROM convo WHERE userQuery ='"+userChat+"'";
	string query = ss.str();
	const char* q = query.c_str();

	if (conn){
		int qstate = mysql_query(conn, q);
		if(!qstate){
			res = mysql_store_result (conn);
			int count = mysql_num_fields(res);
			while (row = mysql_fetch_row(res)){
				for(int i=0;i<count;i++){
					responseCounter = i;
				}
				cout<<"SentiBOT: "<<row[responseCounter];
				cout<<endl;
			}
		}
	}
	else{
		cout<<"Failed to fetch";
	}
}

int main()
{
	string userChat;
	string choice;
	int sessionCounter = 1;
	MYSQL* conn = connectdatabase();
	cout<<"Hi I am SentiBOT. How may I help you?\n";
	//insert(conn);
	while (sessionCounter ==1)
	{
		cout<<"User: ";
		getline(cin>>ws, userChat);
		if(userChat=="exit"||userChat=="Exit"||userChat=="EXIT"){
			cout<<"Are you sure?\n";
			cout<< "[1]: Yes" << endl;
			cout<< "[0]: No" << endl;
			cout<<"User: "; 
			cin>>choice;
			cin.ignore(100, '\n');
				if (choice == "1" || choice =="Yes"){
					cout<<"Thank you for using SentiBOT. Goodbye :D";
					sessionCounter==0;
					break;	
				}
		}
		else{
			display(conn,userChat);
		}
		

	}
	
	return 0;
}

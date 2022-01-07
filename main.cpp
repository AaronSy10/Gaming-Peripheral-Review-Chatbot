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
	string decision, adminPassword; //decision to add
	cout<<"Sorry I didn't quite get that. Would you like to add it to my vocabulary?"<<endl;
	cout<< "[1]: Yes" << endl;
	cout<< "[0]: No" << endl;
	cout<< "User: ";
	getline(cin>>ws, decision);
	if (decision == "Yes"||decision == "yes"||decision == "YES"||decision == "1"){
		cout<< "Enter admin password: ";
		getline(cin>>ws, adminPassword);
		if (adminPassword == "1234"){
			stringstream ss;
			string userQuerydb, botResponsedb;
			cout<<"\nEnter userQuery: ";
			getline(cin>>ws, userQuerydb);
			cout<<"Enter botResponse: ";
			getline(cin>>ws, botResponsedb);
			ss <<"INSERT INTO convo (userQuery, botResponse) VALUES ('"+userQuerydb+"','"+botResponsedb+"')";
			string query = ss.str();
			const char* q = query.c_str();
			qstate = mysql_query(conn,q);
			if(qstate == 0){
				cout<<"Conversation added"<<endl<<endl;
			}
			else{
				cout<<"Failed to insert"<<endl;	
			}
		}
		else{
			cout<<"SentiBOT: Wrong Password!"<<endl<<endl;
		}		
	}
	else{
		cout<<"SentiBOT: Okay!"<<endl<<endl;
	}
}

display(MYSQL* conn,string userChat, int adder){
	MYSQL_ROW row;
	MYSQL_RES* res;
	int responseCounter; //response by bot
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
				adder=1;
				cout<<endl;
			}
		if (adder==0){
			insert(conn);
			adder=1;
		}
		}
	}
	else{
		cout<<"Failed to fetch";
	}
}

int main()
{
	string userChat; //user input
	string choice; //choice to exit
	int sessionCounter = 1; //program running
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
				if (choice == "1" || choice =="Yes" || choice =="yes"){
					cout<<"Thank you for using SentiBOT. Goodbye :D";
					sessionCounter==0;
					break;	
				}
		}
		else{
			display(conn,userChat,0);
		}
		

	}
	
	return 0;
}

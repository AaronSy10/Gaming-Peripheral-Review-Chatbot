#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
#include <bits/stdc++.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std; 

const char* hostname  = "localhost";
const char* username  = "root";
const char* password = "RavidPhelpsR212000";
const char* database = "sentibot";
unsigned int port = 3306;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connectdatabase(){
	MYSQL*conn;
	conn = mysql_init(0);
	conn = mysql_real_connect (conn, hostname, username, password, database, port, unixsocket, clientflag);
	//connecting to database using credentials
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

int checkPositive(MYSQL* conn,string userChat){
	MYSQL_ROW row;
	MYSQL_RES* res;
	int result;
	stringstream ss;
	ss<<"SELECT COUNT(*) FROM sentibot.positive WHERE positiveword='"+userChat+"'";
	string query = ss.str();
	const char* q = query.c_str();

	if (conn){
		int qstate = mysql_query(conn, q); //executes query
		res = mysql_store_result (conn);   //stores result 
		row = mysql_fetch_row(res);		   //gets value of row
		stringstream toint(row[0]);		   //convert from char* to int
		toint >> result;				   //puts int in result var
		return result;					   //returns result of analysis to main
	}
	else{
		cout<<"Failed to fetch";
	}
}


int checkNegative(MYSQL* conn,string userChat){
	MYSQL_ROW row;
	MYSQL_RES* res;
	int result;
	stringstream ss;
	ss<<"SELECT COUNT(*) FROM sentibot.negative WHERE negativeword='"+userChat+"'";
	string query = ss.str();
	const char* q = query.c_str();

	if (conn){
		int qstate = mysql_query(conn, q); //executes query
		res = mysql_store_result (conn);   //stores result 
		row = mysql_fetch_row(res);		   //gets value of row
		stringstream toint(row[0]);		   //convert from char* to int
		toint >> result;				   //puts int in result var
		return result;					   //returns result of analysis to main
	}
	else{
		cout<<"Failed to fetch";
	}
}


int wordCount(string str){
	stringstream s(str);
	string word;
	int count = 0;
	while (s >> word)
		count ++;
	return count;
}

int cleanStatement(MYSQL* conn, string text){   //to use function cleanStatement(connection, string to be cleaned)
	
	//remove punctuations
	for (int i = 0, len = text.size(); i < len; i++)
    {
        if (ispunct(text[i]))
        {
            text.erase(i--, 1);
            len = text.size();
        }
        else if (isdigit(text[i]))
        {
            text.erase(i--, 1);
            len = text.size();
        }
    }
    
    //string to lower case
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    
    //putting all words into an array
    int numWords = 0;
    numWords = wordCount(text);
    
	istringstream ss(text);
	int x = 0;
	string words[numWords]= "";
	int rating = 0, pos = 0 , neg = 0;
	cout<<"SentiBOT: Currently checking word rating..."<<endl;
	while (ss >> words[x]){
		pos = checkPositive(conn, words[x]);
		neg = checkNegative(conn, words[x]);
		rating = rating + pos - neg;
		x++;
	}
	return rating;
}

int main()
{
	string userChat; //user input
	string choice; //choice to exit
	int sessionCounter = 1; //program running
	MYSQL* conn = connectdatabase();
	cout<<"Hi I am SentiBOT. How may I help you?"<<endl;
	cout<<"If you want assistance with product reviews type !help"<<endl;
	while (sessionCounter ==1) //run code while program is open
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
					cout<<"Thank you for using SentiBOT. Goodbye :D"<<endl;
					sessionCounter==0; //close program
					break;	
				}
		}
		else if (userChat == "review"){
			cout<<"SentiBOT: Please input text to be analyzed."<< endl << "User: ";
			getline(cin>>ws, userChat);
			cin.ignore(100, '\n');
			int rate;
			cout<<"SentiBOT: I'm analyzing the statement, it might take a minute..."<<endl;
			rate = cleanStatement(conn,userChat);
			cout<<"SentiBOT: Thanks for leaving a review";
			//increment respective column and product depending on the review if it was positive or negative
	
		}
		else if(userChat=="!help" ||userChat=="!Help"||userChat=="!HELP"){
			cout<<"Hi I am SentiBOT. A chatbot designed to assist you with your product reviews."<<endl;
			cout<<endl<<"For reviewing our RATT mouse please type RATT review or RATT"<<endl;
			cout<<"For reviewing our KIBU keyboard please type KIBU review or KIBU"<<endl<<endl;
		}
		else if (userChat=="RATT review" || userChat=="RATT"){
			cout<<"What can you say about our RATT mouse?"<<endl;
			getline(cin>>ws, userChat);
			int rate;
			cout<<"SentiBOT: I'm analyzing the statement, it might take a minute..."<<endl;
			rate = cleanStatement(conn,userChat);
			cout<<rate<<endl;//need tanggalin for feedback lang
			if (rate>=1){
				cout<<"SentiBOT: Positive Review"<<endl; //need tanggalin for feedback lang 
				//INCREMENT POSITIVE REVIEW ON SUMMARY DATABASE
			}
			else if(rate<=-1){
				cout<<"SentiBOT: Negative Review"<<endl;//need tanggalin for feedback lang
				//INCREMENT NEGATIVE REVIEW ON SUMMARY DATABASE
			}
			else {
				cout<<"SentiBOT: Neutral Review"<<endl;//need tanggalin for feedback lang
				//INCREMENT NEUTRAL REVIEW ON SUMMARY DATABASE
			}
			//NEED ILAGAY SA TABLE YUNG RESULT NG REVIEW
			cout<<"SentiBOT: Thanks for leaving a review on our RATT mouse. Would you like to keep chatting?(yes/no)"<<endl;
			cout<<"User: ";
			getline(cin>>ws,choice);
			if (0 == strcasecmp(&choice[0],"yes")){
				sessionCounter==1;
				cout<<"SentiBOT: What's up? "<<endl;
			}
			else if (0 == strcasecmp(&choice[0],"no")){
				sessionCounter==0;
				cout<<"SentiBOT: Thanks for chatting with me! Hope to see you soon!"<<endl;
				return 0;
			}
			
		}
		else if (userChat=="KIBU review" || userChat=="KIBU"){
			cout<<"What can you say about our KIBU keyboard?"<<endl;
			getline(cin>>ws, userChat);
			int rate;
			cout<<"SentiBOT: I'm analyzing the statement, it might take a minute..."<<endl;
			rate = cleanStatement(conn,userChat);
			cout<<rate<<endl;//need tanggalin for feedback lang
			if (rate>=1){
				cout<<"SentiBOT: Positive Review"<<endl;//need tanggalin for feedback lang
				//INCREMENT POSITIVE REVIEW ON SUMMARY DATABASE
			}
			else if(rate<=-1){
				cout<<"SentiBOT: Negative Review"<<endl;//need tanggalin for feedback lang
				//INCREMENT NEGATIVE REVIEW ON SUMMARY DATABASE
			}
			else {
				cout<<"SentiBOT: Neutral Review"<<endl;//need tanggalin for feedback lang
				//INCREMENT NEUTRAL REVIEW ON SUMMARY DATABASE
			}
			//NEED ILAGAY SA TABLE YUNG RESULT NG REVIEW
			cout<<"SentiBOT: Thanks for leaving a review on our KIBU keyboard. Would you like to keep chatting?(yes/no)"<<endl;
			cout<<"User: ";
			getline(cin>>ws,choice);
			if (0 == strcasecmp(&choice[0],"yes")){
				sessionCounter==1;
				cout<<"SentiBOT: What's up? "<<endl;
			}
			else if (0 == strcasecmp(&choice[0],"no")){
				sessionCounter==0;
				cout<<"SentiBOT: Thanks for chatting with me! Hope to see you soon!"<<endl;
				return 0;
			}
		}
		else{
			display(conn,userChat,0); //normal conversation with user
		}
		

	}
	cout<<"SentiBOT: Thanks for chatting with me! Hope to see you soon!"<<endl;
	return 0;
}

#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
#include <bits/stdc++.h>
#include <conio.h>
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std; 

const char* hostname  = "localhost";
const char* username  = "root";
const char* password = "aaronsy10";
const char* database = "sentibot";
unsigned int port = 3306;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connectdatabase(){ //connect to database
	MYSQL*conn;
	conn = mysql_init(0);
	conn = mysql_real_connect (conn, hostname, username, password, database, port, unixsocket, clientflag);	//connecting to database using credentials

	if (conn){
		cout<<"Peri: Hi I am Peri. How may I help you?"<<endl;
		cout<<"Peri: If you want assistance with our peripheral products type !help"<<endl<<endl;
		return conn;
	}
	else{
		cout<<"Failed to connect to database"<<endl;
		return conn;
	}
}

insert (MYSQL* conn){ //insert conversation into convo database
	int qstate = 0;
	string decision, adminPassword; //decision to add
	repeat:
	cout<<"Peri: Sorry I didn't quite get that. Would you like to teach me the appropriate response? (yes/no)"<<endl;
	cout<< "User: ";
	getline(cin>>ws, decision);
	if (0 == strcasecmp(&decision[0],"yes")){
		int tries = 0,displaytries;
		    char ch;
			verification:
			cout<<"Peri: Please input Verification code"<<endl;
		    cout<<"User: ";
		    string pass = "";
			ch = _getch();
			while(ch!=13)
			{
				pass.push_back(ch);
				cout<<'*';
				ch = _getch();
			}
			if (pass == "1234" && tries <=3)
			{
				
				stringstream ss;
				string userQuerydb="", botResponsedb="";
				cout<<"\nEnter User Input: ";
				getline(cin>>ws, userQuerydb);
				cout<<"Enter Bot Response: ";
				getline(cin>>ws, botResponsedb);
				ss <<"INSERT INTO convo (userQuery, botResponse) VALUES ('"+userQuerydb+"','"+botResponsedb+"')";
				string query = ss.str();
				const char* q = query.c_str();
				confirmadd:
				cout<<"Peri: Are you sure about adding this response?(yes/no)"<<endl<<"User: ";
				getline(cin>>ws, decision);
				if(0 == strcasecmp(&decision[0],"yes")){
					qstate = mysql_query(conn,q);
					if(qstate == 0){
						cout<<"Conversation added"<<endl<<endl;
					}
					else{
						cout<<"Failed to insert"<<endl;	
					}
				}
				else if(0 == strcasecmp(&decision[0],"no")){
					cout<<"Peri: Alright! How else can I help?"<<endl;
				}
				else{
					cout<<"Peri: Sorry, I didn't understand that."<<endl;
					goto confirmadd;
				}
				
			}
			else if (tries == 3)
			{
				cout<<endl<<"Peri: You have entered the maximum attempts, you can try teaching me that again by asking the same question."<<endl;
			}
			else
			{
				displaytries = 3-tries;
				cout <<endl<< "Peri: Sorry the verification code you entered is incorrect. You have "<<displaytries<<" tries left."<<endl;
				tries++;
				goto verification;
			}
	}
	else if(0 == strcasecmp(&decision[0],"no")){
		cout<<"Peri: Alright! How else can I help?"<<endl;
	}
	else{
		goto repeat;
	}
}

display(MYSQL* conn,string userChat, int adder){ //displaying bot response to userquery
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
				cout<<"Peri: "<<row[responseCounter];
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

showReviewRATT(MYSQL* conn){
	MYSQL_ROW row;
	MYSQL_RES* res;
	stringstream ss;
	ss<<"SELECT * FROM sentibot.reviewdata WHERE id = 1";
	string query = ss.str();
	const char* q = query.c_str();
	int qstate = mysql_query(conn, q); 
	res = mysql_store_result (conn);    
	row = mysql_fetch_row(res);	
	cout<<"Product: RATT Mouse"<<endl;
	cout<<"Review Data:"<<endl<<endl;
	cout<<"Positive Reviews\tNegative Reviews\tNeutral Reviews"<<endl;
	for(int i=1;i<4;i++){
		cout<<row[i]<<"\t\t\t";
	}
	cout<<endl;	
}

showReviewKIBU(MYSQL* conn){
	MYSQL_ROW row;
	MYSQL_RES* res;
	stringstream ss;
	ss<<"SELECT * FROM sentibot.reviewdata WHERE id = 1";
	string query = ss.str();
	const char* q = query.c_str();
	int qstate = mysql_query(conn, q); 
	res = mysql_store_result (conn);    
	row = mysql_fetch_row(res);	
	cout<<"Product: KIBU Keyboard"<<endl;
	cout<<"Review Data:"<<endl;
	cout<<"Positive Reviews\tNegative Reviews\tNeutral Reviews"<<endl;
	for(int i=4;i<7;i++){
		cout<<row[i]<<"\t\t\t";
	}
	cout<<endl;	
}

int checkPositive(MYSQL* conn,string userChat){ //checking if word is a positive word
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

int checkNegative(MYSQL* conn,string userChat){ //checking if word is a negative word
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

int wordCount(string str){ //couting the words in a string
	stringstream s(str);
	string word;
	int count = 0;
	while (s >> word)
		count ++;
	return count;
}

int cleanStatement(MYSQL* conn, string text){   //to use function cleanStatement(connection, string to be cleaned) //cleaning the statement for user rating
	
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
	while (ss >> words[x]){
		pos = checkPositive(conn, words[x]);
		neg = checkNegative(conn, words[x]);
		rating = rating + pos - neg;
		x++;
	}
	return rating;
}

incrementReview(MYSQL* conn,string product){ //increment the value of KIBUKeyboardNeutralReviews in reviewdata database
	int qstate = 0;
	stringstream ss;
	ss <<"UPDATE reviewdata SET "+product+" = "+ product+" + 1 WHERE id = 1";
	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn,q);
	if(qstate == 0){
		cout<<"Peri: Your review was counted into our database."<<endl<<endl;
		}
	else{
		cout<<"Peri: Your review was not counted into our database."<<endl;	
		}
}


int main()
{
	string userChat; //user input
	string choice; //choice to exit
	int sessionCounter = 1; //program running
	MYSQL* conn = connectdatabase();
	
	while (sessionCounter ==1) //run code while program is open
	{
		userStart:
		cout<<"User: ";
		getline(cin>>ws, userChat);
		if(0 == strcasecmp(&userChat[0],"exit")){
			cout<<"Peri: Are you sure? (yes/no)"<<endl;
			cout<<"User: "; 
			cin>>choice;
			cin.ignore(100, '\n');
				if (0 == strcasecmp(&choice[0],"yes")){
					sessionCounter==0; //close program
					break;	
				}
		}
		else if(0 == strcasecmp(&userChat[0],"!help")){
			cout<<"Peri: Hi I am Peri. A chatbot designed to assist you with Blazer gaming peripherals."<<endl;
			cout<<endl<<"-To review our RATT mouse, please type RATT review or RATT"<<endl;
			cout<<"-To review our KIBU keyboard, please type KIBU review or KIBU"<<endl;
			cout<<"-To add a response for Peri, please type the user input and provide a verification code to add to our conversation database."<<endl;
			cout<<"-To display a summary of reviews for our products, please type 'show RATT reviews' or 'show KIBU reviews'"<<endl;
			cout<<"-To review our KIBU keyboard, please type KIBU review or KIBU"<<endl;
			cout<<"-For a simple introduction of our products, you can ask 'what is the RATT mouse?' or 'what is the KIBU keyboard?' to know more"<<endl<<endl;
		}
		else if (0 == strcasecmp(&userChat[0],"show ratt reviews")){
			showReviewRATT(conn);
		}
		else if (0 == strcasecmp(&userChat[0],"show kibu reviews")){
			showReviewKIBU(conn);
		}
		else if (userChat=="RATT review" || userChat=="RATT"){
			cout<<"Peri: What can you say about our RATT mouse?"<<endl;
			cout<<"User: ";
			getline(cin>>ws, userChat);
			int rate;
			rate = cleanStatement(conn,userChat);
			if (rate>=1){
				cout<<"Peri: Thank you for giving this product a positive review."<<endl; //chatbot feedback
				incrementReview(conn, "RATTMousePositiveReviews" ); //increment positive review on reviewdata database
				
			}
			else if(rate<=-1){
				cout<<"Peri: Sorry for that. We will try to improve our product to serve you better."<<endl; //chatbot feedback
				incrementReview(conn, "RATTMouseNegativeReviews" ); //increment negative review on reviewdata database
			}
			else {
				cout<<"Peri: Thank you for your review. We will try to improve our product to serve you better."<<endl; //chatbot feedback
				incrementReview(conn, "RATTMouseNeutralReviews" ); //increment neutral review on reviewdata database
			}
			cout<<"Peri: Thanks for leaving a review on our RATT mouse. Would you like to keep chatting? (yes/no)"<<endl;//convo after review
			repeat1: //return here for unrecognized user input
			cout<<"User: ";
			getline(cin>>ws,choice);
			if (0 == strcasecmp(&choice[0],"yes")){
				sessionCounter==1;
				cout<<"Peri: What's up? "<<endl;
				goto userStart; //go to user input at the start
			}
			else if (0 == strcasecmp(&choice[0],"no")){
				sessionCounter==0;
				cout<<"Peri: Thanks for chatting with me! Hope to see you soon!"<<endl;
				return 0;
			}
			else{
				cout<<"Peri: Sorry I didn't get that. Would you like to keep chatting? (yes/no)"<<endl;
				goto repeat1;
			}
			
		}
		else if (userChat=="KIBU review" || userChat=="KIBU"){
			cout<<"Peri: What can you say about our KIBU keyboard?"<<endl;
			cout<<"User: ";
			getline(cin>>ws, userChat);
			int rate;
			rate = cleanStatement(conn,userChat);
			if (rate>=1){
				cout<<"Peri: Thank you for giving this product a positive review."<<endl; //chatbot feedback
				incrementReview(conn, "KIBUKeyboardPositiveReviews" );//increment positive review on reviewdata database
			}
			else if(rate<=-1){
				cout<<"Peri: Sorry for that. We will try to improve our product to serve you better."<<endl; //chatbot feedback
				incrementReview(conn, "KIBUKeyboardNegativeReviews" ); //increment negative review on reviewdata database
			}
			else {
				cout<<"Peri: Thank you for your review. We will try to improve our product to serve you better."<<endl; //chatbot feedback
				incrementReview(conn, "KIBUKeyboardNeutralReviews" ); //increment neutral review on reviewdata database
			}
			
			cout<<"Peri: Thanks for leaving a review on our KIBU keyboard. Would you like to keep chatting? (yes/no)"<<endl; //convo after review
			repeat2: //return here for unrecognized user input
			cout<<"User: ";
			getline(cin>>ws,choice);
			if (0 == strcasecmp(&choice[0],"yes")){
				sessionCounter==1;
				cout<<"Peri: What's up? "<<endl;
				cout<<"Peri: For assistance with product reviews type !help"<<endl;
			}
			else if (0 == strcasecmp(&choice[0],"no")){
				sessionCounter==0;
				cout<<"Peri: Thanks for chatting with me! Hope to see you soon!"<<endl;
				return 0;
			}
			else{
				cout<<"Peri: Sorry I didn't get that. Would you like to keep chatting? (yes/no)"<<endl;
				goto repeat2;
			}
		}
		else{
			display(conn,userChat,0); //normal conversation with user
		}
		

	}
	cout<<"Peri: Thanks for chatting with me! Hope to see you soon!"<<endl;
	return 0;
}

/**
    * Code By: Ritik Agarwal
    * Roll Number: 2018202017
    * Subject: Operating System- Assignment Mini Torrent
**/

#include "Client.h"

int upload_port;
string tracker_ip_1;
int tracker_port_1;
string tracker_ip_2;
int tracker_port_2;
string logfile_name;

map<string, vector<string>> seeder_list;
int chunksize;
char hashString[41];
string hash_value;

string client_ip;
int listen_port;

int main(int argc, char *argv[])
{

	chunksize = 524288;
	if (argv[1] != NULL)
		Initialize_Client_Params(argv);

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

/* 	thread t(handleClient,clientSocket);
	t.detach(); */


	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("1. Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("2. Connected to Server %s %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

	menu_driven_interface();
	string input;
   	while (true)
    {
       getline(cin, input);
       vector<string> input_commands = split(input,' ');
       if (input_commands[0] == "share")
       {
           share_to_tracker(clientSocket,input_commands[0],input_commands[1],input_commands[2]);
       }
       else if (input_commands[0] == "remove")
       {
           remove_shared_file(clientSocket,input_commands[0],input_commands[1]);
       }
       else if (input_commands[0] == "get")
       {
           get_file(clientSocket,input_commands[0],input_commands[1], input_commands[2]);
       }

	   cout << "just a check to see " << "\n";
	   int x;
	   cin >> x;
       recv(clientSocket,buffer,20,0);
	   cout << buffer;
    }

	return 0;
}

void handleClient(int socketid)
{

}

void send_message(int clientSocket,const char* message,int size)
{
	cout << "Client " << message << "\n";
    if(send(clientSocket,message,1024,0)<0)
		cout << "Error in sending " << "\n";
}

void receive_message(int clientSocket,const char* message,int size)
{
	cout << "Client Recv " << message << "\n";
    recv(clientSocket,(char *)message,size,0);
}

void logmessage(char *message)

{
    FILE *fptr;
    fptr=fopen(logfile_name.c_str(),"r");
    if(fptr==NULL)
        fptr=fopen(logfile_name.c_str(),"w");
    else
        fptr=fopen(logfile_name.c_str(),"a");
    if(fptr==NULL)
        cout <<"log file not opened " << "\n";
    fprintf(fptr,"%s\n",message);
    fclose(fptr);
}


void Initialize_Client_Params(char *argv[])
{

	string argv1 = argv[1];
	string argv2 = argv[2];
	string argv3 = argv[3];

	int t1 = argv1.find_last_of(':');
	client_ip = argv1.substr(0, t1);
	stringstream str(argv1.substr(t1 + 1));
	str >> upload_port;

	int t2 = argv2.find_last_of(':');
	tracker_ip_1 = argv2.substr(0, t2);
	stringstream str1(argv2.substr(t2 + 1));
	str1 >> tracker_port_1;

	int t3 = argv3.find_last_of(':');
	tracker_ip_2 = argv3.substr(0, t3);
	stringstream str2(argv3.substr(t3 + 1));
	str2 >> tracker_port_2;

	if (argv[4] != NULL)
		logfile_name = argv[4];
	else
		logfile_name="default_logfile";
}


vector<string> split(const string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);
   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void menu_driven_interface()
{
	int choice;
	cout << "Choose among the below options : "
		 << "\n";
	cout << "1. (share) To share a file "
		 << "\n";
	cout << "2. (get)  To download a remote file "
		 << "\n";
	cout << "3. Seed a downloaded file "
		 << "\n";
	cout << "4. (show) Show Downloads "
		 << "\n";
	cout << "5. (remove) Remove a shared file "
		 << "\n";
	cout << "6. (close) Close application "
		 << "\n";
/* 	cin >> choice;
	switch (choice)
	{
	case 1:
		share_to_tracker();
		break;
	case 2:
		get();
		break;
	case 3:
		seeding_downloaded_file();
		break;
	case 4:
		show_downloads();
		break;
	case 5:
		remove_shared_file();
		break;
	case 6:
		close_application();
		break;
	default:
		cout << "Invalid option.Choose again \n ";
		menu_driven_interface();
	} */
}

// share <local file path> <filename>.mtorrent
void share_to_tracker(int ret,string command,string filepath,string torrent_filename)
{
	string chunk_name = "/home/ritik/Downloads";
/* 	string name = "/home/ritik/Os_Assignment2/test1.txt"; */
	chunkfile(filepath.c_str(), chunk_name);
	cout << hash_value << "\n";
	const unsigned char *buffer_temp = reinterpret_cast<const unsigned char *>(hash_value.c_str());
	hash_value=create_sha1_of_sha1(buffer_temp);
	cout << hash_value << "\n";
	add_mtorrent_file(filepath,torrent_filename);
	string to_share=command+"|"+hash_value+"|"+filepath+":"+client_ip+":"+to_string(upload_port);
	send_message(ret,to_share.c_str(),1024);


}

// get <path to .mtorrent file> <destination path>
void get_file(int ret,string command,string torrent_filepath,string destination_path)
{
}

void seeding_downloaded_file()
{
}

// show downloads
void show_downloads()
{
}

// r​emove <filename.mtorrent>
void remove_shared_file(int ret,string command , string torrent_filename)
{
}

void close_application()
{
}

//The in_port_t and in_addr_t types , in_addr structure are defined as in <netinet/in.h>.
/* 	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length); */

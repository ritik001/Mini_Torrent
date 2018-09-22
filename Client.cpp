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
string client_ip;

map<string, vector<string>> seeder_list;
vector<string> download_files;

int chunksize;
char hashString[41];
string hash_value;

int main(int argc, char *argv[])
{

	logmessage("Client program started \n");
	// This thread will use it's client_ip and upload_port passed through command line to serve as a server.
	thread T1(handleClient_act_as_server);
	T1.detach();

	// chunksize (as we need to divide files into 512 KB)
	chunksize = 524288;
	if (argv[1] != NULL)
		Initialize_Client_Params(argv);

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	/*
	1. Create socket
	2. Connect to Server using server ip and listen port. 
	*/
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

	logmessage("Client connected to server \n");
	menu_driven_interface();
	string input;
	while (true)
	{
		// To accept the input from user for the functionality.
		getline(cin, input);
		vector<string> input_commands = split(input, ' ');
		if (input_commands[0] == "share")
		{
			share_to_tracker(clientSocket, input_commands[0], input_commands[1], input_commands[2]);
		}
		else if (input_commands[0] == "remove")
		{
			remove_shared_file(clientSocket, input_commands[0], input_commands[1]);
		}
		else if (input_commands[0] == "get")
		{
			get_file(clientSocket, input_commands[0], input_commands[1], input_commands[2]);
		}
		else if (input_commands[0] == "show")
		{
			show_downloads();
		}

		recv(clientSocket, buffer, 1024, 0);
		if (input_commands[0] == "get")
		{
			fire_threads_for_seeder_list(buffer);
		}
		/* 	   cout << buffer << "\n"; */
		memset(buffer, '\0', sizeof(buffer));
	}

	return 0;
}

// To act as a server for different clients.
// Client_ip and upload_port is used here.
void handleClient_act_as_server()
{
	int sockfd;
	int ret;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int opt = 1;

	int newSocket;
	struct sockaddr_in new_addr;

	socklen_t addr_size = sizeof(new_addr);
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		printf("Error in connection \n");
		exit(1);
	}
	printf("1. Server socket creation\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(upload_port);
	server_addr.sin_addr.s_addr = inet_addr(client_ip.c_str());

	ret = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (ret < 0)
	{
		printf("Error in binding\n");
		exit(1);
	}
	printf("2. Binded to the port %d\n", PORT);

	if (listen(sockfd, 10) == 0)
		printf("3. Listening\n");
	else
		printf("Error in listening\n");

	while (1)
	{
		newSocket = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

		if (newSocket < 0)
		{
			printf("Connection not accepted \n");
			exit(1);
		}
		client_ip = inet_ntoa(new_addr.sin_addr);
		int listen_port = ntohs(new_addr.sin_port);
		printf("4. Connection accepted from %s %d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));
	}
	close(sockfd);
}

// To connect to clients for requesting the files to be downloaded.
void non_blocking_client(string file_to_download, string ip_of_client_to_download, int port_to_download)
{
	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("1. Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port_to_download);
	serverAddr.sin_addr.s_addr = inet_addr(ip_of_client_to_download.c_str());

	ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("2. Connected to Client Server %s %d\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
}

// After getting seeder list , calling threads to connect to Ip and Port.
void fire_threads_for_seeder_list(char *buffer)
{
	string temp = buffer;
	vector<string> received_seeder_list = split(temp, '|');
	for (int i = 0; i < received_seeder_list.size(); i++)
	{
		vector<string> v2 = split(received_seeder_list[i], ':');
		thread T2(non_blocking_client, v2[0], v2[1], v2[2]);
		T2.detach();
	}
}

void send_message(int clientSocket, const char *message, int size)
{
	logmessage("Sending buffer from client to server \n");
	char buffer[1024];
	strcpy(buffer, message);
	if (send(clientSocket, buffer, 1024, 0) < 0)
		cout << "Error in sending "
			 << "\n";
}

void receive_message(int clientSocket, const char *message, int size)
{
	logmessage("Receving buffer from server to client \n");
	cout << "Client Recv " << message << "\n";
	char buffer[1024];
	strcpy(buffer, message);
	recv(clientSocket, buffer, size, 0);
}

void logmessage(char *message)
{
	FILE *fptr;
	fptr = fopen(logfile_name.c_str(), "r");
	if (fptr == NULL)
		fptr = fopen(logfile_name.c_str(), "w");
	else
		fptr = fopen(logfile_name.c_str(), "a");
	if (fptr == NULL)
		cout << "log file not opened "
			 << "\n";
	fprintf(fptr, "%s\n", message);
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
		logfile_name = "default_logfile";
}

vector<string> split(const string &s, char delimiter)
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

string parse_file_path(string path)
{

	if (path.empty() || path == "")
	{
		cerr << "Invalid arguments passed for filename ";
	}
	char actualpath[PATH_MAX];
	char *full_path = realpath(path.c_str(), actualpath);
	string apath(full_path);
	return apath;
}

void menu_driven_interface()
{
	int choice;
	cout << "Choose among the below options : "
		 << "\n";
	cout << "1. share <filepath> <mtorrent_filename> ; To share a file "
		 << "\n";
	cout << "2. get <mtorrent path> <des path> ; To download a remote file "
		 << "\n";
	cout << "3. Seed a downloaded file "
		 << "\n";
	cout << "4. show downloads ; Show Downloads "
		 << "\n";
	cout << "5. (remove filename.mtorrent) ; Remove a shared file "
		 << "\n";
	cout << "6. close ; Close application "
		 << "\n";
}

/*
1) To create the hash by dividing files into chunks of 512 KB.
2) Make a mtorrent file by adding in format 
<filepath>
tracker1_ip:port1
tracker2_ip:port2
3.3.3.3:3
<filesize>
Hash of file
3) To send a buffer to server which includes
command , hash of hash of file , filepath , client_ip , upload_port
*/

void share_to_tracker(int ret, string command, string filepath, string torrent_filename)
{
	logmessage("Sharing to the tracker \n");
	filepath = parse_file_path(filepath);
	string chunk_name = "chunk";
	download_files.push_back("D " + filepath);
	chunkfile(filepath.c_str(), chunk_name);
	cout << hash_value << "\n";
	add_mtorrent_file(filepath, torrent_filename);
	const unsigned char *buffer_temp = reinterpret_cast<const unsigned char *>(hash_value.c_str());
	hash_value = create_sha1_of_sha1(buffer_temp);

	string to_share = command + "|" + hash_value + "|" + filepath + ":" + client_ip + ":" + to_string(upload_port);
	send_message(ret, to_share.c_str(), 1024);
}

/* 
When Client gives get command. It will get hash from the mtorrent file.
After taking hash of hash of file , send a request to Tracker to get mtorrent file
*/
void get_file(int ret, string command, string torrent_filepath, string destination_path)
{
	logmessage("Sending buffer from client to server hash of hash of file \n");
	torrent_filepath = parse_file_path(torrent_filepath);
	string hash_of_file;
	ifstream file;
	int i = 0;
	file.open(torrent_filepath, ios::in);
	while (getline(file, hash_of_file))
	{
		i = i + 1;
		if (i == 5)
			break;
	}
	const unsigned char *buffer_temp = reinterpret_cast<const unsigned char *>(hash_of_file.c_str());
	hash_of_file = create_sha1_of_sha1(buffer_temp);
	cout << "hash of hash in file " << hash_of_file << "\n";

	string to_share = command + "|" + hash_of_file + "|" + client_ip + ":" + to_string(upload_port);
	send_message(ret, to_share.c_str(), 1024);
}

void seeding_downloaded_file()
{
}

// For Client show list of downloaded and downloading files which is in vector downloaded_files.
void show_downloads()
{
	cout << "Files downloaded or downloading are \n";
	for (int i = 0; i < download_files.size(); i++)
		cout << download_files[i] << "\n";
}

// To remove mtorrent file and delete corresponding Client entry in Tracker (Map and File(Persistent database))
void remove_shared_file(int ret, string command, string torrent_filename)
{
	torrent_filename = parse_file_path(torrent_filename);
}

void close_application()
{
}

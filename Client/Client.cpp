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

	thread T1(handleClient_act_as_server);
	T1.detach();

	chunksize = 524288;
	if (argv[1] != NULL)
		Initialize_Client_Params(argv);

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
	char buffer[1024];
	strcpy(buffer, message);
	if (send(clientSocket, buffer, 1024, 0) < 0)
		cout << "Error in sending "
			 << "\n";
}

void receive_message(int clientSocket, const char *message, int size)
{
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
}

// share <local file path> <filename>.mtorrent
void share_to_tracker(int ret, string command, string filepath, string torrent_filename)
{
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

// get <path to .mtorrent file> <destination path>
void get_file(int ret, string command, string torrent_filepath, string destination_path)
{
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

// show downloads
void show_downloads()
{
	cout << "Files downloaded or downloading are \n";
	for (int i = 0; i < download_files.size(); i++)
		cout << download_files[i] << "\n";
}

// r​emove <filename.mtorrent>
void remove_shared_file(int ret, string command, string torrent_filename)
{
	torrent_filename = parse_file_path(torrent_filename);
}

void close_application()
{
}

//The in_port_t and in_addr_t types , in_addr structure are defined as in <netinet/in.h>.
/* 	bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length); */

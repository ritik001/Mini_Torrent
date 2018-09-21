/**
    * Code By: Ritik Agarwal
    * Roll Number: 2018202017
    * Subject: Operating System- Assignment Mini Torrent
**/

#include "Server.h"

int mutlock = 0;
int count1 = 100;

string client_ip;
int listen_port;
int tracker_port_1;
int tracker_port_2;
string logfile_name;

string seederlist_file;
map<string, set<string>> seederlist;

int main(int argc, char *argv[])
{

    init_extern_params();

    if (argv[1] != NULL)
        Initialize_Server_Params(argv);

    int sockfd;
    int ret;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int opt = 1;

    int newSocket;
    struct sockaddr_in new_addr;

    socklen_t addr_size = sizeof(new_addr);
    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Leave protocol up to service provider

    if (sockfd < 0)
    {
        printf("Error in connection \n");
        exit(1);
    }
    printf("1. Server socket creation\n");

/*         setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt)); */
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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

/*     signal(SIGCHLD, SIG_IGN); */

    /*     printf("The address and port of connected client %s %d",client_ip.c_str(),listen_port); */

    while (1)
    {

        newSocket = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);

        if (newSocket < 0)
        {
            printf("Connection not accepted \n");
            exit(1);
        }
        client_ip = inet_ntoa(new_addr.sin_addr);
        listen_port = ntohs(new_addr.sin_port);
        printf("4. Connection accepted from %s %d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));

        while (true)
        {
        cout << "In Tracker buffer " << "\n";
        if(recv(newSocket,buffer,1024,0)<0)
            cout << "error in receiving " << "\n";

/*         cout << buffer << "\n"; */

        vector<string> input_commands = split(buffer,'|');
        if (input_commands[0] == "share")
        {
            share(input_commands[1],input_commands[2]);
        }
        else if (input_commands[0] == "remove")
        {
            remove(input_commands[1],input_commands[2]);
        }
        else if (input_commands[0] == "get")
        {
            Seed_List(input_commands[1],input_commands[2], input_commands[3]);
        }
        memset(buffer, '\0', sizeof(buffer));
        send(newSocket,"hello",20,0);
        }


/*         thread t1(handleClient, newSocket);

        t1.detach(); */
    }
    close(newSocket);

    return 0;
}

void send_message(int sockfd, string message,int size){
    send(sockfd,message.c_str(),size,0);
}

void receive_message(int sockfd, char* message,int size){
    recv(sockfd,message,size,0);
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
        cout << "log file not opened" << "\n";
    fprintf(fptr,"%s\n",message);
    fclose(fptr);
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




void init_extern_params()
{
}

void Initialize_Server_Params(char *argv[])
{
    string argv1 = argv[1];
    string argv2 = argv[2];
    string argv3 = argv[3];
    string argv4 = argv[4];

    int t1 = argv1.find_last_of(':');
    string tracker_ip_1 = argv1.substr(0, t1);
	stringstream str1(argv1.substr(t1 + 1));
	str1 >> tracker_port_1;

    int t2 = argv2.find_last_of(':');
    string tracker_ip_2 = argv2.substr(0, t2);
	stringstream str2(argv2.substr(t2 + 1));
	str1 >> tracker_port_2;

    seederlist_file=argv[3];
    if(argv[4]!=NULL)
        logfile_name = argv[4];
    else
        logfile_name="default_logfile";

    //seeder list file for seeder_list map
}

void Torrent_Tracker_function()
{
}

void share(string hash_name,string value)
{
    cout << "in share " << "\n";
    auto map_itr = seederlist.find(hash_name);
    if (map_itr != seederlist.end())
    {
        set<string> tracker_list = map_itr->second;
        tracker_list.insert(value);
        seederlist[hash_name] = tracker_list;
    }

    else
    {
        set<string> tracker_list;
        tracker_list.insert(value);
        seederlist[hash_name] = tracker_list;
    }
    cout << "hello dude " << "\n";
    string temp=hash_name+"|"+value;
    cout << temp << "\n";
    cout << "Yo man!! " << seederlist_file << "\n";

    fstream file;
    file.open(seederlist_file.c_str(),ios::in);
    if(file.is_open())
    {
        file.close();
        file.open(seederlist_file.c_str(),ios::app);
    }
    else
    {
        file.close();
        file.open(seederlist_file.c_str(),ios::out);    
    }
    file << temp << "\n";

    file.close();



}

void remove(string param,string param1)
{
    int index = param.find('|') + 1;
    string hash_name = param.substr(0, index);
    string value = param.substr(index);

    auto it = seederlist.find(hash_name);
    if (it != seederlist.end())
    {
        set<string> h2 = it->second;
        if (h2.size() == 1)
            seederlist.erase(it);
        else
        {
            h2.erase(value);
            seederlist[hash_name]=h2;
        }
    }

    else
    {
        cout << "No File found with the given hash value to remove \n";
    }
}

string Seed_List(string hash,string param1,string param2)
{
    set<string>::iterator it_set;
    string result = "";
    map<string, set<string>>::iterator it;
    for (it = seederlist.begin(); it != seederlist.end(); ++it)
    {
        if (it->first == hash)
        {
            set<string> temp = it->second;
            for (it_set=temp.begin();it_set!=temp.end();it_set++)
            {
                result = result + *it_set + "|";
            }
        }
    }

    result=result.substr(0,result.length()-2);

    return result;

    // 22233445 hash    result 123:34343:3434|343434
}

void initialize_seeder_list()
{
    ifstream seeder_file;
    seeder_file.open(seederlist_file);
    string seeder_info;

    while (getline(seeder_file, seeder_info))
    {
        int index = seeder_info.find(' ') + 1;
        string hash_name = seeder_info.substr(0, index);
        string value = seeder_info.substr(index);

        auto map_itr = seederlist.find(hash_name);
        if (map_itr != seederlist.end())
        {
            set<string> tracker_list = map_itr->second;
            tracker_list.insert(value);
            seederlist[hash_name] = tracker_list;
        }

        else
        {
            set<string> tracker_list;
            tracker_list.insert(value);
            seederlist[hash_name] = tracker_list;
        }
    }
    seeder_file.close();

    // From seeder file writing into map seerder list
}

void handleClient(int socketid)
{
    int n;
    char buffer[256];
    mutex mut;
    n = send(socketid, buffer, 10, 0);
    bzero(buffer, 10);
    n = recv(socketid, buffer, 10, 0);
    int k;
    cin >> k;
    if (n < 0)
    {
        printf("ERROR reading from socket");
        exit(1);
    }
    else
        printf("Here is the message: %s\n", buffer);

    bzero(buffer, 10);
    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(socketid, SOL_SOCKET, SO_ERROR, &error, &len);
    while (mutlock)
        ;
    mutlock = 1;
    count1 += 1;

    cerr << "changed count to" << count1 << "by" << this_thread::get_id << "on socket" << socketid;
    mutlock = 0;
    printf("\nPlease enter your message");
    scanf("%s", buffer);
    n = send(socketid, buffer, 10, 0);
    if (n < 0)
    {
        cerr << "ERROR writing to socket";
        exit(1);
    }
}

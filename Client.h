/**
    * Code By: Ritik Agarwal
    * Roll Number: 2018202017
    * Subject: Operating System- Assignment Mini Torrent
**/

#ifndef MiniTorrentClient_LIB
#define MiniTorrentClient_LIB

/* Header files needed for Client */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <bits/stdc++.h>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

/* Header files needed for creating sha1 and the connection part in socket */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/sha.h>

using namespace std;

#define SHA_DIGEST_LENGTH 20
#define PORT 62233

/* Variables , Maps and Vectors used for Client program */
extern int chunksize;
extern char hashString[41];
extern string hash_value;

extern int upload_port;
extern string tracker_ip_1;
extern int tracker_port_1;
extern string tracker_ip_2;
extern int tracker_port_2;
extern string logfile_name;

extern vector<string> download_files;

/* Functions for the Client */
void handleClient_act_as_server();
void send_message(int, const char *, int);
void receive_message(int, const char *, int);
void Initialize_Client_Params(char **);
vector<string> split(const string &, char);
void menu_driven_interface();
void share_to_tracker(int, string, string, string);
void get_file(int, string, string, string);
void seeding_downloaded_file();
void show_downloads();
void remove_shared_file(int, string, string);
void logmessage(string);
void close_application();
void non_blocking_client(string, string, int);
void fire_threads_for_seeder_list(char *);
string parse_file_path(string);

/* Functions for breaking into chunks , getting sha value and making mtorrent file */
void create_sha1(const unsigned char *);
int get_file_size(string);
void chunkfile(const char *, string);
void add_mtorrent_file(string, string);
string create_sha1_of_sha1(const unsigned char *);

#endif
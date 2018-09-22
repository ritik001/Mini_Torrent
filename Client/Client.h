/**
    * Code By: Ritik Agarwal
    * Roll Number: 2018202017
    * Subject: Operating System- Assignment Mini Torrent
**/

#ifndef MiniTorrentClient_LIB
#define MiniTorrentClient_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string.h>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SHA_DIGEST_LENGTH 20
#include <openssl/sha.h>
#include <iostream>
using namespace std;

#define PORT 60001

extern int chunksize;
extern char hashString[41];
extern string hash_value;

extern int upload_port;
extern string tracker_ip_1;
extern int tracker_port_1;
extern string tracker_ip_2;
extern int tracker_port_2;
extern string logfile_name;

extern map<string, vector<string>> seeder_list;
extern vector<string> download_files;

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
void logmessage(char *);
void close_application();
void non_blocking_client(string, string, int);
void fire_threads_for_seeder_list(char *);
string parse_file_path(string);

void create_sha1(const unsigned char *);
int get_file_size(string);
void chunkfile(const char *, string);
void add_mtorrent_file(string, string);
string create_sha1_of_sha1(const unsigned char *);

#endif
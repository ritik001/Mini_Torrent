/**
    * Code By: Ritik Agarwal
    * Roll Number: 2018202017
    * Subject: Operating System- Assignment Mini Torrent
**/

#ifndef MiniTorrentServer_LIB
#define MiniTorrentServer_LIB

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
#include <iostream>
using namespace std;

/* string tracker1_socket="127.0.0.1:4444";
string tracker2_socket="127.0.0.1:5555"; */

#define PORT 60001

extern string client_ip;
extern int listen_port;
extern string seederlist_file;
extern int tracker_port_1;
extern int tracker_port_2;
extern string logfile_name;

void send_message(int, string, int);
void receive_message(int, char *, int);
void init_extern_params();
void Initialize_Server_Params(char **);
void Torrent_Tracker_function();
void share(string, string);
void remove(string, string);
string Seed_List(string, string);
void initialize_seeder_list();
void handleClient(int);
void logmessage(char *);
string parse_file_path(string);

vector<string> split(const string &, char);

extern map<string, set<string>> seederlist;

/* struct seeder
{
    string value_of_hash;
    string value_of_file_name;
    string value_of_client_ip;
    int value_of_listen_port;
}; */

#endif
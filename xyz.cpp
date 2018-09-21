#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SHA_DIGEST_LENGTH 20
#include <openssl/sha.h>
#include <iostream>
using namespace std;

void create_sha1(const unsigned char *);
int get_file_size(string);
void chunkfile(const char *, string);
void add_mtorrent_file(string);
int upload_port;
string tracker_ip_1;
int tracker_port_1;
string tracker_ip_2;
int tracker_port_2;
string logfile_name;

map<string, vector<string>> seeder_list;
int chunksize = 524288;
char hashString[41];
string hash_value;

string client_ip;
int listen_port;

int get_file_size(string name)
{
    int file_size;
    ifstream file;
    file.open(name.c_str(), ios::in);
    if (file.is_open())
    {
        file.seekg(0, file.end);
        file_size = file.tellg();
        file.seekg(0, file.beg);
        file.close();
    }
    else
        cout << "Unable to open file \n";
    return file_size;
}

void chunkfile(const char *filename_path, string chunkname)
{
    ifstream file;
    file.open(filename_path, ios::in);
    if (file.is_open())
    {
        ofstream output;
        int id = 0;
        string fullchunkname;
        char intBuf[10];
        char *buffer = new char[chunksize];
        while (!file.eof())
        {
            fullchunkname = "";
            id = id + 1;
            fullchunkname = chunkname + "/" + to_string(id);

            output.open(fullchunkname.c_str(), ios::out);
            if (output.is_open())
            {
                file.read(buffer, chunksize);
                output.write(buffer, file.gcount());
                output.close();
            }
            const unsigned char *buffer_temp = reinterpret_cast<const unsigned char *>(buffer);
            create_sha1(buffer_temp);
        }

        delete (buffer);
        file.close();
    }
    else
    {
        cout << "Unable to open file for breaking into chunks \n";
    }
}

void create_sha1(const unsigned char *buffer)
{
    cout << "a";
    int a;
    cin >> a;

    string res;
    unsigned char digest[20];
    SHA1(buffer, SHA_DIGEST_LENGTH, digest);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&hashString[i * 2], "%02x", (unsigned int)digest[i]);

    hash_value = hash_value + hashString;
}

void add_mtorrent_file(string filename)
{

    ofstream out;
    string temp1 = tracker_ip_1 + ":" + to_string(tracker_port_1);
    string temp2 = tracker_ip_2 + ":" + to_string(tracker_port_2);
    out.open(filename, ios::out);
    if (out.is_open())
    {

        /*         cout << "hello2  " <<"\n"; */

        out << filename << "\n";
        out << temp1 << "\n";
        out << temp2 << "\n";
        out << get_file_size(filename) << "\n";
        out << hash_value << "\n";
    }
    out.close();
}

int main()
{
    string chunk_name = "/home/ritik/Downloads";
    string name = "/home/ritik/Os_Assignment2/1.pdf";
    chunkfile(name.c_str(), chunk_name);
    cout << hash_value << "\n";
    add_mtorrent_file(name);
}

/* void joinFile(char *chunkName, char *fileOutput) {
	string fileName;
	ofstream outputfile;
	outputfile.open(fileOutput, ios::out | ios::binary);

	if (outputfile.is_open()) {
		bool filefound = true;
		int counter = 1;
		int fileSize = 0;

		while (filefound) {
			filefound = false;
			fileName.clear();
			fileName.append(chunkName);
			fileName.append(".");

			char intBuf[10];
			_itoa(counter,intBuf,10);
			fileName.append(intBuf);

			ifstream fileInput;
			fileInput.open(fileName.c_str(), ios::in | ios::binary);

			if (fileInput.is_open()) {
				filefound = true;
				fileSize = getFileSize(&fileInput);
				char *inputBuffer = new char[fileSize];

				fileInput.read(inputBuffer,fileSize);
				outputfile.write(inputBuffer,fileSize);
				delete(inputBuffer);

				fileInput.close();
			}
			counter++;
		}

		outputfile.close();

		cout << "File merging complete!" << endl;
	}
	else { cout << "Error: Unable to open file for output." << endl; }

} */

/**
    * Code By: Ritik Agarwal
    * Roll Number: 2018202017
    * Subject: Operating System- Assignment Mini Torrent
**/

#include "Client.h"

// tellg function to get the size of file
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

// splitting a file into chunks and generating hash.
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

// creating sha1 for the buffer digest
void create_sha1(const unsigned char *buffer)
{
    string res;
    unsigned char digest[20];
    SHA1(buffer, SHA_DIGEST_LENGTH, digest);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&hashString[i * 2], "%02x", (unsigned int)digest[i]);
    string temp = hashString;
    temp.resize(20);

    hash_value = hash_value + temp;
}

// To generate sha of hash value of file
string create_sha1_of_sha1(const unsigned char *buffer)
{
    unsigned char digest[20];
    SHA1(buffer, SHA_DIGEST_LENGTH, digest);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&hashString[i * 2], "%02x", (unsigned int)digest[i]);
    string temp = hashString;
    temp.resize(20);
    return temp;
}

void add_mtorrent_file(string filename, string mtorrent_filename)
{

    ofstream out;
    string temp1 = tracker_ip_1 + ":" + to_string(tracker_port_1);
    string temp2 = tracker_ip_2 + ":" + to_string(tracker_port_2);
    out.open(mtorrent_filename, ios::out);
    if (out.is_open())
    {
        out << filename << "\n";
        out << temp1 << "\n";
        out << temp2 << "\n";
        out << get_file_size(filename) << "\n";
        out << hash_value << "\n";
    }
    out.close();
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

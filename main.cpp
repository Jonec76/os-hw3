// C++ implemntation of search() and traverse() methods
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;
#define MASK 0xFFFF000000000000
#define ULL_MAX 0xFFFFFFFFFFFFFFFF

const char *input_file_path = "1.input";
const char *output_file_path = "result.output";
const char *storage_path = "./storage";

unsigned long long limit_capacity = 40;
unsigned long long base_capacity = 20;

bool DirectoryExists(unsigned long long key, char **fn) {
    string output_data_name = storage_path;
    unsigned long long mask = MASK;
    string extension = "";
    for (int i = 0; i < 4; i++) {
        if (i == 3)
            extension = ".txt";
        output_data_name += "/" + to_string(key & mask) + extension;
        mask >>= 16;
    }
    strcpy(*fn, output_data_name.c_str());
    ifstream infile(*fn);
    return infile.good();
}

void get_data(unsigned long long key, map<unsigned long long, string> pool) {
    ofstream output_result;
    output_result.open(output_file_path, ios::out | ios::app );
    char *fn = (char *)malloc(128 * sizeof(char));
    map<unsigned long long, string>::iterator it = pool.find(key);
    if (it == pool.end()) {
        if (DirectoryExists(key, &fn)) {
            string value;
            ifstream file;
            file.open(fn);
            file >> value;
            output_result << value << "\n";
            file.close();
        } else {
            output_result << "EMPTY\n";
        }
    } else {
        output_result << it->second << endl;
    }
    free(fn);
	output_result.close();
}

void store_data(map<unsigned long long, string>* pool, unsigned long long base_capacity){
	auto it = pool->begin();
	unsigned long long idx = 0;
	while (it != pool->end() && idx < base_capacity) {
		unsigned long long k = it->first;
		string value = it->second;
		unsigned long long mask = MASK;
		string output_data_name = storage_path;

		for (int i = 0; i < 3; i++) {
			output_data_name += "/" + to_string(k & mask);
			const char *f = output_data_name.c_str();
			DIR *dir = opendir(f);
			if (!dir) {
				if (mkdir(f, 0777) != 0) {
					cout << "Can't create the folder. " << endl;
				}
			}
			mask >>= 16;
		}
		output_data_name += "/" + to_string(k & mask) + ".txt";
		ofstream output_data;
		output_data.open(output_data_name);
		output_data << value << endl;
		output_data.close();

		it++;
		idx++;
		pool->erase(k);
	}
}

int main() {
    // system("rm -rf ./storage");

    FILE *fp = fopen(input_file_path, "r");
    if (!fp) {
        perror("Unable to open file!");
        exit(1);
    }

    DIR *pDir;
    pDir = opendir(storage_path);
    if (!pDir) {
        mkdir(storage_path, 0777);
        (void)closedir(pDir);
    }

    char chunk[512];
    map<unsigned long long, string> pool;
    size_t run = 0;

    while (fgets(chunk, sizeof(chunk), fp) != NULL) {
        chunk[strcspn(chunk, "\n")] = '\0'; // For removing the last \n char.
        char *ch = strtok(chunk, " ");
        run = 0;
        while (ch != NULL) {
            if (run == 0) {
                if (strcmp(ch, "PUT") == 0) {
                    unsigned long long key = strtoull(strtok(NULL, " "), NULL, 10);
                    string value = strtok(NULL, " ");

                    pool[key] = value;
                    // pool.insert(pair<unsigned long long, string>(lk, v));
                    if (pool.size() > limit_capacity) 
                        store_data(&pool, base_capacity);
                    
                } else if (strcmp(ch, "GET") == 0) {
                    unsigned long long key = strtoull(strtok(NULL, " "), NULL, 10);
                    get_data(key, pool);
                } else if (strcmp(ch, "SCAN") == 0) {
                    unsigned long long from = strtoull(strtok(NULL, " "), NULL, 10);
                    unsigned long long to = strtoull(strtok(NULL, " "), NULL, 10);
                    for (unsigned long long key = from; key <= to; key++)
                        get_data(key, pool);
                }
            }
            ch = strtok(NULL, " ");
            run++;
        }
    }
	store_data(&pool, ULL_MAX);
}

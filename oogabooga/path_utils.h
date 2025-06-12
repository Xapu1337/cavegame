#ifndef OOGABOOGA_PATH_UTILS_H
#define OOGABOOGA_PATH_UTILS_H

#include "string.h"

string get_file_extension(string path);
string get_file_name_including_extension(string filePath);
string get_file_name_excluding_extension(string filePath);
string get_directory_of(string path);

#endif

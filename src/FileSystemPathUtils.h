#ifndef FILESYSTEMPATHUTILS_H
#define FILESYSTEMPATHUTILS_H

#include <string>

static const char *const WSL_FILE_PATH_HEADER = "/mnt";
static const char WINDOWS_FILE_PATH_SEPARATOR = '\\';
static const char UNIX_FILE_PATH_SEPARATOR = '/';

std::string windows_to_unix_file_path(std::string file_path, bool is_wsl = true);

std::string unix_to_windows_file_path(std::string file_path, bool is_wsl = true);

std::string convertPath(std::string file_path);

#endif // FILESYSTEMPATHUTILS_H

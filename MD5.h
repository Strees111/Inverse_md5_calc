#ifndef KS_MD5_H
#define KS_MD5_H
#include <iostream>
#include <string>
#include <vector>
#include <atomic>
#ifdef _WIN32 // Код для Windows
#include <windows.h>
#else // Код для Unix-подобных систем
#include <csignal>
#endif

extern std::atomic<bool> is_paused;
extern std::atomic<bool> is_found;
extern std::vector<int> current_position;
extern std::string hash;
extern std::string charset;
extern std::string current_prefix;

void save_state(const std::string &prefix);
void load_state(std::string &prefix);
std::string load(const std::string &config);
std::string md5(const std::string &input);
void bruteForceMD5(int len,std::ostream& out);
#ifdef _WIN32
BOOL WINAPI ConsoleHandler(DWORD signal);
#else
void signal_h(int signal);
#endif
void run(std::ostream& out = std::cout);
#endif

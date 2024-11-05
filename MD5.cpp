#include <iostream>
#include <openssl/md5.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include "MD5.h"
#ifdef _WIN32 // Код для Windows
#include <windows.h>
#else // Код для Unix-подобных систем
#include <csignal>
#endif

std::atomic<bool> is_paused(false);
std::atomic<bool> is_found(false);
std::string hash;
std::string charset;
std::string current_prefix;
std::vector<int> current_position;
std::mutex output_mutex;
std::mutex found_mutex;
unsigned int num_threads = std::thread::hardware_concurrency();

void save_state(const std::string &prefix) {
    std::ofstream out("states.txt");
    if (!out.is_open()) {
        std::cout << "The file could not be opened" << std::endl;
    }
    if (hash.empty() || charset.empty() || current_position.empty()) {
        std::cout << "The state is not saved" << std::endl;
        return;
    }
    out << prefix << std::endl;
    out << hash << std::endl;
    out << charset << std::endl;
    for (int pos: current_position) {
        out << pos << std::endl;
    }
    out.close();
    std::cout << "The state is saved" << std::endl;
}

void load_state(std::string &prefix) {
    std::ifstream in("states.txt");
    if (!in.is_open()) {
        std::cout << "The file could not be opened" << std::endl;
        prefix.clear();
    }
    in >> prefix;
    in >> hash;
    in >> charset;
    current_position.clear();
    int pos;
    while (in >> pos) {
        current_position.push_back(pos);
    }
    in.close();
    std::cout << "The status is loaded" << std::endl;
}

std::string load(const std::string &config) {
    std::ifstream file(config);
    if (!file.is_open()) {
        std::cout << "The file could not be opened" << std::endl;
    }
    std::string temp;
    file >> temp;
    return temp;
}

// Считаем MD5 - хеш
std::string md5(const std::string &input) {
    unsigned char result[MD5_DIGEST_LENGTH]{};
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, input.c_str(), input.length());
    MD5_Final(result, &ctx);
    std::ostringstream out;
    out << std::hex << std::setfill('0');
    for (auto i: result) {
        out << std::setw(2) << static_cast<int>(i);
    }
    return out.str();
}

void bruteForceMD5(int len, std::ostream& out) {
    std::vector<int> local_position;
    {
        std::lock_guard<std::mutex> lock(output_mutex);
        local_position = current_position;
    }
    while (!is_found && !is_paused) {
        std::string attempt;
        for (int pos: local_position) {
            attempt += charset[pos];
        }
        current_prefix = attempt;

        {
            if (md5(attempt) == hash) {
                if (is_found) { return; }
                is_found = true;
                std::lock_guard<std::mutex> lock(found_mutex);
                out << "A match was found: " << attempt << std::endl;
                return;
            }
        }

        // Увеличение локальной позиции
        int idx = static_cast<int>(local_position.size() - 1);
        while (idx >= 0) {
            if (local_position[idx] + 1 < charset.size()) {
                local_position[idx]++;
                break;
            } else {
                local_position[idx] = 0;
                idx--;
            }
        }

        // Расширение длины строки, если достигнут максимум
        if (idx < 0 && local_position.size() < len) {
            local_position.insert(local_position.begin(), 0);
        } else if (idx < 0) {
            break; // Достигнут максимальный размер строки
        }
        // Обновление глобальной позиции для сохранения состояния
        {
            std::lock_guard<std::mutex> lock(output_mutex);
            current_position = local_position;
        }
    }
}

#ifdef _WIN32
// Обработчик для события CTRL+C (аналог SIGINT)
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        is_paused = true;
        save_state(current_prefix);
        std::cout << "The program is suspended. To resume, restart with the parameter 'resume'." << std::endl;
        return TRUE;
    }
    return FALSE;
}
#else
void signal_h(int signal) {
    if (signal == SIGINT) {
        is_paused = true;
        save_state(current_prefix);
        std::cout << "The program is suspended. To resume, restart with the parameter 'resume'." << std::endl;
    }
}
#endif

void run(std::ostream& out) {
    std::vector<std::thread> threads;
    unsigned int max_len = num_threads;
    for (int i = 0; i < max_len; ++i) {
        if (is_found) { break; }
        threads.emplace_back(bruteForceMD5, max_len, std::ref(out));
    }
    for (auto &i: threads) {
        if (i.joinable()) { i.join(); }
    }
    if (!is_found && !is_paused) { out << "There are no matches" << std::endl; }
}
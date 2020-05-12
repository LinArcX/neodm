#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <pthread.h>
#include <unordered_map>
#include <vector>

int index;
std::mutex _mutex;
std::vector<std::vector<std::string>> initial_data;
std::unordered_map<int, pthread_t> _tm;

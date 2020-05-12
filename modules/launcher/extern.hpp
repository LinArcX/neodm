#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <pthread.h>
#include <unordered_map>
#include <vector>

extern int index;
extern std::mutex _mutex;
extern std::vector<std::vector<std::string>> initial_data;
extern std::unordered_map<int, pthread_t> _tm;

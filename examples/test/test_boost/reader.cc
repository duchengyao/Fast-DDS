#include "boost/interprocess/managed_shared_memory.hpp"

#include <iostream>

using namespace boost::interprocess;

int main() {
  managed_shared_memory managed_shm{open_or_create, "boost", 1024};
  std::pair<int*, std::size_t> p = managed_shm.find<int>("Integer");
  if (p.first)
    std::cout << *p.first << '\n';
}
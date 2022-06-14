#ifndef EXTENDABLE_HASHING_H
#define EXTENDABLE_HASHING_H

#include <bits/stdc++.h>

#include "util.hpp"
using namespace std;
#define MAX_BUCKET_SIZE 3

template <typename T>
class Page {
 public:
  vector<pair<string, T>> map;
  int local_depth;
  Page();
  bool full();
  void put(string k, T v);
  T get(string k);
  int get_local_high_bit();
};

template <typename T>
class extendable_hashing {
  int global_depth;
  vector<Page<T>*> directory;

 public:
  extendable_hashing();
  Page<T>* get_page(string k);
  void put(string k, T v);
  T get(string k);
};

template <typename T>
Page<T>::Page() {
  local_depth = 0;
}

template <typename T>
bool Page<T>::full() {
  return map.size() >= MAX_BUCKET_SIZE;
}

template <typename T>
void Page<T>::put(string k, T v) {
  for (int i = 0; i < map.size(); ++i)
    if (map[i].first == k) {
      map.erase(map.begin() + i);
      break;
    }
  map.push_back(make_pair(k, v));
}

template <typename T>
T Page<T>::get(string k) {
  for (int i = 0; i < map.size(); ++i) {
    if (map[i].first == k) return map[i].second;
  }
  return T();
}

template <typename T>
int Page<T>::get_local_high_bit() {
  return 1 << local_depth;
}

template <typename T>
extendable_hashing<T>::extendable_hashing() {
  global_depth = 0;
  directory.push_back(new Page<T>());
}

template <typename T>
Page<T>* extendable_hashing<T>::get_page(string k) {
  size_t h = hash_string(k);
  return directory[h & ((1 << global_depth) - 1)];
}

template <typename T>
void extendable_hashing<T>::put(string k, T v) {
  Page<T>* p = get_page(k);
  bool full = p->full();
  p->put(k, v);
  if (full) {
    if (p->local_depth == global_depth) {
      auto tmp = directory;
      for (int i = 0; i < tmp.size(); ++i) directory.push_back(tmp[i]);
      global_depth += 1;
    }

    Page<T>* p0 = new Page<T>();
    Page<T>* p1 = new Page<T>();

    p0->local_depth = p1->local_depth = p->local_depth + 1;
    auto high_bit = p->get_local_high_bit();

    for (int i = 0; i < p->map.size(); ++i) {
      size_t h = hash_string(p->map[i].first);
      Page<T>* new_p = (h & high_bit) ? p1 : p0;
      new_p->put(p->map[i].first, p->map[i].second);
    }

    for (int i = (hash_string(k) & (high_bit - 1)); i < directory.size();
         i += high_bit) {
      directory[i] = (i & high_bit) ? p1 : p0;
    }

    delete p;
  }
}

template <typename T>
T extendable_hashing<T>::get(string k) {
  return get_page(k)->get(k);
}

#endif
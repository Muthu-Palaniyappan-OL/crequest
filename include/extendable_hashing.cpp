#include "extendable_hashing.hpp"

template <typename S, typename T>
class extendable_hashing {
  int global_depth;
  vector<Page<S, T>*> directory;
  hash<S> hash_func;

 public:
  extendable_hashing();
  Page<S, T>* get_page(S k);
  void put(S k, T v);
  T get(S k);
  T operator[](S s);
};

template <typename S, typename T>
Page<S, T>::Page() {
  local_depth = 0;
}

template <typename S, typename T>
bool Page<S, T>::full() {
  return map.size() >= MAX_BUCKET_SIZE;
}

template <typename S, typename T>
void Page<S, T>::put(S k, T v) {
  for (int i = 0; i < map.size(); ++i)
    if (map[i].first == k) {
      map.erase(map.begin() + i);
      break;
    }
  map.push_back(make_pair(k, v));
}

template <typename S, typename T>
T Page<S, T>::get(S k) {
  for (int i = 0; i < map.size(); ++i) {
    if (map[i].first == k) return map[i].second;
  }
  return T();
}

template <typename S, typename T>
int Page<S, T>::get_local_high_bit() {
  return 1 << local_depth;
}

template <typename S, typename T>
extendable_hashing<S, T>::extendable_hashing() {
  global_depth = 0;
  directory.push_back(new Page<S, T>());
}

template <typename S, typename T>
Page<S, T>* extendable_hashing<S, T>::get_page(S k) {
  size_t h = hash_func(k);
  return directory[h & ((1 << global_depth) - 1)];
}

template <typename S, typename T>
void extendable_hashing<S, T>::put(S k, T v) {
  Page<S, T>* p = get_page(k);
  bool full = p->full();
  p->put(k, v);
  if (full) {
    if (p->local_depth == global_depth) {
      auto tmp = directory;
      for (int i = 0; i < tmp.size(); ++i) directory.push_back(tmp[i]);
      global_depth += 1;
    }

    Page<S, T>* p0 = new Page<S, T>();
    Page<S, T>* p1 = new Page<S, T>();

    p0->local_depth = p1->local_depth = p->local_depth + 1;
    auto high_bit = p->get_local_high_bit();

    for (int i = 0; i < p->map.size(); ++i) {
      size_t h = hash_func(p->map[i].first);
      Page<S, T>* new_p = (h & high_bit) ? p1 : p0;
      new_p->put(p->map[i].first, p->map[i].second);
    }

    for (int i = (hash_func(k) & (high_bit - 1)); i < directory.size();
         i += high_bit) {
      directory[i] = (i & high_bit) ? p1 : p0;
    }

    delete p;
  }
}

template <typename S, typename T>
T extendable_hashing<S, T>::get(S k) {
  return get_page(k)->get(k);
}

template <typename S, typename T>
T extendable_hashing<S, T>::operator[](S s) {
  return get(s);
}
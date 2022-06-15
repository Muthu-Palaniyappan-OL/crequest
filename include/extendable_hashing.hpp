#ifndef EXTENDABLE_HASHING_H
#define EXTENDABLE_HASHING_H

#include <bits/stdc++.h>

using namespace std;
#define MAX_BUCKET_SIZE 3

template <typename S, typename T>
class Page {
 public:
  vector<pair<S, T>> map;
  int local_depth;
  Page();
  bool full();
  void put(S k, T v);
  T get(S k);
  int get_local_high_bit();
};

#include "extendable_hashing.cpp"

#endif
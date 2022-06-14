#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include <iostream>

#include "include/extendable_hashing.hpp"
using namespace std;

#define MAX_BUFFER_SIZE 500

list<string> blocked_ips;
extendable_hashing<int> ip_map;
pthread_t t1;

void remove_blocked_ips() {
  while (!blocked_ips.empty()) {
    string s = "iptables-legacy -D INPUT -s " + blocked_ips.back() + " -j DROP";
    cout << "Removing " << blocked_ips.back() << endl;
    system(s.c_str());
    ip_map.put(blocked_ips.back(), 0);
    blocked_ips.pop_back();
  }
}

void* continously_remove_blocked_ips(void* arg) {
  int time = *((int*)arg);
  while (1) {
    sleep(time);
    remove_blocked_ips();
  }
}

void sig_handler(int signum) {
  remove_blocked_ips();
  pthread_cancel(t1);
  exit(0);
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    cout << "USAGE: \n\t./crequest <port> <window-time> <request-limit>\n";
    return 0;
  }
  int ipc[2];
  signal(SIGINT, sig_handler);
  int duration = stoi(argv[2]);
  pthread_create(&t1, nullptr, continously_remove_blocked_ips,
                 (void*)&duration);
  pipe(ipc);
  pid_t pid = fork();
  if (pid == 0) {
    dup2(ipc[1], 1);
    execlp("tcpdump", "tcpdump", "-i", "any", "-ln", "--immediate-mode", "port",
           argv[1], NULL);
  } else {
    char buffer[MAX_BUFFER_SIZE];
    string s;
    regex r("(/[a-zA-Z0-9]*) HTTP");
    regex regex_ip(" IP (\\d+.\\d+.\\d+.\\d+)");
    smatch m;
    FILE* f = fdopen(ipc[0], "r");
    while (true) {
      fgets(buffer, MAX_BUFFER_SIZE, f);
      s = buffer;
      regex_search(s, m, r);
      if (!m.empty()) {
        string path = m[1];
        regex_search(s, m, regex_ip);
        string ip = m[1];
        ip_map.put(ip, ip_map.get(ip) + 1);
        cout << "Times: " << ip_map.get(ip) << " " << ip << " from " << path
             << endl;
        if (ip_map.get(ip) == stoi(argv[3])) {
          cout << "Blocking " << ip << endl;
          string s = "iptables-legacy -A INPUT -s " + ip + " -j DROP";
          blocked_ips.push_back(ip);
          system(s.c_str());
        }
      }
    }
  }
  return 0;
}
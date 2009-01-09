#ifndef _COMMON_HH_
#define _COMMON_HH_

#include "structures.hh"

// This is borrowed from the STL headers
inline size_t hash_string(const char* __s) {
  unsigned long __h = 0;
  for ( ; *__s; ++__s)
    __h = 5*__h + *__s;
  return __h;
}

gid_t resolve_gid(char *group);
uid_t resolve_uid(char *user);
bool match_user(uid_t uid, gid_t gid);
char* get_preferred_shell(char* filename);
char* get_preferred_shell(configuration &conf);
char* get_default_shell(configuration &conf);
bool test_shell(char* shell);

#endif //_COMMON_HH_

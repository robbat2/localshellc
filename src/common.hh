/* $Header: /code/convert/cvsroot/infrastructure/localshellc/src/common.hh,v 1.1 2005/07/17 19:57:56 robbat2 Exp $ */
#ifndef _COMMON_HH_
#define _COMMON_HH_

inline size_t hash_string(const char* __s) {
  unsigned long __h = 0;
  for ( ; *__s; ++__s)
    __h = 5*__h + *__s;
  return __h;
}

inline *cfg_list create_cfg_list(char *value, uid_t uid, gid_t gid) {
	cfg_list *cfg = new cfg_list;
	cfg->value = strdup(value);
	cfg->uid = uid;
	cfg->gid = gid;
	return cfg;
}
#endif //_COMMON_HH_

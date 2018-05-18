#ifndef SQL_H
#define SQL_H

extern void* sql_init(char *db_name, char *fsql);
extern void sql_close(void *db);

#endif
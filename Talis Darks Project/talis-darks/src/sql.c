#include "sqlite3/sqlite3.h"
#include "SDL.h"

void sql_close(void *db) {
    sqlite3_close((sqlite3*)db);
}

int sql_err(sqlite3 *db, int rc, char *sql, sqlite3_stmt *stmt) {
    SDL_Log("sqlite error %i: %s\nfor sql: %s\n", rc, sqlite3_errmsg(db), sql);
    sqlite3_finalize(stmt);
    return 0;
}

int sql_run(sqlite3 *db, char *sql) {
    int rc = -1;
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) return sql_err(db, rc, sql, stmt);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) return sql_err(db, rc, sql, stmt);
    sqlite3_finalize(stmt);

    return 1;
}

int build_db(void *db_, char *fsql_) {
    sqlite3 *db = (sqlite3*)db_;
    SDL_RWops *rw = SDL_RWFromFile(fsql_, "rb");
    if (rw == NULL) {
        SDL_Log("sql syntax load error\n");
        return 0;
    }

    char sql[1024] = { '\0' };
    char c = '\0';
    int i = 0;

    while (SDL_RWread(rw, &c, 1, 1) > 0) {
        if (i == 1023) {
            SDL_Log("sqlite parse error: max characters exceeded in statement\n");
            return 0;
        }

        if ((c == '\n') || (c == '\r')) continue;

        sql[i] = c;
        if (c == ';') {
            if (!sql_run(db, &sql)) return 0;
            for (i; i>-1; --i) sql[i] = '\0';
            i = 0;
        }
        else i++;
    }

    SDL_RWclose(rw);

    return 1;
}

void* sql_init(char *db_name, char *fsql) {
    sqlite3 *db = NULL;
    if (sqlite3_open(db_name, &db)) {
        SDL_Log("sqlite3 error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    if (!build_db(db, fsql)) return NULL;

    return (void*)db;
}
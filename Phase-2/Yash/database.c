#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "database.h"

static sqlite3 *db = NULL;
int initializeDatabase(void)
{
    const char *sql =
        "CREATE TABLE IF NOT EXISTS passengers ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "age INTEGER,"
        "priority INTEGER,"
        "coach_type INTEGER,"
        "seat_no INTEGER DEFAULT -1,"
        "ticket_price REAL,"
        "status INTEGER DEFAULT 0"
        ");";

    char *errorMessage = NULL;

    if (sqlite3_open("railway.db", &db) != SQLITE_OK)
    {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    if (sqlite3_exec(db, sql, NULL, NULL, &errorMessage) != SQLITE_OK)
    {
        printf("Table creation error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
        return 0;
    }

    printf("\nSQLite database connected successfully!\n");
    return 1;
}

/* Save a new passenger or update an existing passenger */
int savePassenger(const struct Passenger *p)
{
    const char *sql =
        "INSERT INTO passengers "
        "(id, name, age, priority, coach_type, seat_no, ticket_price, status) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?) "
        "ON CONFLICT(id) DO UPDATE SET "
        "name=excluded.name, "
        "age=excluded.age, "
        "priority=excluded.priority, "
        "coach_type=excluded.coach_type, "
        "seat_no=excluded.seat_no, "
        "ticket_price=excluded.ticket_price, "
        "status=excluded.status;";

    sqlite3_stmt *stmt = NULL;

    if (db == NULL || p == NULL)
        return 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Prepare error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, p->id);
    sqlite3_bind_text(stmt, 2, p->name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, p->age);
    sqlite3_bind_int(stmt, 4, p->priority);
    sqlite3_bind_int(stmt, 5, p->coachType);
    sqlite3_bind_int(stmt, 6, p->seatNo);
    sqlite3_bind_double(stmt, 7, p->ticketPrice);
    sqlite3_bind_int(stmt, 8, p->status);

    int result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE)
    {
        printf("Passenger save error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("Passenger %d saved to database.\n", p->id);
    return 1;
}
int loadPassengers(struct Passenger passengers[], int maxPassengers)
{
    const char *sql =
        "SELECT id, name, age, priority, coach_type, seat_no, "
        "ticket_price, status FROM passengers ORDER BY id;";

    sqlite3_stmt *stmt = NULL;
    int count = 0;

    if (db == NULL || passengers == NULL || maxPassengers <= 0)
        return 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Passenger load error: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    while (count < maxPassengers &&
           sqlite3_step(stmt) == SQLITE_ROW)
    {
        passengers[count].id =
            sqlite3_column_int(stmt, 0);

        const unsigned char *name =
            sqlite3_column_text(stmt, 1);

        snprintf(passengers[count].name,
                 sizeof(passengers[count].name),
                 "%s",
                 name ? (const char *)name : "");

        passengers[count].age =
            sqlite3_column_int(stmt, 2);

        passengers[count].priority =
            sqlite3_column_int(stmt, 3);

        passengers[count].coachType =
            sqlite3_column_int(stmt, 4);

        passengers[count].seatNo =
            sqlite3_column_int(stmt, 5);

        passengers[count].ticketPrice =
            sqlite3_column_double(stmt, 6);

        passengers[count].status =
            sqlite3_column_int(stmt, 7);

        count++;
    }

    sqlite3_finalize(stmt);

    printf("%d passenger record(s) loaded.\n", count);

    return count;
}


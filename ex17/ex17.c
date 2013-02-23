#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	int max_rows;
	int max_data;
	struct Address *rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn);
void die(struct Connection * conn, const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}
	Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n",
			addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	struct Database * db = conn->db;
	struct Address *addr;
	char * buffer;
	int rc;
	int i;
	rc = fread(db, sizeof(int), 2, conn->file);
	if (rc != 2)
		die(conn, "Failed to load database");
	db->rows = malloc(sizeof(struct Address) * db->max_rows);
	buffer = malloc(sizeof(char) * db->max_data);
	for(i = db->max_rows, addr = db->rows; i; i--,addr++) {
		rc = fread(addr, sizeof(int), 2, conn->file);
		if (rc != 2)
			goto die;
		if (! addr->set) {
			addr->name = NULL;
			addr->email = NULL;
			continue;
		}
		rc = fread(buffer, sizeof(char), db->max_data, conn->file);
		if (rc != db->max_data)
			goto die;
		addr->name = strndup(buffer, db->max_data);
		rc = fread(buffer, sizeof(char), db->max_data, conn->file);
		if (rc != db->max_data)
			goto die;
		addr->email = strndup(buffer, db->max_data);
	}
	free(buffer);
	return;
die:
	free(buffer);
	die(conn, "Failed to load database");
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn)
		die(NULL, "Memory error");

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db)
		die(conn, "Memory error");

	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");

		if (conn->file) {
			Database_load(conn);
		}
	}

	if (!conn->file)
		die(conn, "Failed to open the file");

	return conn;
}

void Database_close(struct Connection *conn)
{
	if (conn) {
		if (conn->file) fclose(conn->file);
		if (conn->db) free(conn->db);
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write to the database");

	rc = fflush(conn->file);
	if (rc == -1) die(conn, "Cannot flush database");
}

void Database_create(struct Connection *conn, int max_rows, int max_data)
{
	int i = 0;
	struct Database * db = conn->db;

	db->max_rows = max_rows;
	db->max_data = max_data;
	db->rows = malloc(sizeof(struct Address) * max_rows);
	for (i = 0; i < max_rows; i++) {
		// make a prototype to initialize it
		struct Address addr = { .id = i, .set = 0 };
		
		// then just assign it
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr;

	if (!(id < conn->db->max_rows))
		die(conn, "There aren't that many records");
	addr = &conn->db->rows[id];
	if (addr->set)
		die(conn, "Already set, delete it first");

	addr->set = 1;

	addr->name = strndup(name, conn->db->max_data);
	addr->email = strndup(email, conn->db->max_data);
	if (!(addr->name && addr->email))
		die(conn, "Out of memory");
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr;

	if (!(id < conn->db->max_rows))
		die(conn, "There aren't that many records");

	addr = &conn->db->rows[id];

	if (addr->set) {
		Address_print(addr);
	} else {
		die(conn, "ID is not set");
	}
}

void Database_delete(struct Connection *conn, int id)
{
	struct Address *addr;

	if (!(id < conn->db->max_rows))
		die(conn, "There aren't that many records");
	addr = &conn->db->rows[id];
	addr->set = 0;
	if (addr->name) {
		free(addr->name);
		addr->name = NULL;
	}
	if (addr->email) {
		free(addr->email);
		addr->email = NULL;
	}
}

void Database_list(struct Connection *conn)
{
	int i;
	struct Database *db = conn->db;
	struct Address *cur;

	for (i = db->max_rows, cur = db->rows; i != 0; i--, cur++) {
		if (cur->set) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3) die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);

	switch (action) {
		case 'c':
			if (argc != 5)
				die(conn, "Need to specify total entries and  max length");

			Database_create(conn, atoi(argv[3]), atoi(argv[4]));
			Database_write(conn);
			break;
		case 'g':
			if (argc != 4) die(conn, "Need an id to get");

			Database_get(conn, atoi(argv[3]));
			break;
		case 's':
			if (argc != 6)
				die(conn, "Need id, name, email to set");
			Database_set(conn, atoi(argv[3]), argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if (argc != 4)
				die(conn, "Need an id to delete");
			Database_delete(conn, atoi(argv[3]));
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die(conn, "Invalid action, only: c=create, g=get, s=set, d=delete, l=list");
	}

	Database_close(conn);

	return 0;
}

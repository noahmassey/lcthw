#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
	int id;
	char *name;
	char *email;
};

struct Database {
	struct Address **rows;
	int max_data;
	int max_rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
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

void Address_delete(struct Address *addr)
{
}

struct Connection *Database_new(const char *filename, int max_data, int max_rows)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn)
		die("Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db)
		die("Memory error", conn);

	conn->db->max_data = max_data;
	conn->db->max_rows = max_rows;
	conn->db->rows = (struct Address **) calloc(max_rows, sizeof(struct Address *));
	if (!conn->db->rows)
		die("Memory error", conn);

	conn->file = fopen(filename, "w");

	return conn;
}

struct Connection *Database_open(const char *filename)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn)
		die("Memory error", conn);

	conn->db = malloc(sizeof(struct Database));
	if (!conn->db)
		die("Memory error", conn);

	conn->file = fopen(filename, "r+");

	conn->db->rows = (struct Address **) calloc(conn->db->max_rows, sizeof(struct Address *));

	if (!conn->file)
		die("Failed to open the file", conn);
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1)
		die("Failed to load database", conn);

	return conn;
}

void Database_close(struct Connection *conn)
{
	if (conn) {
		if (conn->file)
			fclose(conn->file);
		if (conn->db) {
			if (conn->db->rows)
				free(conn->db->rows);
			free(conn->db);
		}
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	int i = 0;
	int rc = 0;
	int nope = -1;

	rewind(conn->file);

	rc = fwrite(&conn->db->max_data, sizeof(int), 1, conn->file);
	if (rc != 1)
		die("Failed to write max_data to the database", conn);

	rc = fwrite(&conn->db->max_rows, sizeof(int), 1, conn->file);
	if (rc != 1)
		die("Failed to write max_rows to the database", conn);

	for (i = 0; i < conn->db->max_rows; i++) {
		if (conn->db->rows[i] == NULL) {
			rc = fwrite(&nope, sizeof(nope), 1, conn->file);
			if (rc != 1)
				die("Failed to write _empty_ record to the database", conn);
			continue;
		}
		rc = fwrite(&conn->db->rows[i]->id, sizeof(int), 1, conn->file);
		if (rc != 1)
			die("Failed to write id to the database", conn);

		rc = fwrite(conn->db->rows[i]->name, sizeof(char), conn->db->max_data, conn->file);
		if (rc != 1)
			die("Failed to write name to the database", conn);

		rc = fwrite(conn->db->rows[i]->email, sizeof(char), conn->db->max_data, conn->file);
		if (rc != 1)
			die("Failed to write email to the database", conn);
	}

	rc = fflush(conn->file);
	if (rc == -1)
		die("Cannot flush database", conn);
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = conn->db->rows[id];
	if (addr)
		die("Already set, delete it first", conn);


	// WARNING: bug, read the "How To Break It" and fix this
	char *res = strncpy(addr->name, name, conn->db->max_data);

	// demonstrate the strncpy bug
	//if (!res)
	//	die("Name copy failed");
	
	// Fix the strncpy bug
	res[conn->db->max_data - 1] = '\0';

	res = strncpy(addr->email, email, conn->db->max_data);
	if (!res)
		die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
	struct Address *addr = conn->db->rows[id];

	if (addr) {
		Address_print(addr);
	} else {
		die("ID is not set", conn);
	}
}

void Database_delete(struct Connection *conn, int id)
{
	Address_delete(conn->db->rows[id]);
}

void Database_list(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	for (i = 0; i < conn->db->max_rows; i++) {
		struct Address *cur = db->rows[i];

		if (cur) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn;
	int id = 0;

	if (action == 'c') {
		int max_data = MAX_DATA;
		int max_rows = MAX_ROWS;
		if (argc > 3) {
			max_data = atoi(argv[3]);
			if (argc > 4) {
				max_rows = atoi(argv[4]);
			}
		}
		conn = Database_new(filename, max_data, max_rows);
	} else {
		conn = Database_open(filename);
		if (argc > 3)
			id = atoi(argv[3]);

		if (id > conn->db->max_rows)
			die("Specified ID is to large.", conn);
	}

	switch (action) {
		case 'c':
			Database_write(conn);
			break;
		case 'g':
			if (argc != 4)
				die("Need an id to get", conn);

			Database_get(conn, id);
			break;
		case 's':
			if (argc != 6)
				die("Need id, name, email to set", conn);
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if (argc != 4)
				die("Need an id to delete", conn);
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid action, only: c=create, g=get, s=set, d=delete, l=list", conn);
	}

	Database_close(conn);

	return 0;
}

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

typedef struct Address {
	int id;
	char *name;
	char *email;
} Address;

typedef struct Database {
	Address **rows;
	int max_data;
	int max_rows;
} Database;

typedef struct Connection {
	FILE *file;
	Database *db;
} Connection;

void Database_close(Connection *conn);

void die(const char *message, Connection *conn)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	Database_close(conn);

	exit(1);
}

void Address_print(Address *addr)
{
	printf("%d %s %s\n",
			addr->id, addr->name, addr->email);
}

void Address_delete(Address *addr)
{
	addr->id = -1;
	if (addr->name) {
		free(addr->name);
		addr->name = NULL;
	}
	if (addr->email) {
		free(addr->email);
		addr->email = NULL;
	}
	printf("Record deleted\n");
}

Connection *Database_new(const char *filename, int max_data, int max_rows)
{
	Connection *conn = malloc(sizeof(Connection));
	if (!conn)
		die("Memory error", conn);

	conn->db = malloc(sizeof(Database));
	if (!conn->db)
		die("Memory error", conn);

	conn->db->max_data = max_data;
	conn->db->max_rows = max_rows;
	conn->db->rows = (Address **) calloc(max_rows, sizeof(Address));
	if (!conn->db->rows)
		die("Memory error", conn);

	int i = 0;
	for (i = 0; i < max_rows; i++) {
		conn->db->rows[i] = malloc(sizeof(Address));
		conn->db->rows[i]->id = -1;
		conn->db->rows[i]->name = NULL;
		conn->db->rows[i]->email = NULL;
	}

	conn->file = fopen(filename, "w");

	return conn;
}

Connection *Database_open(const char *filename)
{
	int rc = 0;
	int i = 0;

	Connection *conn = malloc(sizeof(Connection));
	if (!conn)
		die("Memory error", conn);

	conn->db = malloc(sizeof(Database));
	if (!conn->db)
		die("Memory error", conn);

	conn->file = fopen(filename, "r+");
	if (!conn->file)
		die("Failed to open the file", conn);

	// Read db->max_data & db->max_rows
	rc = fread(&conn->db->max_data, sizeof(int), 1, conn->file);
	if (rc != 1)
		die("Failed to read max_data from database", conn);
	rc = fread(&conn->db->max_rows, sizeof(int), 1, conn->file);
	if (rc != 1)
		die("Failed to read max_rows from database", conn);

	// Read addresses
	conn->db->rows = (Address **) calloc(conn->db->max_rows, sizeof(Address));
	if (!conn->db->rows)
		die("Memory error", conn);

	int *id = malloc(sizeof(int));
	char *data_buffer = malloc(sizeof(char) * conn->db->max_data);
	int failed = 0;
	for (i = 0; i < conn->db->max_rows; i++) {
		rc = fread(id, sizeof(int), 1, conn->file);
		if (rc != 1) {
			failed = 1;
			break;
		}

		conn->db->rows[i] = malloc(sizeof(Address));
		conn->db->rows[i]->id = *id;

		rc = fread(data_buffer, sizeof(char), conn->db->max_data, conn->file);
		if (rc != conn->db->max_data) {
			failed = 1;
			break;
		}
		if (*id != -1)
			conn->db->rows[i]->name = strndup(data_buffer, conn->db->max_data);
		else
			conn->db->rows[i]->name = NULL;

		rc = fread(data_buffer, sizeof(char), conn->db->max_data, conn->file);
		if (rc != conn->db->max_data) {
			failed = 1;
			break;
		}
		if (*id != -1)
			conn->db->rows[i]->email = strndup(data_buffer, conn->db->max_data);
		else
			conn->db->rows[i]->email = NULL;
	}

	free(data_buffer);
	free(id);

	if (failed)
		die("Failed to read database", conn);

	return conn;
}

void Database_close(Connection *conn)
{
	if (conn) {
		if (conn->file)
			fclose(conn->file);
		if (conn->db) {
			if (conn->db->rows) {
				int i = 0;
				for (i = 0; i < conn->db->max_rows; i++) {
					if (conn->db->rows[i]) {
						if (conn->db->rows[i]->name)
							free(conn->db->rows[i]->name);
						if (conn->db->rows[i]->email)
							free(conn->db->rows[i]->email);
						free(conn->db->rows[i]);
					}
				}
				free(conn->db->rows);
			}
			free(conn->db);
		}
		free(conn);
	}
}

void Database_write(Connection *conn)
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

	char *data_buffer = malloc(sizeof(char) * conn->db->max_data);
	for (i = 0; i < conn->db->max_rows; i++) {
		if (conn->db->rows[i] == NULL) {
			printf("ROW IS NULL %d\n", i);
			rc = fwrite(&nope, sizeof(nope), 1, conn->file);
			if (rc != 1)
				die("Failed to write _empty_ record to the database", conn);
			//continue;
		} else {
			rc = fwrite(&conn->db->rows[i]->id, sizeof(int), 1, conn->file);
			if (rc != 1)
				die("Failed to write id to the database", conn);
		}
	
		memset(data_buffer, 0, conn->db->max_data);
		if (conn->db->rows[i]->name)
			strncpy(data_buffer, conn->db->rows[i]->name, conn->db->max_data);
		rc = fwrite(data_buffer, sizeof(char), conn->db->max_data, conn->file);
		if (rc != conn->db->max_data) {
			free(data_buffer);
			die("Failed to write name to the database", conn);
		}

		memset(data_buffer, 0, conn->db->max_data);
		if (conn->db->rows[i]->email)
			strncpy(data_buffer, conn->db->rows[i]->email, conn->db->max_data);
		rc = fwrite(data_buffer, sizeof(char), conn->db->max_data, conn->file);
		if (rc != conn->db->max_data) {
			free(data_buffer);
			die("Failed to write email to the database", conn);
		}
	}

	free(data_buffer);

	rc = fflush(conn->file);
	if (rc == -1)
		die("Cannot flush database", conn);
}

void Database_set(Connection *conn, int id, const char *name, const char *email)
{
	if (conn->db->rows[id]->id != -1)
		die("Already set, delete it first", conn);

	Address *addr = conn->db->rows[id];
	addr->id = id;
	addr->name = malloc(sizeof(char) * conn->db->max_data);
	addr->email = malloc(sizeof(char) * conn->db->max_data);
	
	char *res = strncpy(addr->name, name, conn->db->max_data);
	if (!res)
		die("Name copy failed", conn);
	addr->name[conn->db->max_data - 1] = '\0';

	res = strncpy(addr->email, email, conn->db->max_data);
	if (!res)
		die("Email copy failed", conn);
	addr->email[conn->db->max_data -1] = '\0';
}

void Database_get(Connection *conn, int id)
{
	Address *addr = conn->db->rows[id];

	if (addr)
		Address_print(addr);
	else
		die("ID is not set", conn);
}

void Database_delete(Connection *conn, int id)
{
	Address *addr = conn->db->rows[id];
	if (addr->id != -1)
		Address_delete(conn->db->rows[id]);
	else
		die("Record not set... cannot delete!", conn);
}

void Database_list(Connection *conn)
{
	int i = 0;
	int count = 0;

	printf("List of Database entries:\n------------------------\n");
	for (i = 0; i < conn->db->max_rows; i++) {
		Address *cur = conn->db->rows[i];

		if (cur && cur->id != -1) {
			Address_print(cur);
			count++;
		}
	}
	if (count == 0)
		printf("*** NO RECORDS FOUND ***\n");
}

int main(int argc, char *argv[])
{
	if (argc < 3)
		die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

	char *filename = argv[1];
	char action = argv[2][0];
	Connection *conn;
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

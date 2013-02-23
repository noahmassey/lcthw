#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NUM_FIELDS 2
#define ADDR_HEADER_SIZE (sizeof(int) * 2)
#define DB_HEADER_SIZE (sizeof(int) * 2)

#define record_size(md) ((md) * NUM_FIELDS + 2 * sizeof(int))
#define database_size(db) (record_size((db)->max_data) * (db)->max_rows + DB_HEADER_SIZE)

struct Address {
	int id;
	int set;
	const char *name;
	const char *email;
};

typedef struct {
	int set;
	int id;
	char data[];
} StoredAddress;

struct Database {
	int max_rows;
	int max_data;
	char rows[];
};

struct Connection {
	int fd;
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

void SAddress_print(StoredAddress *addr, int field_size)
{
	printf("%d %s %s\n",
		addr->id, addr->data, addr->data + field_size);
}

struct Address * Address_load(struct Database * db, StoredAddress * stored)
{
	struct Address * rvalue = malloc(sizeof(struct Address));
	rvalue->set = stored->set;
	rvalue->id = stored->id;
	rvalue->name = stored->data;
	rvalue->email = stored->data + db->max_data;
	return rvalue;
}

void Database_load(struct Connection *conn)
{
	struct Database * header = mmap(NULL, DB_HEADER_SIZE, PROT_READ, MAP_PRIVATE, conn->fd, 0);
	conn->db = mmap(NULL, database_size(header), PROT_READ | PROT_WRITE, MAP_SHARED, conn->fd, 0);
	munmap(header, DB_HEADER_SIZE);
}

struct Connection *Database_open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn)
		die(NULL, "Memory error");

	if (mode == 'c') {
		conn->fd = creat(filename, S_IRUSR|S_IWUSR);
	} else {
		conn->fd = open(filename, O_RDWR);

		if (-1 < conn->fd) {
			Database_load(conn);
		}
	}

	if (conn->fd < 0)
		die(conn, "Failed to open the file");

	return conn;
}

void Database_close(struct Connection *conn)
{
	if (conn) {
		if (conn->fd > -1) close(conn->fd);
		if (conn->db)
			munmap(conn->db, database_size(conn->db));	
		free(conn);
	}
}

void Database_write(struct Connection *conn)
{
	// bwahahahahahahah
}

void Database_create(struct Connection *conn, int max_rows, int max_data)
{
	int i;
	struct Database db = {
		.max_rows = max_rows,
		.max_data = max_data,
	};
	if (DB_HEADER_SIZE != write(conn->fd, &db, DB_HEADER_SIZE))
		die(conn, "Failed creating database");

	for (i = 0; i < max_rows; i++) {
		// make a prototype to initialize it
		StoredAddress addr = {
			.id = i,
			.set = 0,
		};
		ssize_t written = write(conn->fd, &addr, ADDR_HEADER_SIZE);
		if (written != ADDR_HEADER_SIZE)
			die(conn, "Failed creating database");
		lseek(conn->fd, NUM_FIELDS * max_data, SEEK_CUR);
	}
	// write last byte of file to ensure file size
	if (max_rows > 0) {
		char c=0;
		lseek(conn->fd, -1, SEEK_CUR);
		write(conn->fd, &c, 1);
	} 
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
	StoredAddress *addr;
	int max_data = conn->db->max_data;

	if (!(id < conn->db->max_rows))
		die(conn, "There aren't that many records");
	addr = (StoredAddress*) (conn->db->rows + (id * record_size(max_data)));
	if (addr->set)
		die(conn, "Already set, delete it first");

	addr->set = 1;

	strncpy(addr->data, name, max_data -1);
	strncpy(addr->data + max_data, email, max_data - 1);
}

void Database_get(struct Connection *conn, int id)
{
	StoredAddress *addr;

	if (!(id < conn->db->max_rows))
		die(conn, "There aren't that many records");

	addr = (StoredAddress*) (conn->db->rows + (id * record_size(conn->db->max_data)));

	if (addr->set) {
		SAddress_print(addr, conn->db->max_data);
	} else {
		die(conn, "ID is not set");
	}
}

void Database_delete(struct Connection *conn, int id)
{
	StoredAddress *addr;

	if (!(id < conn->db->max_rows))
		die(conn, "There aren't that many records");
	addr = (StoredAddress*) (conn->db->rows + (id * record_size(conn->db->max_data)));
	addr->set = 0;
	memset(addr->data, 0, NUM_FIELDS * conn->db->max_data);
}

void Database_list(struct Connection *conn)
{
	struct Database *db = conn->db;
	int max_data = db->max_data;
	int i = db->max_rows;
	StoredAddress *cur = (StoredAddress *) db->rows;

	do {
		if (cur->set) {
			SAddress_print(cur, max_data);
		}
		cur = (StoredAddress*) ((void*) cur + record_size(max_data));
	} while (--i);	
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

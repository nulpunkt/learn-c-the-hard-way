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
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database {
	struct Address rows[MAX_ROWS];
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close (struct Connection *conn) {
	assert(conn != NULL);

	if(conn->file) fclose(conn->file);
	if(conn->db) free(conn->db);
	free(conn);
}


void die(struct Connection *conn, const char *msg) {
	if(errno) {
		perror(msg);
	} else {
		printf("ERROR: %s\n", msg);
	}

	if (conn != NULL) {
		Database_close(conn);
	}

	exit(1);
}

void Address_print(struct Address *addr) {
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn) {
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die(conn, "Failed to load database");
}

struct Connection *Database_open(const char *filename, char mode) {
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn) die(conn, "Memory error");

	conn->db = malloc(sizeof(struct Database));
	if(!conn) die(conn, "Memory error");

	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");

		if (conn->file) {
			Database_load(conn);
		}
	}

	if(!conn->file) die(conn, "Filed to open file");

	return conn;
}

void Database_write(struct Connection *conn) {
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1) die(conn, "Failed to write file");

	rc = fflush(conn->file);
	if(rc == -1) die(conn, "Failed to flush file");
}

void Database_create(struct Connection *conn) {
	int i = 0;
	for(i = 0; i < MAX_ROWS; i++) {
		struct Address addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email) {
	struct Address *addr = &conn->db->rows[id];
	char *res = NULL;

	addr->set = 1;
	res = strncpy(addr->name, name, MAX_DATA);
	addr->name[MAX_DATA-1] = '\0';
	if(!res) die(conn, "Name copy failed");
	res = strncpy(addr->email, email, MAX_DATA);
	addr->email[MAX_DATA-1] = '\0';
	if(!res) die(conn, "Email copy failed");
}

void Database_get(struct Connection *conn, int id) {
	struct Address *addr = &conn->db->rows[id];
	if (addr->set) {
		Address_print(addr);
	} else {
		die(conn, "ID is not set");
	}
}

void Database_delete(struct Connection *conn, int id) {
	assert(id < MAX_ROWS);

	struct Address addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn) {
	int i = 0;
	struct Database *db = conn->db;

	for(i = 0; i < MAX_ROWS; i++)
	{
		struct Address *cur = &db->rows[i];
		if (cur->set) {
			Address_print(cur);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) die(NULL, "USAGE: ex17 <dbfile> <action> [action params]");

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;

	if(argc > 3) id = atoi(argv[3]);
	if(id >= MAX_ROWS) die(conn, "Sorry, we do not hold that many records");

	switch (action) {
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if(argc < 4) die(conn, "We need an id");
			Database_get(conn, id);
			break;
		case 's':
			if(argc != 6) die(conn, "We need an id, name and email to set");
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;
		case 'd':
			if(argc < 4) die(conn, "We need an id");
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die(conn, "Invalid action, c=create, g=get, s=set, d=delete, l=list");
	}

	Database_close(conn);

	return 0;
}

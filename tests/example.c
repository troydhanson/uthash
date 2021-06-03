#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */
#include "uthash.h"

#define NAME_SIZE 100

struct my_struct {
    int id;                    /* key */
    char name[NAME_SIZE];
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct my_struct *users = NULL;

char *get_input() {
  char *entry=NULL;
  char *newline;
  static char entry_buffer[NAME_SIZE];

  entry = fgets(entry_buffer, sizeof(entry_buffer), stdin); // Read entry
  if (entry==NULL) {
    printf("ENTRY NULL\n");
    return NULL;
  } else {
    // Clean entry from newline
    newline = strchr(entry, '\n');
    if (newline!=NULL) {
      *newline = '\0';
    } else {
      // Clear entry
      printf("Entry is too long\n");
      while (1) {
	entry = fgets(entry_buffer, sizeof(entry_buffer), stdin); // Read entry
	// Exit on error
	if ((entry[0]=='\0') || (entry[0]=='\n') || (entry==NULL) || feof(stdin)) {
	  entry = NULL;
	  break;
	}
	newline = strchr(entry, '\n');
	if (newline!=NULL) {
	  entry=NULL;
	  break;
	}
      }
    }
  }

  return entry;
}

void add_user(int user_id, char *name)
{
    struct my_struct *s;

    HASH_FIND_INT(users, &user_id, s);  /* id already in the hash? */
    if (s == NULL) {
        s = (struct my_struct*)malloc(sizeof(struct my_struct));
        s->id = user_id;
        HASH_ADD_INT(users, id, s);  /* id: name of key field */
    }
    strcpy(s->name, name);
}

struct my_struct *find_user(int user_id)
{
    struct my_struct *s;

    HASH_FIND_INT(users, &user_id, s);  /* s: output pointer */
    return s;
}

void delete_user(struct my_struct *user)
{
    HASH_DEL(users, user);  /* user: pointer to deletee */
    free(user);
}

void delete_all()
{
    struct my_struct *current_user, *tmp;

    HASH_ITER(hh, users, current_user, tmp) {
        HASH_DEL(users, current_user);  /* delete it (users advances to next) */
        free(current_user);             /* free it */
    }
}

void print_users()
{
    struct my_struct *s;

    for (s = users; s != NULL; s = (struct my_struct*)(s->hh.next)) {
        printf("user id %d: name %s\n", s->id, s->name);
    }
}

int name_sort(struct my_struct *a, struct my_struct *b)
{
    return strcmp(a->name, b->name);
}

int id_sort(struct my_struct *a, struct my_struct *b)
{
    return (a->id - b->id);
}

void sort_by_name()
{
    HASH_SORT(users, name_sort);
}

void sort_by_id()
{
    HASH_SORT(users, id_sort);
}

int main()
{
    char *in;
    int id = 1, running = 1;
    struct my_struct *s;
    unsigned num_users;

    while (running) {
        printf(" 1. add user\n");
        printf(" 2. add/rename user by id\n");
        printf(" 3. find user\n");
        printf(" 4. delete user\n");
        printf(" 5. delete all users\n");
        printf(" 6. sort items by name\n");
        printf(" 7. sort items by id\n");
        printf(" 8. print users\n");
        printf(" 9. count users\n");
        printf("10. quit\n");

	in = get_input();
	if (in==NULL) { printf("Invalid entry\n"); continue; }
        switch(atoi(in)) {
            case 1:
                printf("name?\n");
		in = get_input();
		if (in==NULL) { printf("Invalid entry\n"); continue; }
                add_user(id++, in);
                break;
            case 2:
                printf("id?\n");
		in = get_input();
		if (in==NULL) { printf("Invalid entry\n"); continue; }
                id = atoi(in);
                printf("name?\n");
		in = get_input();
		if (in==NULL) { printf("Invalid entry\n"); continue; }
                add_user(id, in);
                break;
            case 3:
                printf("id?\n");
		in = get_input();
		if (in==NULL) { printf("Invalid entry\n"); continue; }
                s = find_user(atoi(in));
                printf("user: %s\n", s ? s->name : "unknown");
                break;
            case 4:
                printf("id?\n");
		in = get_input();
		if (in==NULL) { printf("Invalid entry\n"); continue; }
                s = find_user(atoi(in));
                if (s) {
                    delete_user(s);
                } else {
                    printf("id unknown\n");
                }
                break;
            case 5:
                delete_all();
                break;
            case 6:
                sort_by_name();
                break;
            case 7:
                sort_by_id();
                break;
            case 8:
                print_users();
                break;
            case 9:
                num_users = HASH_COUNT(users);
                printf("there are %u users\n", num_users);
                break;
            case 10:
                running = 0;
                break;
        }
    }

    delete_all();  /* free any structures */
    return 0;
}

#include <stdio.h>   /* printf */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>  /* strcpy */
#include "uthash.h"

struct my_struct {
    int id;                    /* key */
    char name[21];
    UT_hash_handle hh;         /* makes this structure hashable */
};

struct my_struct *users = NULL;

void add_user(int user_id, const char *name)
{
    struct my_struct *s;

    HASH_FIND_INT(users, &user_id, s);  /* id already in the hash? */
    if (s == NULL) {
        s = (struct my_struct*)malloc(sizeof *s);
        s->id = user_id;
        HASH_ADD_INT(users, id, s);  /* id is the key field */
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
    struct my_struct *current_user;
    struct my_struct *tmp;

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

int by_name(const struct my_struct *a, const struct my_struct *b)
{
    return strcmp(a->name, b->name);
}

int by_id(const struct my_struct *a, const struct my_struct *b)
{
    return (a->id - b->id);
}

const char *getl(const char *prompt)
{
    static char buf[21];
    char *p;
    printf("%s? ", prompt); fflush(stdout);
    p = fgets(buf, sizeof(buf), stdin);
    if (p == NULL || (p = strchr(buf, '\n')) == NULL) {
        puts("Invalid input!");
        exit(EXIT_FAILURE);
    }
    *p = '\0';
    return buf;
}

int main()
{
    int id = 1;
    int running = 1;
    struct my_struct *s;
    int temp;

    while (running) {
        printf(" 1. add user\n");
        printf(" 2. add or rename user by id\n");
        printf(" 3. find user\n");
        printf(" 4. delete user\n");
        printf(" 5. delete all users\n");
        printf(" 6. sort items by name\n");
        printf(" 7. sort items by id\n");
        printf(" 8. print users\n");
        printf(" 9. count users\n");
        printf("10. quit\n");
        switch (atoi(getl("Command"))) {
            case 1:
                add_user(id++, getl("Name (20 char max)"));
                break;
            case 2:
                temp = atoi(getl("ID"));
                add_user(temp, getl("Name (20 char max)"));
                break;
            case 3:
                s = find_user(atoi(getl("ID to find")));
                printf("user: %s\n", s ? s->name : "unknown");
                break;
            case 4:
                s = find_user(atoi(getl("ID to delete")));
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
                HASH_SORT(users, by_name);
                break;
            case 7:
                HASH_SORT(users, by_id);
                break;
            case 8:
                print_users();
                break;
            case 9:
                temp = HASH_COUNT(users);
                printf("there are %d users\n", temp);
                break;
            case 10:
                running = 0;
                break;
        }
    }

    delete_all();  /* free any structures */
    return 0;
}

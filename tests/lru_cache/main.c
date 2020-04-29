#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cache.h"

#define MAX_RANDOM_ENTRIES	32

struct key_record {
	char *key;
	char *value;
};

int generate_random_entry(struct key_record **entry);
int generate_random_string(char **dst, const size_t len);
void free_random_entry(void *entry);

void *producer(void *arg)
{
	struct foo_cache *cache = arg;
	int i;

	for (i = 0; i < MAX_RANDOM_ENTRIES; i++) {
		struct key_record *entry = NULL;
		if (generate_random_entry(&entry)) {
			fprintf(stderr, "generate_random_entry() failed\n");
			continue;
		}
#if defined(DEBUG)
		printf("Random Entry:\n");
		printf("  key: %s\n", entry->key);
		printf("   Key: %s\n", entry->value);
#else
		printf("inserted %s (%d)\n", entry->key,
		       (int)strlen(entry->key));
#endif
		if (foo_cache_insert(cache, entry->key, entry)) {
			fprintf(stderr, "foo_cache_insert() failed\n");
			continue;
		}
	}

	pthread_exit(NULL);
}

void *consumer(void *arg)
{
	struct foo_cache *cache = arg;
	struct key_record *result = NULL;
	char *buffer = malloc(64);
	char key[33];
	int stop = 0;

	if (!buffer)
		goto out;

	/* give producer time to populate the cache */
	sleep(2);
	printf("\n\n");

	do {
		memset(key, 0, 64);
		result = NULL;

		printf("Enter key for lookup: ");
		fgets(buffer, sizeof(key), stdin);
		sscanf(buffer, "%s\n", key);
		/* read '\n' from stdin */
		getchar();

		if (strncmp(key, "exit", 4) == 0) {
			stop = 1;
			continue;
		}

		printf("Got key %s (%d)\n", key, (int)strlen(key));

		if (foo_cache_lookup(cache, key, &result)) {
			fprintf(stderr, "Could not retrieve key %s\n", key);
			continue;
		}

		if (!result) {
			printf("MISS\n");
			continue;
		}

		printf("HIT\n");
		printf("key: %s\n", result->key);
		printf("key : %s\n", result->value);
	} while (!stop);

out:
	if (buffer)
		free(buffer);
	pthread_exit(NULL);
}

int main()
{
	int rv;
	struct foo_cache *cache = NULL;
	pthread_t workers[2];

	rv = foo_cache_create(&cache, MAX_RANDOM_ENTRIES / 2,
			      free_random_entry);
	if (rv) {
		fprintf(stderr, "Could not create cache\n");
		exit(1);
	}

	(void)pthread_create(&workers[0], NULL, producer, (void *)cache);
	(void)pthread_create(&workers[1], NULL, consumer, (void *)cache);

	pthread_join(workers[0], NULL);
	pthread_join(workers[1], NULL);

	(void)foo_cache_delete(cache, 0);
	return 0;
}

int generate_random_entry(struct key_record **entry)
{
	struct key_record *new = NULL;
	char *key = NULL;
	char *value = NULL;
	int rv;

	if (!entry)
		return EINVAL;

	rv = generate_random_string(&key, 33);
	if (rv)
		return rv;

	rv = generate_random_string(&value, 129);
	if (rv)
		return rv;

	if ((new = malloc(sizeof(*new))) == NULL) {
		free(key);
		free(value);
		return ENOMEM;
	}

	new->key = key;
	new->value = value;

	*entry = new;
	return 0;
}

int generate_random_string(char **dst, const size_t len)
{
	static const char alphanum[] =
	    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	size_t i;
	char *s;

	if (!dst || len == 0)
		return EINVAL;

	if ((s = malloc(len)) == NULL)
		return ENOMEM;

	for (i = 0; i < len - 1; i++) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len - 1] = '\0';
	*dst = s;
	return 0;
}

void free_random_entry(void *entry)
{
#if defined(DEBUG)
	fprintf(stderr, "In %s: entry @ %p\n", __func__, entry);
#endif
	struct key_record *record = entry;
	if (!record)
		return;
	if (record->key)
		free(record->key);
	if (record->value)
		free(record->value);
	free(record);
	record = NULL;
}

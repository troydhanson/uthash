/*
 * =====================================================================================
 *
 *       Filename:  cache.c
 *
 *    Description:  A simple cache
 *
 *        Version:  1.0
 *        Created:  04/11/2013 02:31:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Oliver Lorenz (ol), olli@olorenz.org
 *        Company:  https://olorenz.org
 *        License:  This is licensed under the same terms as uthash itself
 *
 * =====================================================================================
 */

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include "cache.h"
#include "uthash.h"

/**
 * A cache entry
 */
struct foo_cache_entry {
	char *key; /**<The key */
	void *data; /**<Payload */
	UT_hash_handle hh; /**<Hash Handle for uthash */
};
#define KEY_MAX_LENGTH		32

/**
 * A cache object
 */
struct foo_cache {
	size_t max_entries; /**<Amount of entries this cache object can hold */
	pthread_rwlock_t cache_lock; /**<A lock for concurrent access */
	struct foo_cache_entry *entries; /**<Head pointer for uthash */
	void (*free_cb) (void *element);/**<Callback function to free cache entries */
};

/** Creates a new cache object

    @param dst
    Where the newly allocated cache object will be stored in

    @param capacity
    The maximum number of elements this cache object can hold

    @return EINVAL if dst is NULL, ENOMEM if malloc fails, 0 otherwise
*/
int foo_cache_create(struct foo_cache **dst, const size_t capacity,
		     void (*free_cb) (void *element))
{
	struct foo_cache *new = NULL;
	int rv;

	if (!dst)
		return EINVAL;

	if ((new = malloc(sizeof(*new))) == NULL)
		return ENOMEM;

	if ((rv = pthread_rwlock_init(&(new->cache_lock), NULL)) != 0)
		goto err_out;

	new->max_entries = capacity;
	new->entries = NULL;
	new->free_cb = free_cb;
	*dst = new;
	return 0;

err_out:
	if (new)
		free(new);
	return rv;
}

/** Frees an allocated cache object

    @param cache
    The cache object to free

    @param keep_data
    Whether to free contained data or just delete references to it

    @return EINVAL if cache is NULL, 0 otherwise
*/
int foo_cache_delete(struct foo_cache *cache, int keep_data)
{
	struct foo_cache_entry *entry, *tmp;
	int rv;

	if (!cache)
		return EINVAL;

	rv = pthread_rwlock_wrlock(&(cache->cache_lock));
	if (rv)
		return rv;

	if (keep_data) {
		HASH_CLEAR(hh, cache->entries);
	} else {
		HASH_ITER(hh, cache->entries, entry, tmp) {
			HASH_DEL(cache->entries, entry);
			if (cache->free_cb)
				cache->free_cb(entry->data);
			free(entry);
		}
	}
	(void)pthread_rwlock_unlock(&(cache->cache_lock));
	(void)pthread_rwlock_destroy(&(cache->cache_lock));
	free(cache);
	cache = NULL;
	return 0;
}

/** Checks if a given key is in the cache

    @param cache
    The cache object

    @param key
    The key to look-up

    @param result
    Where to store the result if key is found.

    A warning: Even though result is just a pointer,
    you have to call this function with a **ptr,
    otherwise this will blow up in your face.

    @return EINVAL if cache is NULL, 0 otherwise
*/
int foo_cache_lookup(struct foo_cache *cache, char *key, void *result)
{
	int rv;
	struct foo_cache_entry *tmp = NULL;
	char **dirty_hack = result;

	if (!cache || !key || !result)
		return EINVAL;

	rv = pthread_rwlock_wrlock(&(cache->cache_lock));
	if (rv)
		return rv;

	HASH_FIND_STR(cache->entries, key, tmp);
	if (tmp) {
		size_t key_len = strnlen(tmp->key, KEY_MAX_LENGTH);
		HASH_DELETE(hh, cache->entries, tmp);
		HASH_ADD_KEYPTR(hh, cache->entries, tmp->key, key_len, tmp);
		*dirty_hack = tmp->data;
	} else {
		*dirty_hack = result = NULL;
	}
	rv = pthread_rwlock_unlock(&(cache->cache_lock));
	return rv;
}

/** Inserts a given <key, value> pair into the cache

    @param cache
    The cache object

    @param key
    The key that identifies <value>

    @param data
    Data associated with <key>

    @return EINVAL if cache is NULL, ENOMEM if malloc fails, 0 otherwise
*/
int foo_cache_insert(struct foo_cache *cache, char *key, void *data)
{
	struct foo_cache_entry *entry = NULL;
	struct foo_cache_entry *tmp_entry = NULL;
	size_t key_len = 0;
	int rv;

	if (!cache || !data)
		return EINVAL;

	if ((entry = malloc(sizeof(*entry))) == NULL)
		return ENOMEM;

	if ((rv = pthread_rwlock_wrlock(&(cache->cache_lock))) != 0)
		goto err_out;

	entry->key = key;
	entry->data = data;
	key_len = strnlen(entry->key, KEY_MAX_LENGTH);
	HASH_ADD_KEYPTR(hh, cache->entries, entry->key, key_len, entry);

	if (HASH_COUNT(cache->entries) >= cache->max_entries) {
		HASH_ITER(hh, cache->entries, entry, tmp_entry) {
			HASH_DELETE(hh, cache->entries, entry);
			if (cache->free_cb)
				cache->free_cb(entry->data);
			else
				free(entry->data);
			/* free(key->key) if data has been copied */
			free(entry);
			break;
		}
	}

	rv = pthread_rwlock_unlock(&(cache->cache_lock));
	return rv;

err_out:
	if (entry)
		free(entry);
	(void)pthread_rwlock_unlock(&(cache->cache_lock));
	return rv;

}

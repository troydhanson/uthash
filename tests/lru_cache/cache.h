/*
 * =====================================================================================
 *
 *       Filename:  cache.h
 *
 *    Description:  A simple cache
 *
 *        Version:  1.0
 *        Created:  04/11/2013 02:30:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Oliver Lorenz (ol), olli@olorenz.org
 *        Company:  https://olorenz.org
 *        License:  This is licensed under the same terms as uthash itself
 *
 * =====================================================================================
 */

#ifndef _CACHE_
#define _CACHE_

struct foo_cache;

extern int foo_cache_create(struct foo_cache **dst, const size_t capacity,
			    void (*free_cb) (void *element));
extern int foo_cache_delete(struct foo_cache *cache, int keep_data);
extern int foo_cache_lookup(struct foo_cache *cache, char *key, void *result);
extern int foo_cache_insert(struct foo_cache *cache, char *key, void *data);

#endif

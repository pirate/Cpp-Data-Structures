#ifndef HASHTABLE_H
#define HASHTABLE_H

#ifdef __cplusplus
extern "C"
{
#endif

	typedef int (*hashtable_hash_function)(void *key, int size);
	typedef int (*hashtable_len_function)(void *key);

	typedef struct
	{
		int keyHash;
		void *value;
	} hashtableentry_t;

	typedef struct
	{
		hashtableentry_t *entries;

		hashtable_hash_function hash;
		hashtable_len_function len;

		int entry_count;
	} hashtable_t;

	// Hashtable functions
	void hashtable_init(hashtable_t *hashtable, hashtable_hash_function hash, hashtable_len_function len);
	void hashtable_destroy(hashtable_t *hashtable);

	void hashtable_set(hashtable_t *hashtable, void *key, void *value);
	int hashtable_get(hashtable_t *hashtable, void *key, void **value);
	void hashtable_remove(hashtable_t *hashtable, void *key);

	int hashtable_count(hashtable_t *hashtable);
	void hashtable_clear(hashtable_t *hashtable);

	// General hash functions
	int hashtable_hash_fnv(void *key, int size);

#ifdef __cplusplus
}
#endif

#endif

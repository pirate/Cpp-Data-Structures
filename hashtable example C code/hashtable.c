#include "hashtable.h"

#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
int hashtable_find(hashtable_t *hashtable, int keyHash)
{
	int i = 0;

	for (; i < hashtable->entry_count; i++)
	{
		if (hashtable->entries[i].keyHash == keyHash)
			return i;
	}

	return -1;
}

hashtableentry_t *hashtable_expand(hashtable_t *hashtable, int count)
{
	int oldLen = hashtable->entry_count * sizeof(hashtableentry_t);

	if (count == 0)
		return (hashtableentry_t *)((char *)hashtable->entries + oldLen);

	hashtable->entry_count += count;
	if (hashtable->entry_count <= 0)
	{
		hashtable->entries = NULL;
		hashtable->entry_count = 0;
		return;
	}
	hashtable->entries = (hashtableentry_t *)realloc(hashtable->entries, hashtable->entry_count * sizeof(hashtableentry_t));

	return (hashtableentry_t *)((char *)hashtable->entries + oldLen);
}

void hashtable_init(hashtable_t *hashtable, hashtable_hash_function hash, hashtable_len_function len)
{
	if (!hashtable)
		return;

	hashtable->entries = NULL;
	hashtable->entry_count = 0;

	hashtable->hash = hash;
	hashtable->len = len;
}

void hashtable_destroy(hashtable_t *hashtable)
{
	hashtable_clear(hashtable);
}

void hashtable_set(hashtable_t *hashtable, void *key, void *value)
{
	int keyHash = hashtable->hash(key, hashtable->len(key));
	hashtableentry_t *ptr;
	int i;

	if ((i = hashtable_find(hashtable, keyHash)) != -1)
	{
		hashtable->entries[i].value = value;
		return;
	}

	ptr = hashtable_expand(hashtable, 1);
	ptr->keyHash = keyHash;
	ptr->value = value;
}

int hashtable_get(hashtable_t *hashtable, void *key, void **value)
{
	int keyHash = hashtable->hash(key, hashtable->len(key));
	int i;
	
	if ((i = hashtable_find(hashtable, keyHash)) != -1)
	{
		if (value)
			*value = hashtable->entries[i].value;

		return 1;
	}

	return 0;
}

void hashtable_remove(hashtable_t *hashtable, void *key)
{
	int keyHash = hashtable->hash(key, hashtable->len(key));
	int i;

	if ((i = hashtable_find(hashtable, keyHash)) != -1)
	{
		int totalLen = (hashtable->entry_count - i) * sizeof(hashtableentry_t);

		if (totalLen > 0)
			memcpy(&hashtable->entries[i], &hashtable->entries[i + 1], totalLen);

		hashtable_expand(hashtable, -1);
	}
}

int hashtable_count(hashtable_t *hashtable)
{
	return hashtable->entry_count;
}

void hashtable_clear(hashtable_t *hashtable)
{
	if (hashtable->entries)
	{
		free(hashtable->entries);
		hashtable->entries = NULL;
	}

	hashtable->entry_count = 0;
}

int hashtable_hash_fnv(void *key, int size)
{
	unsigned int hval = 0x811c9dc5;
	unsigned char *bp = (unsigned char *)key;
    unsigned char *be = bp + size;

    while (bp < be) 
	{
		hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
		hval ^= (unsigned int)*bp++;
    }

    return (int)hval;
}

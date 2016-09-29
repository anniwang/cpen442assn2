#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "crc.h"

//#define MYSTRING "AB9D30BC3893371A081D18A536FB1483"
#define MY_CRC 0x7eb54af3

/*HASH TABLE: https://gist.github.com/tonious/1377667*/
// struct entry_s {
// 	char *key;
// 	char *value;
// 	struct entry_s *next;
// };
// typedef struct entry_s entry_t;

// struct hashtable_s {
// 	int size;
// 	struct entry_s **table;
// };
// typedef struct hashtable_s hashtable_t;

// hashtable_t *ht_create( int size ) {

// 	hashtable_t *hashtable = NULL;
// 	int i;

// 	if ( size < 1 ) return NULL;

// 	/* Allocate the table itself. */
// 	if ( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
// 		return NULL;
// 	}

// 	/* Allocate pointers to the head nodes. */
// 	if ( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
// 		return NULL;
// 	}
// 	for ( i = 0; i < size; i++ ) {
// 		hashtable->table[i] = NULL;
// 	}

// 	hashtable->size = size;

// 	return hashtable;
// }

// int ht_hash( hashtable_t *hashtable, char *key ) {

// 	unsigned long int hashval;
// 	int i = 0;

// 	/* Convert our string to an integer */
// 	while ( hashval < ULONG_MAX && i < strlen( key ) ) {
// 		hashval = hashval << 8;
// 		hashval += key[ i ];
// 		i++;
// 	}

// 	return hashval % hashtable->size;
// }

// entry_t *ht_newpair( char *key, char *value ) {
// 	entry_t *newpair;

// 	if ( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
// 		return NULL;
// 	}

// 	if ( ( newpair->key = strdup( key ) ) == NULL ) {
// 		return NULL;
// 	}

// 	if ( ( newpair->value = strdup( value ) ) == NULL ) {
// 		return NULL;
// 	}

// 	newpair->next = NULL;

// 	return newpair;
// }

// void ht_set( hashtable_t *hashtable, char *key, char *value ) {
// 	int bin = 0;
// 	entry_t *newpair = NULL;
// 	entry_t *next = NULL;
// 	entry_t *last = NULL;

// 	bin = ht_hash( hashtable, key );

// 	next = hashtable->table[ bin ];

// 	while ( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
// 		last = next;
// 		next = next->next;
// 	}

// 	/* There's already a pair.  Let's replace that string. */
// 	if ( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

// 		free( next->value );
// 		next->value = strdup( value );

// 		/* Nope, could't find it.  Time to grow a pair. */
// 	} else {
// 		newpair = ht_newpair( key, value );

// 		/* We're at the start of the linked list in this bin. */
// 		if ( next == hashtable->table[ bin ] ) {
// 			newpair->next = next;
// 			hashtable->table[ bin ] = newpair;

// 			/* We're at the end of the linked list in this bin. */
// 		} else if ( next == NULL ) {
// 			last->next = newpair;

// 			/* We're in the middle of the list. */
// 		} else  {
// 			newpair->next = next;
// 			last->next = newpair;
// 		}
// 	}
// }

// /* Retrieve a key-value pair from a hash table. */
// char *ht_get( hashtable_t *hashtable, char *key ) {
// 	int bin = 0;
// 	entry_t *pair;

// 	bin = ht_hash( hashtable, key );

// 	/* Step through the bin, looking for our value. */
// 	pair = hashtable->table[ bin ];
// 	while ( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
// 		pair = pair->next;
// 	}

// 	/* Did we actually find anything? */
// 	if ( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
// 		return NULL;

// 	} else {
// 		return pair->value;
// 	}

// }

/*END HASH TABLE*/

// random string generator from stack overflow...
char *randstring(int length) {
	char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	size_t stringLen = 26 * 2 + 10 + 7;
	char *randomString;

	randomString = malloc(sizeof(char) * (length + 1));

	if (!randomString) {
		return (char*)0;
	}

	unsigned int key = 0;

	for (int n = 0; n < length; n++) {
		key = rand() % stringLen;
		randomString[n] = string[key];
	}

	randomString[length] = '\0';

	return randomString;
}

int main(void)
{
	char* str = "abcdefghijklmnopqrstuvwxyz012345";
	crc_t crc;
	int i = 0;

	//hashtable_t *hashtable = ht_create( 65536 );
	/*
	ht_set( hashtable, "key1", "inky" );
	ht_set( hashtable, "key2", "pinky" );
	printf( "%s\n", ht_get( hashtable, "key1" ) );
	printf( "%s\n", ht_get( hashtable, "key2" ) );
	*/
	crc = crc_init();
	crc = crc_update(crc, (unsigned char *)str, strlen(str));
	crc = crc_finalize(crc);

	while (1) {

		if (crc == MY_CRC || i >= 1000000) {
			break;
		}

		else {

			str = randstring(strlen(str));
			i++;
			//printf("%d: crc= 0x%lx, %s\n", i, crc, str);
			//break;
			crc = crc_init();
			crc = crc_update(crc, (unsigned char *)str, strlen(str));
			crc = crc_finalize(crc);
		}
	}

	printf("0x%lx = 0x%lx; string is '%s'\n", (unsigned long)crc, MY_CRC, str);

	/*char* generated = randstring(strlen(str));
	printf("generated string: %s\n", generated);
	generated = randstring(strlen(str));
	printf("generated string: %s\n", generated);
	generated = randstring(strlen(str));
	printf("generated string: %s\n", generated);*/


	return 0;
}

/*
RESULTS for 123456789

$ ./crc
0xcbf43926

$ python pycrc.py --model crc-32 --check-string 123456789
0xcbf43926


 */
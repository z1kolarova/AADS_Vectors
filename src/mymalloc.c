/**
 * \file    mymalloc.c
 * \author  Dulik
 * \brief   Module that allocates and track the memory.
 *
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/* Includes --------------------------------------------------------------------------------------*/
#include "mymalloc.h"

#include <stdlib.h>

#ifndef NDEBUG
  #include <stdint.h>
  #include <stdio.h>

/* Private types ---------------------------------------------------------------------------------*/
typedef void *tHTableKey;          /* typ vyhledávacího klice = ukazatel */
typedef size_t tHTableData;        /* typ dat uloženych pod hledanym klicem = velikost
                                      alokované pameti*/
typedef unsigned int tHTableIndex; /* index do vnitřního pole hash tabulky */

typedef struct tHTableNode_s {
  tHTableKey key;             /* vyhledávací klíč */
  tHTableData data;           /* data */
  struct tHTableNode_s *next; /* next node */
} tHTableNode;

/* Private macros --------------------------------------------------------------------------------*/
/* počet položek pole hash, musí být prvočislo */
  #define HASH_TABLE_SIZE 101

/* funkce pro porovnani 2 klíčů */
  #define compEQ(a, b) (a == b)

/* Private variables -----------------------------------------------------------------------------*/
/* jednoduchá hashovaci tabulka=pole ukazatelu na uzel */
static tHTableNode *mHashTable[HASH_TABLE_SIZE];
static size_t mTotalAllocated = 0;

/* Private function declarations -----------------------------------------------------------------*/
static tHTableIndex hashFn(tHTableKey key);
static tHTableNode *insertHTableNode(tHTableKey key, tHTableData data);
static tHTableData deleteNode(tHTableKey key);
static tHTableData *findNodeDataRef(tHTableKey key);
#endif

/* Exported functions definitions ----------------------------------------------------------------*/
/*! \defgroup mymalloc MyMalloc
 *  \brief The MYMALLOC library is used to debug memory allocation. For
 *  debugging, we need to keep a record of each allocated block of dynamic
 *  memory so that we know how big it is when it is released.\n\n
 *  Each time we allocate memory, we will print out information about the size
 *  of the memory currently allocated and the total memory allocated, and we
 *  will print the same information when we release it. We will store the
 *  records of allocated blocks (= pointer x size of the block it points to) in
 *  a hash table.\n\n
 *  The hash table has the simplest possible implementation - i.e. explicit
 *  concatenation of synonyms into a linear list, a simple hash function of type
 *  "modulo".
 *
 *  If macro \a NDEBUG is defined, the implementation is simple wrapper around
 * the standard calls.
 *
 * \{
 */

/*! Allocates the dynamic memory. This wraps the \ref malloc call from standard
 * library.
 *
 * \param[in] size  Size that is requested to allocate.
 *
 * \return Returns NULL on failure, pointer to the allocated memory otherwise.
 */
void *myMalloc(size_t size)
{
  void *tmpUk = malloc(size);
#ifndef NDEBUG
  if (tmpUk != NULL) {
    insertHTableNode(tmpUk, size);
    mTotalAllocated += size;
    printf("myMalloc: allocating %zu bytes, memory allocated %zu bytes\n", size, mTotalAllocated);
  }
#endif
  return tmpUk;
}

/*! Frees the allocated memory. This wraps the \ref free call from standard
 * library.
 *
 * \param[in] memblock  Pointer to the memory to free.
 */
void myFree(void *memblock)
{
#ifndef NDEBUG
  size_t size = 0;

  if (memblock != NULL) {
    size = deleteNode(memblock);
    mTotalAllocated -= size;
    printf("myFree: releasing %zu bytes, memory allocated %zu bytes\n", size, mTotalAllocated);
  }

#endif
  free(memblock);
}

/*! Reallocate the memory. This wraps the \ref realloc call from standard
 * library.
 *
 * \param[in] ptr     Pointer to memory to reallocate.
 * \param[in] newSize New size that is requested to allocate.
 *
 * \return Returns NULL on failure, pointer to the allocated memory otherwise.
 */
void *myRealloc(void *ptr, size_t newSize)
{
  void *tPtr = realloc(ptr, newSize);
#ifndef NDEBUG

  // ukazatel se nezmenil. Nova pamet se vlezla do volneho mista za nebo doslo
  // ke zmenseni pameti
  if (tPtr == ptr) {
    tHTableData *size = findNodeDataRef(ptr);
    mTotalAllocated += newSize - *size;

    if (*size > newSize) {
      printf("myRealloc: releasing %zu bytes, memory allocated %zu bytes\n",
             (*size - newSize),
             mTotalAllocated);
    } else {
      printf("myRealloc: allocating %zu bytes, memory allocated %zu bytes\n",
             (newSize - *size),
             mTotalAllocated);
    }

    *size = newSize;
  } else if (ptr == NULL && tPtr != NULL) {  // kdyz je realloc volan s null na vstupu
    insertHTableNode(tPtr, newSize);
    mTotalAllocated += newSize;
    printf(
      "myRealloc: allocating %zu bytes, memory allocated %zu bytes\n", newSize, mTotalAllocated);
  } else if (tPtr != NULL) {  // uspesne alokovano nove misto
    size_t size = newSize - deleteNode(ptr);
    insertHTableNode(tPtr, newSize);
    mTotalAllocated += size;
    printf("myRealloc: allocating %zu bytes, memory allocated %zu bytes\n", size, mTotalAllocated);
  }

#endif
  return tPtr;
}

/* Private function definitions ------------------------------------------------------------------*/
#ifndef NDEBUG
/*! Hash function that converts key to index to the table.
 *
 * \param[in] key
 *
 * \return Returns the computed index.
 */
static tHTableIndex hashFn(tHTableKey key)
{
  return ((uintptr_t)key % HASH_TABLE_SIZE);
}

/*! Creates new node, fills it with data and insert in into the table at the
 * first position of the synonym list.
 *
 * \param[in] key   Key of the node.
 * \param[in] data  Data of the node.
 *
 * \return Returns pointer to the created node.
 */
static tHTableNode *insertHTableNode(tHTableKey key, tHTableData data)
{
  tHTableNode *first, *ukTmp;
  tHTableIndex index;
  index = hashFn(key); /* hash fce vrátí index do pole hash tab. */

  /* nyní vytvoříme v dyn. paměti nový uzel */
  if ((first = malloc(sizeof(tHTableNode))) == NULL) {
    fprintf(stderr, "out of memory (insertHTableNode)\n");
    exit(1); /* pokud se alokace nepodařila, konec */
  }

  /* Zde uděláme klasický list_insert_first jako v lineárním seznamu */
  ukTmp = mHashTable[index];  // schováme si ukazatel na začátek seznamu synonym
  mHashTable[index] = first;  /* a přepíšeme jej ukazatelem na nový 1. uzel */
  first->next = ukTmp;        /* na nový první uzel navážeme starý 1. uzel */
  first->key = key;           /* a naplníme klíč a data hodnotami */
  first->data = data;
  return first;
}

/*! Delete node from the hash table.
 *
 * \param[in] key Key of the node that will be deleted.
 *
 * \return Returns data of the deleted node.
 */
static tHTableData deleteNode(tHTableKey key)
{
  tHTableNode *ukAct, *ukPrev = NULL;
  tHTableIndex index;
  tHTableData data;
  /* nejdříve spočítáme index, kde začíná lin. seznam synonym */
  index = hashFn(key);
  ukAct = mHashTable[index];

  while (ukAct && !compEQ(ukAct->key, key)) {
    ukPrev = ukAct;
    ukAct = ukAct->next;
  }

  if (ukAct == NULL) {
    return 0;
  }

  if (ukPrev == NULL) /* mažeme první uzel seznamu synonym ? */
  {
    mHashTable[index] = ukAct->next;
  }    /* ano: nastav ukazatel na první prvek na další za mazaným */
  else /* ne: propoj prvek před mazaným s prvkem za mazaným */
  {
    ukPrev->next = ukAct->next;
  }

  data = ukAct->data;
  free(ukAct);
  return data;
}

/*! Look up the node in the table and returns reference to it.
 *
 * \param[in] key Key of the node to look for.
 *
 * \return Returns pointer to the nodes data.
 */
static tHTableData *findNodeDataRef(tHTableKey key)
{
  tHTableNode *uk;
  uk = mHashTable[hashFn(key)];

  while (uk && !compEQ(uk->key, key)) {
    uk = uk->next;
  }

  return &uk->data;
}
#endif
/*! \} */

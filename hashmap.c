#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

void insertMap(HashMap * map, char * key, void * value) {

  int pos;
  pos = hash(key,map->capacity);
  while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL){
    pos = (pos+1)% map->capacity;
  }  
  map->buckets[pos] = createPair (key,value);  
  map->size++;
  map->current = pos;
}

void enlarge(HashMap * map) {
  
  enlarge_called = 1; //no borrar (testing purposes)
  int pos=0;
  int cap2 = map->capacity;
  Pair ** aux = map->buckets;
  map -> capacity *= 2;
  map->buckets=(Pair*) calloc (sizeof(Pair)*map->capacity);
  map->size=0;

  while(i<cap2){
    if(aux[pos]!=NULL){
      insertMap (map,aux[pos]->key,aux[pos]->value);
    }
  }
  free (aux);
}
HashMap * createMap(long capacity) {

  int i = 0;
  HashMap * mapa = malloc(sizeof(HashMap));
  mapa->buckets = malloc(sizeof(Pair*) * capacity);
  mapa->size = 0;
  for(i=0 ; i<capacity ; i++)
  mapa->buckets[i] = NULL;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
  }

void eraseMap(HashMap * map,  char * key) {    
  
  int pos;
  pos = hash(key,map->capacity);

  if (is_equal (map->buckets[pos]->key, key)){
    map->size--;
    map->buckets[pos]->key = NULL;
  }
  else {
    while (map->buckets[pos] != NULL){
      if (is_equal (map->buckets[pos]->key, key)){
        map->size--;
        map->buckets[pos]->key = NULL; 
      }
      pos = (pos+1)% map->capacity;
    }
  }
}
Pair * searchMap(HashMap * map,  char * key) {   

  int pos;
  pos = hash(key,map->capacity);

  if (is_equal (map->buckets[pos]->key, key)){
    map->current = pos;
    return map->buckets[pos];
    }
  else {
    while (map->buckets[pos] != NULL &&
      is_equal(map->buckets[pos]->key,key) == 0){
      pos = (pos+1)% map->capacity;
    }
    map->current = pos;  
    return map->buckets[pos];      
  }
}

Pair * firstMap(HashMap * map) {

  int pos = 0;
  while (map->buckets[pos] == NULL || map->buckets[pos]->key == NULL){
    pos = (pos+1)% map->capacity;  
  }
  map->current = pos;
  return map->buckets[pos]; 
  }

Pair * nextMap(HashMap * map) {

  int pos;
  pos = map->current;
  pos = (pos+1)% map->capacity; 
  
  if (pos+1 == map->capacity) return NULL;
  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL){
    pos = (pos+1)% map->capacity;
    
  }
  map->current = pos;
  return map->buckets[pos];
}

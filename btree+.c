#include "btree+.h"

int global_node_rrn = 0;

LeafNode* createLeafNode() {
  LeafNode* newLeafNode = (LeafNode*) malloc(sizeof(LeafNode));

  newLeafNode->rrn = global_node_rrn++;
  newLeafNode->next = NULL;
  newLeafNode->num_keys = 0;
  for (int i = 0; i < N-1; i++)
    memset(newLeafNode->keys[i], '\0', LICENSE_PLATE_SIZE);

  return newLeafNode; 
}

int insertInLeafNode(LeafNode** node, const char* key) {
  if ((*node) == NULL)
    (*node) = createLeafNode();
  
  if ((*node)->num_keys == N - 1) 
    return OVERFLOW;

  int i = 0;
  while (strcmp((*node)->keys[i], key) > 0) 
    i++;

  if (i < (*node)->num_keys) { //needs to adjust the keys
    for (int j = (*node)->num_keys; j > i; j--) 
      strcpy((*node)->keys[j], (*node)->keys[j-1]);
  }
  strcpy((*node)->keys[i], key);

  return NOT_OVERFLOW;
}
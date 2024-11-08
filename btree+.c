#include "btree+.h"
#include <stdio.h>
#include <string.h>

int global_node_rrn = 0;

const int OVERFLOW = 1;
const int NOT_OVERFLOW = 0;

LeafNode* createLeafNode() {
  LeafNode* newLeafNode = (LeafNode*) malloc(sizeof(LeafNode));

  newLeafNode->rrn = global_node_rrn++;
  newLeafNode->next_rrn = -1;
  newLeafNode->num_keys = 0;
  for (int i = 0; i < N-1; i++)
    memset(newLeafNode->keys[i], '\0', LICENSE_PLATE_SIZE);

  return newLeafNode; 
}

InternalNode* createInternalNode() {
  InternalNode* newInternalNode = (InternalNode*) malloc(sizeof(InternalNode));

  newInternalNode->rrn = global_node_rrn++;
  newInternalNode->is_pseudo_leaf = false;
  newInternalNode->num_keys = 0;
  for (int i = 0; i < M-1; i++) {
    memset(newInternalNode->keys[i], '\0', LICENSE_PLATE_SIZE);
    newInternalNode->children[i] = newInternalNode->leafs[i] = -1;
  }
  newInternalNode->children[M-1] = newInternalNode->leafs[M-1] = -1;
  
  return newInternalNode;
}

int insertInLeafNode(LeafNode** node, const char* key) {
  // if ((*node) == NULL)
  //   (*node) = createLeafNode();
  
  if ((*node)->num_keys == N - 1) 
    return OVERFLOW;

  int i = 0;
  while (strcmp(key, (*node)->keys[i]) > 0 && (*node)->keys[i][0] != '\0') 
    i++;
  
  //printf("[DEBUG] inserted index: %d\n", i);
  if (i < (*node)->num_keys) { //needs to adjust the keys
    for (int j = (*node)->num_keys; j > i; j--) 
      strcpy((*node)->keys[j], (*node)->keys[j-1]);
  }
  strcpy((*node)->keys[i], key);
  (*node)->num_keys++;
  printf("[DEBUG] inserted: %s", (*node)->keys[i]);

  return NOT_OVERFLOW;
}

void temporaryBuildBTreePlus () {
  char key[LICENSE_PLATE_SIZE];
  int action = 0;
  LeafNode* rootLeaf = createLeafNode();
  
  printf("acao: ");
  scanf(" %d", &action);
  while (action == 1) {
    printf("digite uma chave: ");
    scanf(" %s", key);
    printf("%d\n", rootLeaf->next_rrn);
    if (insertInLeafNode(&rootLeaf, key) == OVERFLOW) {
      puts("\n\toverflow!!!");
      break;
    }
    else
      puts("\n\tnot overflow!!!");
    printLeafNode(rootLeaf);    
  }

}

void printLeafNode(LeafNode* node) {
  printf("\tRRN: %d\n", node->rrn);
  printf("\t\tkeys: ");
  for (int i = 0; i < node->num_keys; i++)
    printf("%s ", node->keys[i]);
  printf("\n\t\tnext_rrn: %d\n", node->next_rrn);
}

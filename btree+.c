#include "btree+.h"
#include <string.h>

int global_internal_node_rrn = 0;
int global_leaf_node_rrn = 0;

const int OVERFLOW = 1;
const int NOT_OVERFLOW = 0;

LeafNode* createLeafNode() {
  LeafNode* newLeafNode = (LeafNode*) malloc(sizeof(LeafNode));

  newLeafNode->rrn = global_leaf_node_rrn++;
  newLeafNode->next_rrn = -1;
  newLeafNode->num_keys = 0;
  for (int i = 0; i < N-1; i++)
    memset(newLeafNode->keys[i], '\0', LICENSE_PLATE_SIZE);

  return newLeafNode; 
}

InternalNode* createInternalNode() {
  InternalNode* newInternalNode = (InternalNode*) malloc(sizeof(InternalNode));

  newInternalNode->rrn = global_internal_node_rrn++;
  newInternalNode->is_pseudo_leaf = false;
  newInternalNode->num_keys = 0;
  for (int i = 0; i < M-1; i++) {
    memset(newInternalNode->keys[i], '\0', LICENSE_PLATE_SIZE);
    newInternalNode->children[i] = -1;
  }
  newInternalNode->children[M-1] =  -1;
  
  return newInternalNode;
}

int insertInLeafNode(LeafNode** node, const char* key) {
  if ((*node) == NULL)
    (*node) = createLeafNode();
  
  if ((*node)->num_keys == N - 1) 
    return OVERFLOW;

  int i = 0;
  while (strcmp(key, (*node)->keys[i]) > 0 && (*node)->keys[i][0] != '\0') 
    i++;
  
  if (i < (*node)->num_keys) { //needs to adjust the keys
    for (int j = (*node)->num_keys; j > i; j--) 
      strcpy((*node)->keys[j], (*node)->keys[j-1]);
  }
  strcpy((*node)->keys[i], key);
  (*node)->num_keys++;
  printf("[DEBUG] inserted: %s on leaf node %d", (*node)->keys[i], (*node)->rrn);

  writeLeafNodeToFile((*node));

  return NOT_OVERFLOW;
}

void temporaryBuildBTreePlus () {
  char key[LICENSE_PLATE_SIZE];
  int action = 0;
  bool root_is_leaf = true;
  LeafNode* rootLeaf = NULL;
  InternalNode* rootInternal = NULL;
  
  printf("acao: ");
  scanf(" %d", &action);
  while (action == 1) {
    printf("digite uma chave: ");
    scanf(" %s", key);

    if (insertInLeafNode(&rootLeaf, key) == OVERFLOW) {
      puts("\n\toverflow!!!");
      root_is_leaf = false;
      // create internal node
      InternalNode *newInternalNode = NULL;
      LeafNode *newLeafNode = NULL;
      internalNodeSplit(&rootLeaf, &newInternalNode, &newLeafNode, key);
    }
    
    printLeafNode(rootLeaf); 

    printf("acao: ");
    scanf(" %d", &action);
  }

}

void internalNodeSplit (LeafNode** leftChild, InternalNode** parent, LeafNode** rightChild, char* key) {
  char all_keys[N][LICENSE_PLATE_SIZE];
  sortKeys(all_keys, (*leftChild)->keys, key);
  int median = (N/2)-1, index_r = 0;

  if (insertInInternalNode(parent, all_keys[median]) == OVERFLOW) {
    puts("overflow em NÓ INTERNO");
  }

  for (int i = 0; i < median; i++) {
    strcpy((*leftChild)->keys[i], all_keys[i]);
  }

  if ((*rightChild) == NULL) {
    (*rightChild) = createLeafNode();
  }

  for (int i = median+1; i < N; i++) {
    strcpy((*rightChild)->keys[index_r++], all_keys[i]);
    memset((*leftChild)->keys[i-1], '\0', LICENSE_PLATE_SIZE);
    (*leftChild)->num_keys--;
    (*rightChild)->num_keys++;
  }

  (*leftChild)->next_rrn = (*rightChild)->rrn;
  (*parent)->is_pseudo_leaf = true;
  (*parent)->children[0] = (*leftChild)->rrn;
  (*parent)->children[1] = (*rightChild)->rrn;
  puts("FILHO ESQ");
  printLeafNode(*leftChild);

  puts("PAI");
  printInternalNode(*parent);

  puts("FILHO DIR");
  printLeafNode(*rightChild);


}

int insertInInternalNode (InternalNode** node, char *key) {
  if ((*node) == NULL) {
    *node = createInternalNode();
  }
  
  if ((*node)->num_keys == M-1) {
    return OVERFLOW;
  } 

  int i = 0;
  while (strcmp(key, (*node)->keys[i]) > 0 && (*node)->keys[i][0] != '\0') 
    i++;
  
  if (i < (*node)->num_keys) { //needs to adjust the keys
    for (int j = (*node)->num_keys; j > i; j--) 
      strcpy((*node)->keys[j], (*node)->keys[j-1]);
  }
  strcpy((*node)->keys[i], key);
  (*node)->num_keys++;
  
  printf("[DEBUG] inserted: %s on internal node %d", (*node)->keys[i], (*node)->rrn);
  
  writeInternalNodeToFile((*node));

  return NOT_OVERFLOW;

}

int partition(char arr[N][LICENSE_PLATE_SIZE], int low, int high) {
  char pivot[LICENSE_PLATE_SIZE];
  strncpy(pivot, arr[high], LICENSE_PLATE_SIZE);
  pivot[LICENSE_PLATE_SIZE - 1] = '\0';

  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (strcmp(arr[j], pivot) < 0) {
      i++;
      char temp[LICENSE_PLATE_SIZE];
      strncpy(temp, arr[i], LICENSE_PLATE_SIZE);
      strncpy(arr[i], arr[j], LICENSE_PLATE_SIZE);
      strncpy(arr[j], temp, LICENSE_PLATE_SIZE);
    }
  }

  char temp[LICENSE_PLATE_SIZE];
  strncpy(temp, arr[i + 1], LICENSE_PLATE_SIZE);
  strncpy(arr[i + 1], arr[high], LICENSE_PLATE_SIZE);
  strncpy(arr[high], temp, LICENSE_PLATE_SIZE);

  return i + 1;
}

void quickSort(char arr[N][LICENSE_PLATE_SIZE], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

void sortKeys(char result[N][LICENSE_PLATE_SIZE], char keys[N-1][LICENSE_PLATE_SIZE], char key[LICENSE_PLATE_SIZE]) {
  for (int i = 0; i < N - 1; i++) {
    strncpy(result[i], keys[i], LICENSE_PLATE_SIZE);
    result[i][LICENSE_PLATE_SIZE - 1] = '\0';
  }

  strncpy(result[N - 1], key, LICENSE_PLATE_SIZE);
  result[N - 1][LICENSE_PLATE_SIZE - 1] = '\0';

  quickSort(result, 0, N - 1);
}

void printLeafNode (LeafNode* node) {
  printf("\tRRN: %d\n", node->rrn);
  printf("\t\tkeys: ");
  for (int i = 0; i < node->num_keys; i++)
    printf("%s ", node->keys[i]);
  printf("\n\t\tnext_rrn: %d\n", node->next_rrn);
}

void printInternalNode (InternalNode* node) {
  printf("\tRRN: %d\n", node->rrn);
  printf("\t\tkeys: ");
  for (int i = 0; i < node->num_keys; i++)
    printf("%s ", node->keys[i]);
  for (int i = 0; i <= node->num_keys; i++)
    printf("%d ", node->children[i]);
}

void writeLeafNodeToFile (LeafNode* node) {
  FILE *f;
  size_t aux;

  if ((f = fopen("data/leaf.idx", "wb")) == NULL) {
    printf("[ERROR] Failed to open 'data/leaf.idx' :<\n");
    free(f);
    return;
  }

  long offset = node->rrn * sizeof(LeafNode);
  fseek(f, 0, SEEK_END);
  long fileSize = ftell(f);

  // Pad with -1 if needed
  if (offset > fileSize) {
    fseek(f, fileSize, SEEK_SET);
    for (long i = fileSize; i < offset; i++) {
      fputc(-1, f);
    }
  }

  fseek(f, offset, SEEK_SET);
  aux = fwrite(node, sizeof(LeafNode), 1, f);

  fclose(f);
}

void writeInternalNodeToFile (InternalNode* node) {
  FILE *f;
  size_t aux;

  if ((f = fopen("data/internal.idx", "wb")) == NULL) {
    printf("[ERROR] Failed to open 'data/internal.idx' :<\n");
    free(f);
    return;
  }

  long offset = node->rrn * sizeof(InternalNode);
  fseek(f, 0, SEEK_END);
  long fileSize = ftell(f);

  // Pad with -1 if needed
  if (offset > fileSize) {
    fseek(f, fileSize, SEEK_SET);
    for (long i = fileSize; i < offset; i++) {
      fputc(-1, f);
    }
  }

  fseek(f, offset, SEEK_SET);
  aux = fwrite(node, sizeof(InternalNode), 1, f);

  fclose(f);
}
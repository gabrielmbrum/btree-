#include "btree+.h"
#include <string.h>

int global_node_rrn = 0;
int global_vehicle_rrn = 0;

Node* createNode(bool is_leaf) {
  // Allocate memory for the new node
  Node* newNode = (Node*) malloc(sizeof(Node));
  
  // Initialize the fields
  newNode->rrn = global_node_rrn++; // Increment the global RRN
  newNode->is_leaf = is_leaf;               // Set the leaf status
  newNode->num_keys = 0;                    // No keys initially

  // Set the max_keys based on whether the node is a leaf or not
  newNode->max_keys = is_leaf ? N : M;

  // Initialize the keys and children
  for (int i = 0; i < newNode->max_keys; i++) {
    memset(newNode->keys[i], '\0', LICENSE_PLATE_SIZE);
    newNode->keys_rrn[i] = -1;             // Set RRN of keys to -1
    newNode->children[i] = -1;             // Initialize child pointers to -1
  }
  // The last child slot for internal nodes
  newNode->children[newNode->max_keys] = -1;

  // Initialize parent and next node RRNs
  newNode->parent_rrn = -1;                // No parent initially
  newNode->next_node_rrn = -1;             // No sibling initially

  return newNode;
}

int insert(Node** node, const char* key, int key_rrn) {
    if ((*node) == NULL) {
        (*node) = createNode(true);
    }

    // Check for overflow condition
    if ((*node)->num_keys == (*node)->max_keys) {
        return OVERFLOW;
    }

    // Find the correct position to insert the new key
    int i = 0;
    while (i < (*node)->num_keys && strcmp(key, (*node)->keys[i]) > 0) {
        i++;
    }

    // Shift keys and key_rrn to make room for the new key
    if (i < (*node)->num_keys) {
        for (int j = (*node)->num_keys; j > i; j--) {
            strcpy((*node)->keys[j], (*node)->keys[j - 1]);
            (*node)->keys_rrn[j] = (*node)->keys_rrn[j - 1];
        }
    }

    // Insert the new key and its associated RRN
    strcpy((*node)->keys[i], key);
    (*node)->keys_rrn[i] = key_rrn;

    // Increment the number of keys in the node
    (*node)->num_keys++;

    // Debugging output
    printf("[DEBUG] Inserted: %s with KEY_RRN %d in leaf node %d\n", key, key_rrn, (*node)->rrn);
    printNode(*node);
    // Persist the updated node to disk (function assumed to be defined elsewhere)
    writeNodeToFile((*node));

    return NOT_OVERFLOW;
}

void temporaryBuildBTreePlus () {
  char key[LICENSE_PLATE_SIZE];
  int action = 0;
  Node* root = NULL;
  root = createNode(true);
  
  printf("acao: ");
  scanf(" %d", &action);
  while (action == 1) {
    printf("digite uma chave: ");
    scanf(" %s", key);

    if (root->is_leaf) {
      if (insert(&root, key, global_vehicle_rrn++) == OVERFLOW) {
        puts("\n\toverflow!!!");
        
        Node *parent = NULL;
        parent = (root->parent_rrn) == -1 ? createNode(false) : searchNode(parent->rrn); 
        Node *rightChild = NULL;
        split(&root, &parent, &rightChild, key);
        root = parent;
      }
    } else {
      Node *aux = root;
      bool encontrado;
      while (!aux->is_leaf) {
        int i;
        for (i = 0; i < aux->num_keys; i++) {
          if (strcmp(key, aux->keys[i]) < 0) break;
        }

        aux = searchNode(aux->children[i]);
      }
    }

    
    
    //printLeafNode(rootLeaf); 

    printf("acao: ");
    scanf(" %d", &action);
  }

}

Node* searchNode (int rrn) {
  return NULL;
}

void split (Node** leftChild, Node** parent, Node** rightChild, char* key) { 
  int max = (*leftChild)->max_keys + 1;
  char all_keys[max][LICENSE_PLATE_SIZE];
  int all_keys_rrn[max];
  sortKeys(max, all_keys, all_keys_rrn, (*leftChild)->keys, key, global_vehicle_rrn++, (*leftChild)->keys_rrn);
  int median = (N/2) + 1, index_r = 0;

  printf("[DEBUG] CHECKING KEYS IN ALL KEYS | %d\n", median);
  for (int i = 0; i < max; i++) {
    printf("%s ", all_keys[i]);
  }
  printf("\n\n");

  if (insert(parent, all_keys[median], all_keys_rrn[median]) == OVERFLOW) {
    puts("overflow no PAI");
  }

  for (int i = 0; i < median; i++) {
    strcpy((*leftChild)->keys[i], all_keys[i]);
    (*leftChild)->keys_rrn[i] = all_keys_rrn[i];
  }

  if ((*rightChild) == NULL) {
    (*rightChild) = createNode(true);
  }

  for (int i = median; i < max; i++) {
    strcpy((*rightChild)->keys[index_r], all_keys[i]);
    (*rightChild)->keys_rrn[index_r++] = all_keys_rrn[i];
    memset((*leftChild)->keys[i], '\0', LICENSE_PLATE_SIZE);
    (*leftChild)->num_keys--;
    (*rightChild)->num_keys++;
  }
  (*leftChild)->num_keys++;

  (*leftChild)->next_node_rrn = (*rightChild)->rrn;
  (*parent)->children[0] = (*leftChild)->rrn;
  (*parent)->children[1] = (*rightChild)->rrn;
  (*leftChild)->parent_rrn = (*rightChild)->parent_rrn = (*parent)->rrn;

  printf("\tPARENT\n");
  printNode(*parent);
  printf("\tLeft Child\n");
  printNode(*leftChild);
  printf("\tRight Child\n");
  printNode(*rightChild);

}

void sortKeys(int max, char result[max][LICENSE_PLATE_SIZE], int result_rrn[max], char keys[max-1][LICENSE_PLATE_SIZE], char key[LICENSE_PLATE_SIZE], int key_rrn, int keys_rrn[max - 1]) {
  // Find the correctwh position to insert the new key
  int i = 0;
  while (i < (max - 1) && strcmp(key, keys[i]) > 0) {
    strcpy(result[i], keys[i]);
    result_rrn[i] = keys_rrn[i];
    i++;
  }

  // Insert the new key and its associated RRN
  strcpy(result[i], key);
  result_rrn[i] = key_rrn;

  // Shift keys and key_rrn to make room for the new key
  if (i < max - 1) {
    for (int j = max - 1; j > i; j--) {
      strcpy(result[j+1], keys[j]);
      result_rrn[j+1] = keys_rrn[j];
    }
  }
}

void printNode(Node *node) {
  printf("\tRRN: %d\n", node->rrn);
  printf("\t\tparent: %d\n", node->parent_rrn);
  printf("\t\tnext node: %d\n", node->next_node_rrn);
  printf("\t\tnum_keys: %d | max_keys: %d\n", node->num_keys, node->max_keys);
  printf("\t\tkeys: ");
  for (int i = 0; i < node->num_keys; i++)
    printf("%s ", node->keys[i]);
  printf("\n\t\tchildren: ");
  for (int i = 0; i <= node->num_keys; i++)
    printf("%d ", node->children[i]);     
  printf("\n");
}

void writeNodeToFile (Node* node) {
  FILE *f;
  size_t aux;

  if ((f = fopen("data/btreeplus.idx", "wb")) == NULL) {
    printf("[ERROR] Failed to open 'data/btreeplus.idx' :<\n");
    free(f);
    return;
  }

  long offset = node->rrn * sizeof(Node);
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

void initializeQueue(Queue** queue) {
  (*queue)->first = (*queue)->last = NULL;
  (*queue)->size = 0;
}

void enqueue(Queue** queue, Node* node, bool modified) {
  if (contains(*queue, node->rrn)) {
    moveToLast(queue, node, modified);
    return;
  }
  
  if ((*queue)->size == P) {
    dequeue(queue);
  }

  // Create and insert new queue node
  QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
  newNode->node = node;
  newNode->modified = modified;
  newNode->next = NULL;

  if ((*queue)->last != NULL) {
    (*queue)->last->next = newNode;
  }

  (*queue)->last = newNode;
  if ((*queue)->first == NULL) {
    (*queue)->first = newNode;
  }
  (*queue)->size++;
}

void dequeue(Queue** queue) {
  if ((*queue)->first == NULL) return;

  QueueNode* temp = (*queue)->first;

  if (temp->modified) {
    writeNodeToFile(temp->node);
  }

  if (temp->next == NULL) {
    free(temp);
  } else {
    while (temp->next->next != NULL) 
      temp = temp->next;

    (*queue)->last = temp;
    free(temp->next);
  }
  (*queue)->size--;
}

void moveToLast(Queue** queue, Node* node, bool modified) {
  if ((*queue)->first == NULL || (*queue)->last == NULL)
    return;

  if ((*queue)->last->node == node || (*queue)->size < 1)
    return;

  QueueNode* current = (*queue)->first;
  QueueNode* prev = NULL;
  
  while (current != NULL && current->node != node) {
    prev = current;
    current = current->next;
  }

  if (current == NULL)
    return;

  if (current == (*queue)->first) {
    (*queue)->first = current->next;
  } else {
    prev->next = current->next;
  }
  current->next = NULL;
  (*queue)->last->next = current;
  (*queue)->last = current;
  (*queue)->last->modified = modified;
}

bool contains(Queue *queue, int rrn) {
  if (queue->size == 0) return false;

  QueueNode* node = queue->first;
  while (node != NULL) {
    if (node->node->rrn == rrn) {
      return true;
    }
    node = node->next;
  }
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
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


int partition(char arr[N][LICENSE_PLATE_SIZE], int low, int high, int *keys_rrn) {
  char pivot[LICENSE_PLATE_SIZE];
  int pivot2 = keys_rrn[high];
  strncpy(pivot, arr[high], LICENSE_PLATE_SIZE);
  pivot[LICENSE_PLATE_SIZE - 1] = '\0';

  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (strcmp(arr[j], pivot) < 0) {
      i++;
      char temp[LICENSE_PLATE_SIZE];
      int temp2 = keys_rrn[i];
      strncpy(temp, arr[i], LICENSE_PLATE_SIZE);
      keys_rrn[i] = keys_rrn[j];
      strncpy(arr[i], arr[j], LICENSE_PLATE_SIZE);
      keys_rrn[j] = temp2;
      strncpy(arr[j], temp, LICENSE_PLATE_SIZE);
    }
  }

  char temp[LICENSE_PLATE_SIZE];
  int temp2 = keys_rrn[i + 1];
  strncpy(temp, arr[i + 1], LICENSE_PLATE_SIZE);
  keys_rrn[i+1] = keys_rrn[high];
  strncpy(arr[i + 1], arr[high], LICENSE_PLATE_SIZE);
  keys_rrn[high] = temp2;
  strncpy(arr[high], temp, LICENSE_PLATE_SIZE);

  return i + 1;
}

void quickSort(char arr[N][LICENSE_PLATE_SIZE], int low, int high, int *keys_rrn) {
  if (low < high) {
    int pi = partition(arr, low, high, keys_rrn);
    quickSort(arr, low, pi - 1, keys_rrn);
    quickSort(arr, pi + 1, high, keys_rrn);
  }
}

void internalNodeSplit (LeafNode** leftChild, InternalNode** parent, LeafNode** rightChild, char* key) {
  char all_keys[N][LICENSE_PLATE_SIZE];
  //sortKeys(all_keys, (*leftChild)->keys, key);
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

// LeafNode* createLeafNode() {
//   LeafNode* newLeafNode = (LeafNode*) malloc(sizeof(LeafNode));

//   newLeafNode->rrn = global_leaf_node_rrn++;
//   newLeafNode->next_rrn = -1;
//   newLeafNode->num_keys = 0;
//   for (int i = 0; i < N-1; i++)
//     memset(newLeafNode->keys[i], '\0', LICENSE_PLATE_SIZE);

//   return newLeafNode; 
// }

// InternalNode* createInternalNode() {
//   InternalNode* newInternalNode = (InternalNode*) malloc(sizeof(InternalNode));

//   newInternalNode->rrn = global_internal_node_rrn++;
//   newInternalNode->is_pseudo_leaf = false;
//   newInternalNode->num_keys = 0;
//   for (int i = 0; i < M-1; i++) {
//     memset(newInternalNode->keys[i], '\0', LICENSE_PLATE_SIZE);
//     newInternalNode->children[i] = -1;
//   }
//   newInternalNode->children[M-1] =  -1;
  
//   return newInternalNode;
// }
*/
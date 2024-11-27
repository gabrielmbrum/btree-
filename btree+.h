#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define M 2
#define N 3
#define P 10

#define LICENSE_PLATE_SIZE 8
#define MODEL_SIZE 20
#define BRAND_SIZE 20
#define CATEGORY_SIZE 15
#define STATUS_SIZE 16

#define OVERFLOW 1
#define NOT_OVERFLOW 0
#define ERROR -1

typedef struct leafNode {
  char keys[N-1][LICENSE_PLATE_SIZE];
  int num_keys;
  int rrn;
  int next_rrn;
} LeafNode;

typedef struct internalNode {
  char keys[M-1][LICENSE_PLATE_SIZE];
  int children[M];
  bool is_pseudo_leaf;                      // true if node's children are leaf nodes
  int num_keys;
  int rrn;
} InternalNode;

typedef struct node {
  char keys[N][LICENSE_PLATE_SIZE];
  int keys_rrn[N];
  int children[N+1];
  int max_keys;
  int num_keys;
  int rrn;
  int parent_rrn;
  int next_node_rrn;
  bool is_leaf;
} Node;

typedef struct {
  char licensePlate[LICENSE_PLATE_SIZE];    
  char model[MODEL_SIZE];                   
  char brand[BRAND_SIZE];                   
  int year;                                 
  char category[CATEGORY_SIZE];             
  int km;                              
  char status[STATUS_SIZE];                 
} Vehicle;

Node* createNode(bool is_leaf);

void writeNodeToFile (Node* node);

void split (Node** leftChild, Node** parent, Node** rightChild, char* key);

Node* searchNode (int rrn);

void printNode(Node *node);

/*
  creates a empty leaf node and set it rrn by the global node rrn var
*/
LeafNode* createLeafNode();

/*
  creates a empty internal node and set it rrn by the global node rrn var
*/
InternalNode* createInternalNode();

/*
  insert in a leaf node and in case of overflow, returns OVERFLOW, other wise returns NOT_OVERFLOW
*/
int insertInLeafNode(LeafNode** node, const char* key);

int insertInInternalNode (InternalNode** node, char *key);

void temporaryBuildBTreePlus ();

void internalNodeSplit (LeafNode** leftChild, InternalNode** parent, LeafNode** rightChild, char* key);

int partition(char arr[N][LICENSE_PLATE_SIZE], int low, int high, int *arr2);

void quickSort(char arr[N][LICENSE_PLATE_SIZE], int low, int high, int *arr2);

void sortKeys(int max, char result[max][LICENSE_PLATE_SIZE], int result_rrn[max], char keys[max-1][LICENSE_PLATE_SIZE], char key[LICENSE_PLATE_SIZE], int key_rrn, int keys_rrn[max - 1]);

/*
  [DEBUG] print leaf node info
*/
void printLeafNode(LeafNode* node);

void printInternalNode (InternalNode* node);

void writeLeafNodeToFile (LeafNode* node);

void writeInternalNodeToFile (InternalNode* node);

// typedef struct {
//   char keys[M - 1][LICENSE_PLATE_SIZE];    // Vehicle license plates (keys)
//   int keys_rrn[M - 1];                         // RRNs of the vehicles in the data file
//   int children[M];                         // RRNs of the child nodes
//   int numKeys;                             // Number of keys in the page
//   bool isLeaf;                             // Indicates if the node is a leaf
//   int rrn;                                 // RRN of the node itself
// } BTreeNode;

typedef struct queueNode {
  Node *node;
  bool modified;
  struct queueNode *next;
} QueueNode;

typedef struct queue {
  QueueNode *first;
  QueueNode *last;
  int size;
} Queue;

void initializeQueue(Queue **queue);

void enqueue(Queue** queue, Node* node, bool modified);

void dequeue(Queue** queue);

void moveToLast(Queue** queue, Node* node, bool modified);

void writeAllModified(Queue* queue);

bool contains(Queue* queue, int rrn);

// typedef struct queueNode {
//   BTreeNode *node;
//   bool modified;
//   struct queueNode *next;
// } QueueNode;

// typedef struct queue {
//   QueueNode *first;
//   QueueNode *last;
//   int size;
// } Queue;
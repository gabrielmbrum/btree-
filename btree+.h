#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define M 4
#define N 5
#define P 10

#define LICENSE_PLATE_SIZE 8
#define MODEL_SIZE 20
#define BRAND_SIZE 20
#define CATEGORY_SIZE 15
#define STATUS_SIZE 16

typedef struct leafNode {
  char keys[N-1][LICENSE_PLATE_SIZE];
  int num_keys;
  int rrn;
  int next_rrn;
} LeafNode;

typedef struct internalNode {
  char keys[M-1][LICENSE_PLATE_SIZE];
  int children[M];
  bool is_pseudo_leaf;
  int num_keys;
  int leafs[M];
  int rrn;
} InternalNode;

typedef struct {
  char licensePlate[LICENSE_PLATE_SIZE];    
  char model[MODEL_SIZE];                   
  char brand[BRAND_SIZE];                   
  int year;                                 
  char category[CATEGORY_SIZE];             
  int km;                              
  char status[STATUS_SIZE];                 
} Vehicle;

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

void temporaryBuildBTreePlus ();

/*
  [DEBUG] print leaf node info
*/
void printLeafNode(LeafNode* node);

// typedef struct {
//   char keys[M - 1][LICENSE_PLATE_SIZE];    // Vehicle license plates (keys)
//   int keys_rrn[M - 1];                         // RRNs of the vehicles in the data file
//   int children[M];                         // RRNs of the child nodes
//   int numKeys;                             // Number of keys in the page
//   bool isLeaf;                             // Indicates if the node is a leaf
//   int rrn;                                 // RRN of the node itself
// } BTreeNode;

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
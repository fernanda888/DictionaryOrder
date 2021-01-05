//-----------------------------------------------------------------------------
// Maria Fernanda Becerra
// mfbecerr
// CSE101
// pa6
// Dictionary.c
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Dictionary.h"

#define KEY_TYPE char*
#define VAL_TYPE int*
#define KEY_UNDEF NULL
#define VAL_UNDEF NULL
#define KEY_FORMAT "%s"
#define VAL_FORMAT "%p"
#define KEY_CMP(x,y) strcmp(x,y)

// Private Types and Functions ------------------------------------------------
// NodeObj
typedef struct NodeObj{
   KEY_TYPE key;
   VAL_TYPE value;
   struct NodeObj* left;
   struct NodeObj* right;
   struct NodeObj* parent;
   char color;
} NodeObj;
// Node
typedef NodeObj* Node;
// newNode()
// Constructor for Node type.
Node newNode(KEY_TYPE k, VAL_TYPE v) {
   Node N = malloc(sizeof(NodeObj));
   N->key = k;
   N->value = v;
   N->color='b';
   N->left = N->right = N->parent= NULL;
   return(N);
}
// freeNode()
// Destructor for Node type
void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}
// DictionaryObj
typedef struct DictionaryObj{
   Node root;
   Node cursor;
   Node nil;
   int numPairs;
   int duplicate;
} DictionaryObj;
// deleteAll()
// deletes all Nodes in the subtree rooted at R.
void deleteAll(Node R,Dictionary T){
   if( R!=T->nil ){
      deleteAll(R->left,T);
      deleteAll(R->right,T);
      freeNode(&R);
   }

}
Node findKey(Node R, KEY_TYPE k){
   if(R->key==NULL || KEY_CMP(k, R->key)==0) 
      return R;
   if( KEY_CMP(k, R->key)<0 ) 
      return findKey(R->left, k);
   else // strcmp(k, R->key)>0
      return findKey(R->right, k);
}
// findLeftmost()
// Returns the leftmost Node in the subtree rooted at R, or NULL if R is NULL.
Node findLeftmost(Node R){
   Node L = R;
   if( L!=NULL ){
     for( ; (L->left)->key!=NULL; L=L->left);
   }
   return L;
}
Node TreeMinimum(Node X){
  while((X->left)->key!=KEY_UNDEF){
    X=X->left;
  }
  return X;
}
Node TreeMaximum(Node X){
  while((X->right)->key!=KEY_UNDEF){
    X=X->right;
  }
  return X;
}
void printInOrder(FILE* out,Dictionary T,Node P){
  if(P!=T->nil){
    printInOrder(out,T,P->left);
    fprintf(out,KEY_FORMAT,P->key);
   // fprintf(out,"\n");
    printInOrder(out,T,P->right);
  }
}
void printPreOrder(FILE* out,Dictionary T,Node P){
  if(P!=T->nil){
    fprintf(out,KEY_FORMAT,P->key);
//    fprintf(out,"\n");
    printPreOrder(out,T,P->left);
    printPreOrder(out,T,P->right);
  }
}
void printPostOrder(FILE* out,Dictionary T,Node P){
  if(P!=T->nil){
    printPostOrder(out,T,P->left);
    printPostOrder(out,T,P->right);
    fprintf(out,KEY_FORMAT,P->key);
  //  fprintf(out,"\n");
  }
}
void LeftRotate(Dictionary T, Node X){
  Node Y= X->right;

  X->right=Y->left;
  if(Y->left != T->nil){
    (Y->left)->parent=X;
  }

  Y->parent=X->parent;
  if(X->parent==T->nil){
    T->root=Y;
  }
  else if(X==(X->parent)->left){
    (X->parent)->left=Y;
  }
  else{
    (X->parent)->right=Y;
  }

  Y->left=X;
  X->parent=Y;
}
void RightRotate(Dictionary T, Node X){
  Node Y=X->left;
  X->left=Y->right;
  if(Y->right!=T->nil){
    (Y->right)->parent=X;
  }

  Y->parent=X->parent;
  if(X->parent==T->nil){
    T->root=Y;
  }
  else if( X==(X->parent)->right){
    (X->parent)->right=Y;
  }
  else{
    (X->parent)->left=Y;
  }

  Y->right=X;
  X->parent=Y;
}
void InsertFixUp(Dictionary T, Node Z){
  Node Y;
  while((Z->parent)->color=='r'){
    if(Z->parent==((Z->parent)->parent)->left){
      Y=((Z->parent)->parent)->right;
      if(Y->color=='r'){
        (Z->parent)->color='b';
        Y->color='b';
        ((Z->parent)->parent)->color='r';
        Z=(Z->parent)->parent;
      }
      else{
        if(Z==(Z->parent)->right){
          Z=Z->parent;
          LeftRotate(T,Z);
        }
        (Z->parent)->color='b';
        ((Z->parent)->parent)->color='r';
        RightRotate(T,(Z->parent)->parent);
      }
    }
    else{
      Y=((Z->parent)->parent)->left;
      if(Y->color=='r'){
        (Z->parent)->color='b';
        Y->color='b';
        ((Z->parent)->parent)->color='r';
        Z=(Z->parent)->parent;
      }
      else{
        if(Z==(Z->parent)->left){
          Z=Z->parent;
          RightRotate(T,Z);
        }
        (Z->parent)->color='b';
        ((Z->parent)->parent)->color='r';
        LeftRotate(T,(Z->parent)->parent);
      }
    }
  }
  T->root->color='b';
}
void Transplant(Dictionary T, Node U, Node V){
  if(U->parent==T->nil){
    T->root=V;
  }
  else if(U==(U->parent)->left){
    (U->parent)->left=V;
  }
  else{
    (U->parent)->right=V;
  }
  V->parent=U->parent;
}
void DeleteFixUp(Dictionary T, Node X){
  Node W;
  while(X!=(T->root) && X->color=='b'){
    if(X==(X->parent)->left){
      W=X->parent->right;
      if(W->color=='r'){
        W->color='b';
        (X->parent)->color='r';
        LeftRotate(T,(X->parent));
        W=(X->parent)->right;
      }
      if((W->left)->color=='b' && (W->right)->color=='b'){
        W->color='r';
        X=X->parent;
      }
      else{
        if((W->right)->color=='b'){
          (W->left)->color='b';
          W->color='r';
          RightRotate(T,W);
          W=(X->parent)->right;
        }
        W->color=(X->parent)->color;
        (X->parent)->color='b';
        (W->right)->color='b';
        LeftRotate(T,(X->parent));
        X=T->root;
      }
    }
    else{
      W=(X->parent)->left;
      if(W->color=='r'){
        W->color='b';
        (X->parent)->color='r';
        RightRotate(T,(X->parent));
        W=(X->parent)->left;
      }
      if((W->right)->color=='b' && (W->left)->color=='b'){
        W->color='r';
        X=X->parent;
      }
      else{
        if((W->left)->color=='b'){
          (W->right)->color='b';
          W->color='r';
          LeftRotate(T,W);
          W=(X->parent)->left;
        }
        W->color=(X->parent)->color;
        (X->parent)->color='b';
        (W->left)->color='b';
        RightRotate(T,(X->parent));
        X=T->root;
      }
    }
  }
  X->color='b';
}

// Constructors-Destructors ---------------------------------------------------
// newDictionary()
// Constructor for the Dictionary type. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique){
   Dictionary D = malloc(sizeof(DictionaryObj));
   D->cursor=NULL;
   D->numPairs = 0;
   D->duplicate=unique;
   D->nil=newNode(KEY_UNDEF,VAL_UNDEF);
   (D->nil)->color='b';
   D->root = D->nil;
   return D;
}
// freeDictionary()
// Destructor for the Dictionary type.
void freeDictionary(Dictionary* pD){
   if( pD!=NULL && *pD!=NULL ){
      makeEmpty(*pD);
      freeNode(&(*pD)->nil);
      free(*pD);
      *pD = NULL;
   }
}
// Access functions -----------------------------------------------------------

// size()
// Returns the number of (key, value) pairs in Dictionary D.
int size(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling size() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   return D->numPairs;
}
// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling getUnique() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if(D->duplicate==0){
     return 0;
   }
   else{
     return 1;
   }
}
// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k 
// (i.e. if KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k){
  Node N;
   if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   N = findKey(D->root, k);
   if(N==NULL){
     return VAL_UNDEF;
   }
   else{
     return N->value;
   }
}

// Manipulation procedures ----------------------------------------------------

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary T, KEY_TYPE k, VAL_TYPE v){
  Node Z, Y, X;
  if( T==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling insert() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
  }
  if(getUnique(T)==1){
    if( findKey(T->root, k)!=T->nil ){
        fprintf(stderr, 
          "Dictionary Error: cannot insert() duplicate key\n");
        exit(EXIT_FAILURE);
    }
 }
  Z = newNode(k, v);
//printf("new Node\n");
  Y=T->nil;
  X=T->root;
  while(X!=T->nil){
    Y=X;
    if(KEY_CMP(Z->key,X->key)<0){
      X=X->left;
    }
    else{
      X=X->right;
    }
  }
//printf("after while\n");
  Z->parent=Y;
  if(Y==T->nil){
    T->root=Z;
  }
  else if(KEY_CMP(Z->key,Y->key)<0){
    Y->left=Z;
  }
  else{
    Y->right=Z;
  }
  Z->left=T->nil;
  Z->right=T->nil;
  Z->color='r';
//printf("before fix up\n");
  InsertFixUp(T,Z);
  (T->numPairs)++;
//printf("afterfixup\n");
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary T, KEY_TYPE k){
  Node Z,Y,X;
   if( T==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling delete() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   Z = findKey(T->root, k);
   if( Z==NULL ){
      fprintf(stderr, 
         "Dictionary Error: cannot delete() non-existent key\n");
      exit(EXIT_FAILURE);
   }
   if((T->cursor)==Z){
    T->cursor=NULL; 
   }
//printf("after cursor\n");
   Y=Z;
   char yOgCol=Y->color;
   if(Z->left==T->nil){
     X=Z->right;
//printf("before transp\n");
     Transplant(T,Z,(Z->right));
//     freeNode(&Z);
//printf("Transplant?\n");
   }
   else if(Z->right==T->nil){
     X=Z->left;
     Transplant(T,Z,(Z->left));
//     freeNode(&Z);
   }
   else{
//printf("after case 2\n");
     Y=TreeMinimum(Z->right);
     yOgCol=Y->color;
     X=Y->right;
     if(Y->parent==Z){
       X->parent=Y;
     }
     else{
      Transplant(T,Y,(Y->right));
      Y->right=Z->right;
      (Y->right)->parent=Y; 
     }
     Transplant(T,Z,Y);
     Y->left=Z->left;
     (Y->left)->parent=Y;
     Y->color=Z->color;
  //   freeNode(&Z);
   }
   if(yOgCol=='b'){
    DeleteFixUp(T,X); 
   }
//printf("it's delete fix up\n");
  // freeNode(&Z);
   (T->numPairs)--;
}
// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling makeEmpty() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }

   deleteAll(D->root,D);
   D->root = D->nil;
   D->cursor= NULL;
//   freeNode(&D->nil);
   D->numPairs = 0;
}
// beginForward()
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling beginForward() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if(D->numPairs==0){
    return VAL_UNDEF; 
   }
   Node R=D->root;
   while(R->left!=D->nil){
     R=R->left;
   }
   D->cursor=R;
   return R->value;
}
// beginReverse()
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling beginReverse() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
   }
   if(D->numPairs==0){
    return VAL_UNDEF; 
   }
   Node R=D->root;
   while(R->right!=D->nil){
     R=R->right;
   }
   D->cursor=R;
   return R->value;
}
// currentKey()
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling currentKey() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
  }
  if(D->cursor==NULL){
    return KEY_UNDEF;
  }
  else{
    return (D->cursor)->key;
  }
}
// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling currentVal() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
  }
  if(D->cursor==NULL){
    return VAL_UNDEF;
  }
  else{
    return (D->cursor)->value;
  }
}
// next()
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling next() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
  }
  if(D->cursor==NULL){
    return VAL_UNDEF;
  }
  Node Last=TreeMaximum(D->root);
  if((D->cursor)==Last){
    D->cursor=NULL;
    return VAL_UNDEF;
  }
  else{
    Node X=D->cursor;
    if(X->right!=D->nil){
      Node M=TreeMinimum(X->right);
      (D->cursor)=M;
      return (D->cursor)->value;
    }
    Node Y=X->parent;
    while(Y!=D->nil && X==(Y->right)){
      X=Y;
      Y=(Y->parent);
    }
    (D->cursor)=Y;
    return (D->cursor)->value;
  }
}
// prev()
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling prev() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
  }
  if(D->cursor==NULL){
    return VAL_UNDEF;
  }
  Node First=TreeMinimum(D->root);
  if((D->cursor)==First){
    D->cursor=NULL;
    return VAL_UNDEF;
  }
  else{
    Node X=D->cursor;
    if(X->left!=D->nil){
      (D->cursor)=TreeMaximum(X->left);
      return (D->cursor)->value;
    }
    Node Y=X->parent;
    while(Y!=D->nil && X==Y->left){
      X=Y;
      Y=Y->parent;
    }
    (D->cursor)=Y;
    return (D->cursor)->value;
  }
}
// Other operations -----------------------------------------------------------

// printDictionary()
// Prints the keys (only) of D in an order determined by the parameter ord.
// If ord is "pre", "in" or "post", executes the corresponding tree traversal
// on the underlying RBT, printing keys only (no values).  If ord is some
// other string, prints nothing.
void printDictionary(FILE* out, Dictionary D, const char* ord){
  if( D==NULL ){
      fprintf(stderr, 
         "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
      exit(EXIT_FAILURE);
  }
  if(strcmp(ord,"in")==0){
  Node X=D->root;
  printInOrder(out,D,X);
  }
  else if(strcmp(ord,"pre")==0){
  Node X=D->root;
  printPreOrder(out,D,X);
  }
  else if(strcmp(ord,"post")==0){
  Node X=D->root;
  printPostOrder(out,D,X);
  }
}

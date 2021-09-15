#include <stdio.h>
#include <stdlib.h>
#include "hcompress.h"
// malloc
typedef struct node {
  struct tnode* huffNode;
  struct node* next;
} LinkedList;

LinkedList* llCreate();
int llIsEmpty(LinkedList* ll);
void llDisplay(LinkedList* ll);
void llAdd(LinkedList** ll, struct tnode*  newHuffNode);
void llFree(LinkedList* ll);
void llAddInOrder(LinkedList** ll, struct tnode* newHuffNode);

LinkedList* llCreate() {
  return NULL;
}

int llIsEmpty(LinkedList* ll) {
  return (ll == NULL);
}

void llDisplay(LinkedList* ll) {
  LinkedList* p = ll;
  printf("[");
  while (p != NULL) {
    
    printf("%c: %d, ", (*p).huffNode->c, (*p).huffNode->weight);
    p = p->next;
  }

  printf("]\n");
}

void llAdd(LinkedList** ll, struct tnode* newHuffNode) {
  // Create the new node
  LinkedList* newNode = (LinkedList*)malloc(1 * sizeof(LinkedList));
  newNode->huffNode = newHuffNode;
  newNode->next = NULL;

  // Find the end of the list
  LinkedList* p = *ll;
  if (p == NULL) {// Add first element
    *ll = newNode;// This is why we need ll to be a **
  } else {
    while (p->next != NULL) {
      p = p->next;
    }    // Attach it to the end
    p->next = newNode;
  }
}

void llAddInOrder(LinkedList** ll, struct tnode* newHuffNode){
  LinkedList* newNode = (LinkedList*)malloc(sizeof(LinkedList));
  newNode->huffNode = newHuffNode;
  newNode->next = NULL;
  int stillLooking = 1;//1 true, 0 false
  
  LinkedList* p = *ll;
  if(p==NULL){
    *ll=newNode;
  }else if((p->huffNode->weight)>=(newNode->huffNode->weight)){//check if new node should be first element
    newNode->next=p;
    *ll=newNode;
  }else{
    while(stillLooking==1){
      if(p->next==NULL || (p->next->huffNode->weight)>=(newNode->huffNode->weight)){
	stillLooking=0;
      }else{
	p=p->next;
      }
    }
    if(p->next==NULL){
      p->next=newNode;
    }else{
      LinkedList* oldNext=p->next;
      p->next=newNode;
      newNode->next=oldNext;
    }
  }
}

void llFree(LinkedList* ll) {
  LinkedList* p = ll;
  while (p != NULL) {
    LinkedList* oldP = p;
    p = p->next;
    free(oldP);
  }
}



int main() {
  LinkedList* l = llCreate();
  llDisplay(l);
  struct tnode* huff1 = (struct tnode*)malloc(sizeof(struct tnode));
  (*huff1).c='a';
  (*huff1).weight=5;
  llAddInOrder(&l, huff1);
  llDisplay(l);
  struct tnode* huff2 = (struct tnode*)malloc(sizeof(struct tnode));
  (*huff2).c='b';
  (*huff2).weight=9;
  llAddInOrder(&l, huff2);
  llDisplay(l);
  struct tnode* huff3 = (struct tnode*)malloc(sizeof(struct tnode));
  (*huff3).c='c';
  (*huff3).weight=8;
  llAddInOrder(&l, huff3);
  llDisplay(l);
  struct tnode* huff4 = (struct tnode*)malloc(sizeof(struct tnode));
  (*huff4).c='d';
  (*huff4).weight=4;
  llAddInOrder(&l, huff4);
  llDisplay(l);
  llFree(l);
}

#include <stdio.h>
#include <stdlib.h> // malloc
#include "hcompress.h"

typedef struct node {
  struct tnode* huffNode;
  struct node* next;
} LinkedList;

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

void llAdd(LinkedList** ll, struct tnode*  newHuffNode) {

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
  LinkedList* newNode = (LinkedList*)malloc(1*sizeof(LinkedList));
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

#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"
#include <string.h>

struct tnode* createFreqTable(char* filename);

struct tnode* createHuffmanTree(struct tnode* leafNodes);

void encodeFile(char* argv, struct tnode* leafNodes);
int isNotRoot(struct tnode* node); 
int lengthOfString(char* string);
void checkMap(char* bits, struct tnode* root);

void decodeFile(char* fileName, struct tnode* root);
int isLeaf(struct tnode* node);
void decodeByte(unsigned char byte, char* bits);
void freeTree(struct tnode* root);

int main(int argc, char* argv[]) {

  // Check the make sure the input parameters are correct

   if (argc != 3) {
      printf("Error: The correct format is \"hcompress -e filename\" or \"hcompress -d filename.huf\"\n");
      fflush(stdout);

    exit(1);

   }
  // Create the frequency table by reading the generic file
  struct tnode* leafNodes = createFreqTable("decind.txt");

  // Create the huffman tree from the frequency table

  struct tnode* treeRoot = createHuffmanTree(leafNodes);
 
  // encode
  if (strcmp(argv[1], "-e") == 0) {

    // Pass the leafNodes since it will process bottom up
    encodeFile(argv[2], leafNodes);
  } else { // decode

    // Pass the tree root since it will process top down
     decodeFile(argv[2], treeRoot);
  }
  freeTree(treeRoot);
  free(leafNodes);
  
  return 0;

}

struct tnode* createFreqTable(char* filename){

  FILE* file=fopen(filename, "r");
  char c;
  struct tnode* ascii;

  //file not found error
  if(file==NULL){
    printf("File is not available\n");
    fclose(file);
    return NULL;
   //file found  
  }else{
    //creates an array of tnodes, one for each acii value
    //sets the weight(occurrences) of each value to 0
    ascii=(struct tnode*)malloc(128*sizeof(struct tnode));
    for(int i=0; i<128; i++){
      ascii[i].c=i;
      ascii[i].weight=1;
    }
    //goes through the file and increment the weight of the ascii value found
    while((c=fgetc(file))!=EOF){
      ascii[(int)c].weight++;
    }
    fclose(file);
    return ascii;
  }
}

//this is overcommented because it helps me find errors and I am having so much trouble finding this
 //%^?* error
struct tnode* createHuffmanTree(struct tnode* leafNodes){
  //creates and populates a linked lis with every ascii character node
  LinkedList* ll =llCreate();
  for(int i=0; i<128; i++){
    llAddInOrder(&ll, &leafNodes[i]);
  }
  struct tnode* branch;
  //as long as there are two nodes to be combined continue looking
  while(ll->next!=NULL){
    //creates a branch node who's weight is the children's weight combined
    branch = (struct tnode*)malloc(sizeof(struct tnode));
    branch->weight=((ll->huffNode->weight)+(ll->next->huffNode->weight));
    //add to the linked list and set the children of the branch node
    //adds new branch node to linked list
    //branch must come after children nodes because min weight of every node is 1
    llAddInOrder(&ll, branch);
    //set the left child to the first element of the linked list because it is the smaller
    branch->left=ll->huffNode;
    //set the first element of the linked list's parent to our branch node
    ll->huffNode->parent=branch;
    //move forward
    ll=ll->next;
    //set the right child to the second element in the linked list because it is the larger
    branch->right=ll->huffNode;
    //set the parent of the element to our branch node
    ll->huffNode->parent=branch;
    //progress to the next part of the list to continue the process
    ll=ll->next;
  }
  llFree(ll);
  //this is root node since it is the only one left at the end of the linked list
  return branch;
}


void encodeFile(char* fileName, struct tnode* leafNodes){
  FILE* file=fopen(fileName, "r");
  char c;
  unsigned char byte[8];
  int index=0;
  struct tnode* root=leafNodes;

  while(root->parent!=NULL){
    root=root->parent;
  }
  struct tnode* node;
  struct tnode* parent;
  //creates the new file's name
  int newStringLength=lengthOfString(fileName)+4;
  char newFileName[newStringLength];
  for(int i=0; i<=lengthOfString(fileName); i++){
    newFileName[i]=fileName[i];
  }

  //adds the extension
  char extension[] = ".huf";
  strncat(newFileName, extension, 4);
  
  //file not found error
  if(file==NULL){
    printf("File is not available\n");
    fclose(file);
  //file found  
  }else{
    unsigned char huffCode[128];
    int huffCodeIndex;
    unsigned char buffer;
    FILE* newFile = fopen(newFileName, "w");
    //while there are characters in the file continue looking
    while((c=fgetc(file))!=EOF){
      huffCodeIndex=0;
      //finds the node relating to the ascii character
      node=&leafNodes[(int)c];
      //go up the tree until the root is found
      //if when going up the child was the left node a 0 is input
      //if child was right node 1 is input
      while(isNotRoot(node)==1){
	parent=node->parent;
	if(node==parent->left){
	  huffCode[huffCodeIndex]=0;
	}else{
	  huffCode[huffCodeIndex]=1;
	}
	huffCodeIndex++;
	node=parent;
      }
      //once the huffCode of the letter has been found add it to the byte buffer to be flushed
        //added in reverse order so that when read in it reads top down instead of bottom up
      for(int i=huffCodeIndex-1; i>=0; i--){
	  byte[index]=huffCode[i];
	  index++;
	  if(index==8){
	    buffer=0;
	    //creates a byte of data that matches the 1's and 0's read in
	    for(int i=0; i<8; i++){
	      buffer<<=1;
	      if(byte[i])buffer|=1;
	    }
	    //*flushing noie*
	    fprintf(newFile, "%c", buffer);
   	   
	    index=0;
	  }
      }
    }
    //flushes any remaining bits to the file
    for(int i=0; i<8; i++){
      buffer<<=1;
      if(byte[i])buffer|=1;
    }
    fprintf(newFile, "%c", buffer);
    fclose(newFile);
    fclose(file);
  }

  printf("File encoded as: %s\n", newFileName);

}


//checks if the node in question is the root of the tree
//returns 1 if it is the root
//returns 0 if not the root
int isNotRoot(struct tnode* node){
  if(node->parent==NULL){
    return 0;
  }else{
    return 1;
  }
}

//finds the length of the entered string
int lengthOfString(char* string){
  int length=0;

  while(string[length]!='\0'){
    length++;
  }
  return length;
}


void decodeFile(char* fileName, struct tnode* root){
  FILE* file = fopen(fileName, "r");

  if(file==NULL){
    printf("File is not available\n");
  }else{

  //creates the new file's name
  int newStringLength=lengthOfString(fileName)+4;
  char newFileName[newStringLength];
  for(int i=0; i<=lengthOfString(fileName); i++){
    newFileName[i]=fileName[i];
  }

  //adds the extension
  char extension[] = ".dec";
  strncat(newFileName, extension, 4);

  struct tnode* currNode=root;
  char byte;
  int c;
  char*  bits = (char*)malloc(8*sizeof(char));
  FILE* newFile = fopen(newFileName, "w");
    //while there are characters in the file continue looking
    while((byte=fgetc(file))!=EOF){
      //get segmentation fault if this is not there ?
      printf("");
      //decode the code read in and then follow the path down 0=left 1=right
      decodeByte(byte, bits);
      for(int i=0; i<8; i++){
	if(bits[i]==1){
	  currNode=currNode->right;
	}else{
	  currNode=currNode->left;
	}
	//if the node is a leaf node write the char in the node to the new file
	if(isLeaf(currNode)==1){
	  c=currNode->c;
	  currNode=root;
	  fprintf(newFile, "%c", c);
	}
      }
    }
  fclose(newFile);
  free(bits);
  printf("File decoded to: %s\n", newFileName);
  }
  fclose(file);
}

//frees the mem allocated for the tree
void freeTree(struct tnode* root){
  if(root->left!=NULL){
    freeTree(root->left);
  }
  if(root->right!=NULL){
    freeTree(root->right);
  }
  //makes sure this not a leaf before freeing
  if(isLeaf(root)==0){
    free(root); 
  }
}

void  decodeByte(unsigned char byte, char* bits){

  unsigned char buffer=byte;
  unsigned char mask=1;
  //creates a byte of data that matches the 1's and 0's read in
  for(int i=7; i>=0; i--){
    if((buffer&mask)==1){
      bits[i]=1;
    }else bits[i]=0;
    buffer=buffer>>1;
  }
}

//if children not leaf >:(
int isLeaf(struct tnode* node){
  if(node->left==NULL && node->right==NULL)return 1;
  return 0;
}


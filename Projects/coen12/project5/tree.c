#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct tree{
	int data;
	struct tree *parent;
	struct tree *left;
	struct tree *right;
};


struct tree *createTree(int data, struct tree *left, struct tree *right){
	
	struct tree *root = malloc(sizeof(struct tree));
	assert(root!=NULL);
	root->data = data;
	root->right = right;
	root->left = left;
	root->parent = NULL;
	return root;
}

void destroyTree(struct tree *root){
	
	if(root->left!=NULL) destroyTree(root->left);
	if(root->right!=NULL) destroyTree(root->right);
	free(root);
	return;
}

int getData(struct tree *root){
	return root->data;
}

struct tree *getLeft(struct tree *root){
	return root->left;
}

struct tree *getRight(struct tree *root){
	return root->right;
}

struct tree *getParent(struct tree *root){
	return root->parent;
}

void setRight(struct tree *root, struct tree *right){
	root->right = right;
	return;
}

void setLeft(struct tree *root, struct tree *left){
	root->left = left;
	return;
}

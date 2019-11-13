/*
 *	tree.cpp
 */

#include<iostream>
#include "tree.h"
using namespace std;

struct node* create_node(int data)
{
	struct node* new_node = new node;
	new_node -> data = data;
	new_node -> left = NULL;
	new_node -> right = NULL;
	return new_node;
}

struct node* add_node(int data, struct node* root)
{
	if(root == NULL)
	{
		root = create_node(data);
		return root;
	}
	else
	{
		if (data < root -> data)
		{
			root -> left = add_node(data, root -> left);
		}
		else
		{
			root -> right = add_node(data, root -> right);
		}
	}
}

void print_tree(struct node* root, int space)
{
	struct node* curr = root;
	if(curr == NULL)
	{
		return;
	}
	space += COUNT;
	print_tree(curr -> right, space);
	printf("\n");
	for (int i = COUNT; i < space; i++) 
	{
        printf(" "); 
	}
	cout << curr -> data << endl;
	print_tree(curr -> left, space);
	
}
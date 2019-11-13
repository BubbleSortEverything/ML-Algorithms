/*
 *	tree.h
 */
#define COUNT 10
struct node
{
	int data;
	struct node* left;
	struct node* right;
};
struct node* create_node(int data);
void print_tree(struct node* root, int space);
struct node* add_node(int data, struct node* root);
/*
 *	Main.cpp
 */

#include <iostream>
#include <cstring>
#include "tree.h"

using namespace std;

int main()
{
	struct node* root;
	root = create_node(3);
	for(int	i = 0; i < 5; i++)
	{
		add_node(i, root);
	}
	print_tree(root, 10);
	


	return 0;
}
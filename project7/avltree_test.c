/**
 * C 语言: AVL树
 *
 * @author skywang
 * @date 2013/11/07
 */
#include <stdio.h>
#include "avltree.h"

//static int arr[]= {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};
static int arr[]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
#define TBL_SIZE(a) ( (sizeof(a)) / (sizeof(a[0])) )

void main()
{
	int i,ilen;
	AVLTree root=NULL;

	printf("== add: ");
	ilen = TBL_SIZE(arr);
	for(i=0; i<ilen; i++)
	{
		printf("%d ", arr[i]);
		root = avltree_insert(root, arr[i]);

		printf("\n== insert:%d \n", arr[i]);
		print_avltree(root, root->key, 0);
		printf("\n");
	}

	printf("\n== pre order: ");
	preorder_avltree(root);

	printf("\n== in order: ");
	inorder_avltree(root);

	printf("\n== pro order: ");
	proorder_avltree(root);
	printf("\n");

	printf("== height: %d\n", avltree_height(root));
	printf("== min: %d\n", avltree_minimum(root)->key);
	printf("== max: %d\n", avltree_maximum(root)->key);
	printf("== tree information: \n");
	print_avltree(root, root->key, 0);


	i = 8;
	printf("\n== delete node: %d", i);
	root = avltree_delete(root, i);

	printf("\n== height: %d", avltree_height(root));
	printf("\n== in order: ");
	inorder_avltree(root);
	printf("\n== tree information: \n");
	print_avltree(root, root->key, 0);

	// 销毁二叉树
	destroy_avltree(root);
}

/*
 * single_linked_list.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Ahmed Montasser
 */


/*----Preprocessor Section--------------------------------------------------------------------*/
#include "single_linked_list.h"
/*--------------------------------------------------------------------------------------------*/


/*----Functions Definition--------------------------------------------------------------------*/
void SLL_insertNodeAtBeginning(sll_type data)
{
	/* Allocate a New Node */
	struct sll_node *new_node = NEW_NODE_ALLOC(struct sll_node);

	/* (Runtime Error)>> BLOCK CODE: if new_node wasn't reserved */
	if( new_node == NULL_PTR )
	{
		printf("\n\nRuntime Error! No Heap Memory Available.\n\n");while(1);
	}

	/* Allocate the new node data */
	new_node->data =  data;

	/* Set the new_node next pointer to current head address */
	new_node->next_node = sll_head;

	/* Set the head pointer with new node address */
	sll_head = new_node;
}

/*----------------------------------------------------------------*/
void SLL_insertNodeAtEnd(sll_type data)
{
	struct sll_node *nodes_slider;

	/* Allocate a New Node */
	struct sll_node *new_node = NEW_NODE_ALLOC(struct sll_node);

	/* (Runtime Error)>> BLOCK CODE: if new_node wasn't reserved */
	if( new_node == NULL_PTR )
	{
		printf("\n\nRuntime Error! No Heap Memory Available.\n\n");while(1);
	}

	/* Allocate the new node data */
	new_node->data =  data;

	/* Set new_node next_node pointer with NULL */
	new_node->next_node = NULL_PTR;

	if( sll_head == NULL_PTR )
	{
		/* Set the head pointer with new node address */
		sll_head = new_node;
	}
	else
	{
		/* Set the slider pointer with head pointer */
		nodes_slider = sll_head;

		/* Loop till reach last node in the list */
		while( nodes_slider->next_node != NULL_PTR )
		{
			nodes_slider = nodes_slider->next_node;
		}

		/* set the old last node next_node pointer with new_node ptr value */
		nodes_slider->next_node = new_node;
	}
}


int  SLL_insertNodeAtIndex(sll_type data, int index)
{
	int i=0, lsize=0;
	struct sll_node *prev_node;

	/* Allocate a New Node */
	struct sll_node *new_node = NEW_NODE_ALLOC(struct sll_node);

	/* (Runtime Error)>> BLOCK CODE: if new_node wasn't reserved */
	if( new_node == NULL_PTR )
	{
		printf("\n\nRuntime Error! No Heap Memory Available.\n\n");while(1);
	}

	/* if the index is invalid the node will be added by default at the end of list and index will return */
	lsize = SLL_sizeofLinkedList();
	if( lsize < index )
	{

		printf("\nWarning, Index invalid! Node will be added at list End..\n");

		SLL_insertNodeAtEnd(data);

		return (lsize+1);
	}


	/* Allocate the new node data */
	new_node->data =  data;

	prev_node = sll_head;
	for(i=0 ; i<index-1 ; i++)
	{
		prev_node = prev_node->next_node;
	}

	/* Set the New Node next pointer with the prev node next pointer */
	new_node->next_node = prev_node->next_node;

	/* Set the prev node next pointer with new_node */
	prev_node->next_node = new_node;

	return index;
}

/*----------------------------------------------------------------*/
int  SLL_findNodeIndex(sll_type data)
{
	int index=0;
	struct sll_node* nodes_slider = sll_head;

	/* Loop over the list until find the node that have the desired data */
	while( nodes_slider != NULL_PTR )
	{
		if( nodes_slider->data == data)
		{
			return index;
		}

		index++;
		nodes_slider = nodes_slider->next_node;
	}

	return NOT_FOUND;
}

/*----------------------------------------------------------------*/
int  SLL_updateNodePassValue(sll_type old_data, sll_type new_data)
{
	int index=0;
	struct sll_node* nodes_slider = sll_head;

	/* Loop over the list until find the node that have the desired data to be updated */
	while( nodes_slider != NULL_PTR )
	{
		if( nodes_slider->data == old_data)
		{
			nodes_slider->data = new_data;
			return index;
		}

		index++;
		nodes_slider = nodes_slider->next_node;
	}

	return NOT_FOUND;
}

/*----------------------------------------------------------------*/
int  SLL_deleteNodeByValue(sll_type data)
{
	struct sll_node* nodes_slider = sll_head;
	struct sll_node* toBeDeleted;

	while( nodes_slider != NULL_PTR )
	{
		if( nodes_slider->next_node->data == data)
		{
			toBeDeleted = nodes_slider->next_node;
			nodes_slider->next_node = nodes_slider->next_node->next_node;

			free(toBeDeleted);

			return DONE;
		}
		nodes_slider = nodes_slider->next_node;
	}

	return NOT_FOUND;
}

/*----------------------------------------------------------------*/
void SLL_deleteNodeByIndex(int index)
{
	int i;
	struct sll_node* prev_node = sll_head;
	struct sll_node* toBeDeleted;

	for(i=0 ; i<index-1 ; i++)
	{
		prev_node = prev_node->next_node;
	}

	toBeDeleted = prev_node->next_node;

	prev_node->next_node = prev_node->next_node->next_node;

	free(toBeDeleted);
}

/*----------------------------------------------------------------*/
int SLL_sizeofLinkedList(void)
{
	int size;
	struct sll_node* nodes_slider;

	/* Set the node_slider pointer with 1st element at head */
	nodes_slider = sll_head;

	/* Loop over all the nodes and increase the size with 1 until reach NULL, break loop */
	while( nodes_slider != NULL )
	{
		size++;
		nodes_slider = nodes_slider->next_node;
	}

	return size;
}

/*----------------------------------------------------------------*/
void SLL_printLinkedList(void)
{
	struct sll_node* nodes_slider;

	/* Set the node_slider pointer with 1st element at head */
	nodes_slider = sll_head;

	/* Loop over all the nodes and print the node data until reach NULL, break loop */
	printf("Linked List =");
	while( nodes_slider != NULL )
	{
		printf(" %d", nodes_slider->data);
		nodes_slider = nodes_slider->next_node;
	}
	printf("\n");
}
/*--------------------------------------------------------------------------------------------*/

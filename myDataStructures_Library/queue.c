/*
 * queue.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Ahmed Montasser
 */

/*----Preprocessor Section--------------------------------------------------------------------*/
#include "queue.h"
/*--------------------------------------------------------------------------------------------*/


/*----Functions Definition--------------------------------------------------------------------*/
void QUEUE_enqueue(queue_type data)
{
	/* Init the slider pointer with Queue head */
	struct queue_node *nodes_slider= queue_head;;

	/* Allocate new node */
	struct queue_node *new_node = NEW_NODE_ALLOC(struct queue_node);

	/* (Runtime Error)>> BLOCK CODE: if new_node wasn't reserved */
	if( new_node == NULL_PTR )
	{
		printf("\n\nRuntime Error! No Heap Memory Available.\n\n");while(1);
	}

	/* Set the new node data */
	new_node->data = data;

	/* Set the new node next pointer to NULL */
	new_node->next_node = NULL_PTR;

	if( queue_head == NULL_PTR )
	{
		/* Set the head with new_node address */
		queue_head = new_node;
	}
	else
	{
		/* Slide till reach last node, set its next pointer with new node */
		while( nodes_slider->next_node != NULL_PTR )
		{
			nodes_slider = nodes_slider->next_node;
		}

		nodes_slider->next_node = new_node;
	}
}

/*----------------------------------------------------------------*/
void QUEUE_dequeue(void)
{
	/* Save the head value in another variable */
	struct queue_node *toBeDeleted = queue_head;

	/* if stack empty do nothing */
	if( queue_head == NULL_PTR )
	{
		return;
	}

	/* Adjust stack head to next node */
	queue_head = queue_head->next_node;

	/* Delete the 1st node */
	free(toBeDeleted);
}

/*----------------------------------------------------------------*/
queue_type QUEUE_readTopValue(void)
{
	return queue_head->data;
}

/*----------------------------------------------------------------*/
void QUEUE_print(void)
{
	struct queue_node *nodes_slider = queue_head;

	if( queue_head == NULL_PTR)
	{
		return;
	}

	printf("Queue =");
	while( nodes_slider != NULL_PTR )
	{
		printf(" %d", nodes_slider->data);
		nodes_slider = nodes_slider->next_node;
	}
	printf("\n");
}
/*--------------------------------------------------------------------------------------------*/

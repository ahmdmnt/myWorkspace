/*
 * stack.c
 *
 *  Created on: Oct 13, 2020
 *      Author: Ahmed Montasser
 */

/*----Preprocessor Section--------------------------------------------------------------------*/
#include "stack.h"
/*--------------------------------------------------------------------------------------------*/


/*----Functions Definition--------------------------------------------------------------------*/
void STACK_PUSH(stack_type data)
{
	/* Allocate new node */
	struct stack_node *new_node = NEW_NODE_ALLOC(struct stack_node);

	/* (Runtime Error)>> BLOCK CODE: if new_node wasn't reserved */
	if( new_node == NULL_PTR )
	{
		printf("\n\nRuntime Error! No Heap Memory Available.\n\n");while(1);
	}


	/* Save the in the new node */
	new_node->data = data;

	/* Set the new node next pointer to head */
	new_node->next_node = stack_head;

	/* Set the head pointer with new node */
	stack_head = new_node;
}

/*----------------------------------------------------------------*/
void STACK_POP(void)
{
	/* Save the head value in another variable */
	struct stack_node *toBeDeleted = stack_head;

	/* if stack empty do nothing */
	if( stack_head == NULL_PTR )
	{
		return;
	}

	/* Adjust stack head to next node */
	stack_head = stack_head->next_node;

	/* Delete the 1st node */
	free(toBeDeleted);
}

/*----------------------------------------------------------------*/
stack_type STACK_PEEK(void)
{
	return stack_head->data;
}

/*----------------------------------------------------------------*/
void STACK_print(void)
{
	struct stack_node *nodes_slider=stack_head;

	if( stack_head == NULL_PTR)
	{
		return;
	}

	printf("Stack =");
	while( nodes_slider != NULL_PTR )
	{
		printf(" %d", nodes_slider->data);
		nodes_slider = nodes_slider->next_node;
	}
	printf("\n");
}
/*--------------------------------------------------------------------------------------------*/

/*
 * main.c
 * 		Test the Designed Data Structures functions
 *
 *  Created on: Oct 12, 2020
 *      Author: Ahmed Montasser
 */

#include "single_linked_list.h"
#include "stack.h"
#include "queue.h"


struct sll_node *sll_head = NULL_PTR;
struct stack_node *stack_head = NULL_PTR;
struct queue_node *queue_head = NULL_PTR;

int main(void)
{
//	int index;
//	int list_size;
//
//	setbuf(stdout, NULL);
//
//	SLL_insertNodeAtEnd(5);
//	SLL_insertNodeAtEnd(6);
//	SLL_insertNodeAtEnd(7);
//	SLL_insertNodeAtEnd(8);
//
//	SLL_printLinkedList();
//
//	SLL_insertNodeAtBeginning(1);
//	SLL_insertNodeAtBeginning(2);
//	SLL_insertNodeAtBeginning(3);
//	SLL_insertNodeAtBeginning(4);
//
//	SLL_insertNodeAtIndex(10, 4);
//	SLL_insertNodeAtIndex(20, 5);
//
//	SLL_insertNodeAtIndex(9, 200);
//
//	SLL_printLinkedList();
//
//	list_size = SLL_sizeofLinkedList();
//	printf("Size= %d\n\n", list_size);
//
//	index = SLL_findNodeIndex(20);
//	printf("Value 20 is at index = %d\n", index);
//
//	SLL_updateNodePassValue(20, 10);
//	SLL_updateNodePassValue(9, 7);
//
//	SLL_printLinkedList();
//
//	SLL_deleteNodeByValue(8);
//	SLL_printLinkedList();
//
//	SLL_deleteNodeByIndex(4);
//	SLL_printLinkedList();

	/*****************************************************************/

	STACK_PUSH(10);
	STACK_PUSH(20);
	STACK_PUSH(30);
	STACK_PUSH(40);

	STACK_print();

	STACK_POP();

	printf("Top of stack value = %d \n", STACK_PEEK());

	STACK_print();

	/*****************************************************************/

	QUEUE_enqueue(100);
	QUEUE_enqueue(200);
	QUEUE_enqueue(300);
	QUEUE_enqueue(400);

	QUEUE_print();


	QUEUE_dequeue();

	printf("Top of Queue value = %d \n", QUEUE_readTopValue());

	QUEUE_print();

	return 0;
}

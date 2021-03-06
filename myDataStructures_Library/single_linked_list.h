/*
 * single_linked_list.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Ahmed Montasser
 */

#ifndef SINGLE_LINKED_LIST_H_
#define SINGLE_LINKED_LIST_H_

/*----Preprocessor Section--------------------------------------------------------------------*/
#ifndef _DSL_LIB_
#define _DSL_LIB_
	#include <stdio.h>
	#include <stdlib.h>
#endif /* _DSL_LIB_ */

#define NOT_FOUND 0xFFFFFFFF
#define DONE 0xFFFFFFFE
#define NULL_PTR ((void*)0)
#define NEW_NODE_ALLOC(TYPE) (TYPE*)malloc(sizeof(TYPE))
/*--------------------------------------------------------------------------------------------*/


/*----SLL Node Type Definition-----------------------------------------------------------------*/

/* Change the sll_type definition with desired, Ex: typedef <C_DATATYPE> sll_type; */
typedef int sll_type;

/* Node Type Declaration */
struct sll_node
{
	sll_type data;
	struct sll_node *next_node;
};
/*--------------------------------------------------------------------------------------------*/


/*----Extern-ed Global Variable----------------------------------------------------------------*/
extern struct sll_node *sll_head;
/*--------------------------------------------------------------------------------------------*/


/*----Functions Prototypes--------------------------------------------------------------------*/
void SLL_insertNodeAtBeginning(sll_type data);
void SLL_insertNodeAtEnd(sll_type data);
int  SLL_insertNodeAtIndex(sll_type data, int index);
int  SLL_findNodeIndex(sll_type data);
int  SLL_updateNodePassValue(sll_type old_data, sll_type new_data);
int  SLL_deleteNodeByValue(sll_type data);
void SLL_deleteNodeByIndex(int index);
int  SLL_sizeofLinkedList(void);
void SLL_printLinkedList(void);
/*--------------------------------------------------------------------------------------------*/


#endif /* SINGLE_LINKED_LIST_H_ */

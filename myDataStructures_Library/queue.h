/*
 * queue.h
 *
 *  Created on: Oct 13, 2020
 *      Author: Ahmed Montasser
 */

#ifndef QUEUE_H_
#define QUEUE_H_

/*----Preprocessor Section--------------------------------------------------------------------*/
#ifndef _DSL_LIB_
#define _DSL_LIB_
	#include <stdio.h>
	#include <stdlib.h>
#endif /* _DSL_LIB_ */

#define NULL_PTR ((void*)0)
#define NEW_NODE_ALLOC(TYPE) (TYPE*)malloc(sizeof(TYPE))
/*--------------------------------------------------------------------------------------------*/


/*----SLL Node Type Definition-----------------------------------------------------------------*/

/* Change the sll_type definition with desired, Ex: typedef <C_DATATYPE> sll_type; */
typedef int queue_type;

/* Node Type Declaration */
struct queue_node
{
	queue_type data;
	struct queue_node *next_node;
};
/*--------------------------------------------------------------------------------------------*/


/*----Extern-ed Global Variable----------------------------------------------------------------*/
extern struct queue_node *queue_head;
/*--------------------------------------------------------------------------------------------*/


/*----Functions Prototypes--------------------------------------------------------------------*/
void QUEUE_enqueue(queue_type data);
void QUEUE_dequeue(void);
queue_type QUEUE_readTopValue(void);
void QUEUE_print(void);
/*--------------------------------------------------------------------------------------------*/

#endif /* QUEUE_H_ */

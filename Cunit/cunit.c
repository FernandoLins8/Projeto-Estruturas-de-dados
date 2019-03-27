#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void pq_test();
void hash_test();

int main()
{
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	CU_pSuite pSuite = NULL;
	pSuite = CU_add_suite("huff_strcts", 0, 0);

	if(!pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if(!CU_add_test(pSuite, "pq_test", pq_test))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if(!CU_add_test(pSuite, "hash_test", hash_test))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

	CU_cleanup_registry();
	return 0;
}

void pq_test()
{
	Queue *queue = create_priority_queue();
	enqueue(queue, create_node(1, 5,NULL, NULL, NULL));
	enqueue(queue, create_node(2, -4,NULL, NULL, NULL));
	enqueue(queue, create_node(4, 2,NULL, NULL, NULL));
	enqueue(queue, create_node(5, 5,NULL, NULL, NULL));

	CU_ASSERT(dequeue(queue)->value == 2);
	CU_ASSERT(dequeue(queue)->value == 4);
	CU_ASSERT(dequeue(queue)->value == 5);
	CU_ASSERT(dequeue(queue)->value == 1);
	CU_ASSERT(queue->head == NULL);
}

void hash_test()
{
	Hash_table *ht = create_hash_table();
	put(ht, 'a', 20);
	put(ht, 'z', 6);
	put(ht, '0', 3);
	put(ht, '0', 40);

	CU_ASSERT(ht != NULL);
	CU_ASSERT(ht->table['a']->freq == 20);
	CU_ASSERT(ht->table['z']->freq == 6);
	CU_ASSERT(ht->table['0']->freq == 40);
	CU_ASSERT(ht->table['x'] == NULL);
}
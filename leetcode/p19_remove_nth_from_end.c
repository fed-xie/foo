/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
#include <stdlib.h>

struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
	if (NULL == head)
		return NULL;
	//assert(0 < n && n < list_length)
	struct ListNode* a = head;
	struct ListNode** p = &head;
	for (int i=0; i<n; ++i)
		a = a->next;
	while (a) {
		a = a->next;
		p = &((*p)->next);
	}
	a = *p;
	*p = (*p)->next;
	free(a);
	return head;
}
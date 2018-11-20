/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
#include <stdlib.h>

struct ListNode* reverseKGroup(struct ListNode* head, int k)
{
	if (k <= 1 || NULL == head)
		return head;
	struct ListNode *end, **p = &head;
	int count = 0;
	for (end = head; NULL != end; end = end->next) {
		if (k == ++count) {
			count = 0;
			
			struct ListNode *tail = *p;
			for (int l=1; l<k; ++l) {
				struct ListNode* node = tail->next;
				tail->next = node->next;
				node->next = *p;
				*p = node;
			}
			end = tail;
			p = &(tail->next);
		}
	}
	return head;
}

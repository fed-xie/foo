/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* swapPairs(struct ListNode* head)
{
	struct ListNode *beg, *end;
	if (NULL == head || NULL == head->next)
		return head;
	beg = head;
	end = beg->next;
	//swap
	beg->next = end->next;
	end->next = beg;
	
	head = end;
	
	struct ListNode** p = &(beg->next);
	beg = beg->next;
	while (beg && beg->next) {
		end = beg->next;
		
		beg->next = end->next;
		end->next = beg;
		
		*p = end;
		
		p = &(beg->next);
		beg = beg->next;
	}
	return head;
}
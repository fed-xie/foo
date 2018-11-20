/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2)
{
	if (NULL == l2)
		return l1;
	if (NULL == l1)
		return l2;
	struct ListNode* head;
	struct ListNode** p = &head;
	while (l1 && l2) {
		if (l1->val < l2->val) {
			*p = l1;
			l1 = l1->next;
		}else {
			*p = l2;
			l2 = l2->next;
		}
		p = &((*p)->next);
	}
	*p = l2 ? l2 : l1;
	return head;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize)
{
	if (NULL == lists || 0 == listsSize)
		return NULL;
	while (listsSize > 1) {
		int i = 0;
		int count = 0;
		for (; i+1<listsSize; i+=2)
			lists[count++] = mergeTwoLists(lists[i], lists[i+1]);
		if (i<listsSize)
			lists[count++] = lists[i];
		listsSize = count;
	}
	return lists[0];
}
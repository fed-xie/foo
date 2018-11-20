/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode {
	int val;
	struct ListNode *next;
};

#include <stdlib.h>

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2)
{
	struct ListNode dummy;
	struct ListNode *node = &dummy;
	
	int carry = 0;
	while (l1 && l2) {
		int val = l1->val + l2->val + carry;
		if (val >= 10) {
			carry = 1;
			val -= 10;
		}else {
			carry = 0;
		}
		node->next = malloc(sizeof(struct ListNode));
		node = node->next;
		node->val = val;
		l1 = l1->next;
		l2 = l2->next;
	}
	if (l2)
		l1 = l2;
	while (l1) {
		int val = l1->val + carry;
		if (val >= 10) {
			carry = 1;
			val -= 10;
		}else {
			carry = 0;
		}
		node->next = malloc(sizeof(struct ListNode));
		node = node->next;
		node->val = val;
		l1 = l1->next;
	}
	if (carry) {
		node->next = malloc(sizeof(struct ListNode));
		node = node->next;
		node->val = 1;
	}
	node->next = NULL;
	return dummy.next;
}

#include <stdio.h>
#include <string.h>	
static struct ListNode *node_build(const char *digits)
{
    struct ListNode *res, *p, *prev;
    int first = 1;
    int len = strlen(digits);
    const char *c = digits + len - 1;
    prev = NULL;
    while (len-- > 0) {
        p = malloc(sizeof(*p));
        if (first) {
            first = 0;
            res = p;
        }
        p->val = *c-- - '0';
        p->next = NULL;
        if (prev != NULL) {
            prev->next = p;
        }
        prev = p;
    }

    return res;
}

static void show(struct ListNode *ln)
{
    int sum = 0, factor = 1;
    while (ln != NULL) {
        sum += ln->val * factor;
        factor *= 10;
        ln = ln->next;
    }
    printf("%d\n", sum);
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: ./test n1 n2\n");
        exit(-1);
    }

    struct ListNode *l1 = node_build(argv[1]);
    struct ListNode *l2 = node_build(argv[2]);
    struct ListNode *res = addTwoNumbers(l1, l2);
    show(l1);
    show(l2);
    show(res);
    return 0;
}
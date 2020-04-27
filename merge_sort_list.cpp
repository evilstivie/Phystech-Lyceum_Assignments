#include <iostream>
using namespace std;

struct Node {
    int el;
    Node *next, *prev;
};

Node *lst;
int N, res_n = 0;

void print_node(Node *&head) {
    if (head == NULL) { return; }
    cout << head->el << " ";
    print_node(head->next);
}

void build_list(Node *&head, Node *prevn, int n) {
    if (n) {
        head = new Node;
        cin >> (head->el);
        head->next = NULL;

        head->prev = prevn;
        build_list(head->next, head, n - 1);
    }
}

Node* add_node(Node *head, int a, int n) {
    Node *tmp = new Node;
    tmp->el = a;
    tmp->prev = NULL;
    tmp->next = NULL;

    if (head == NULL) {
        head = tmp;
        return head;
    }

    --n;
    Node *cur = head;
    for (int i = 1; i < n && cur->next != NULL; ++i) {
        cur = cur->next;
    }

    if (!n) {
        tmp->next = head;
        head->prev = tmp;
        head = tmp;
        return head;
    }

    if (cur->next != NULL) {
        cur->next->prev = tmp;
    }
    tmp->next = cur->next;
    cur->next = tmp;
    tmp->prev = cur;
    cur = tmp;
    return head;
}

Node *get_node_by_num(Node *head, int n) {
    int i = 0;
    --n;
    while (head != NULL) {
        if (i == n) {
            break;
        }
        head = head->next;
        ++i;
    }
    return head;
}

void merge(int l, int m, int r) {
    Node *head1 = get_node_by_num(lst, l + 1);
    Node *head2 = get_node_by_num(lst, m + 1);

    res_n = 1;
    Node *res = NULL;
    int i = l, j = m;
    while (i < m && j < r) {
        if (head1->el < head2->el) {
            res = add_node(res, head1->el, res_n);
            head1 = head1->next;
            ++i;
        } else {
            res = add_node(res, head2->el, res_n);
            head2 = head2->next;
            ++j;
        }
        ++res_n;
    }

    while (i < m) {
        res = add_node(res, head1->el, res_n);
        head1 = head1->next;
        ++i; ++res_n;
    }
    while (j < r) {
        res = add_node(res, head2->el, res_n);
        head2 = head2->next;
        ++j; ++res_n;
    }

    Node *h = get_node_by_num(lst, l + 1);
    for (int k = 0; k < r - l; ++k) {
        if (!k) {
            h->el = res->el;
        } else {
            h->el = res->el;
        }
        h = h->next;
        res = res->next;
    }
}

void merge_sort(int l, int r) {
    if (l + 1 >= r) {
        return;
    }
    int m = l + (r - l) / 2;

    merge_sort(l, m);
    merge_sort(m, r);
    merge(l, m, r);
}

int main() {
    cin >> N;

    lst = new Node;
    build_list(lst, lst, N);

    merge_sort(0, N);

    print_node(lst);
}

#include <cassert>
#include <iostream>

#include "stack.h"

// Вспомогательная функция для подсчета узлов в стеке
static int count_nodes(const Stack *s) {
    int cnt = 0;
    Node *cur = s->top;
    while (cur) {
        cnt++;
        cur = cur->next;
    }
    return cnt;
}

// Тест createNode: проверка полей и возможности связать узлы
static void test_createNode() {
    Node *n = createNode(12345);
    assert(n != nullptr);
    assert(n->data == 12345);
    assert(n->next == nullptr);

    Node *m = createNode(67890);
    assert(m != nullptr);
    n->next = m;
    assert(n->next == m);
    assert(n->next->data == 67890);

    std::free(m);
    std::free(n);

    std::cout << "test_createNode passed\n";
}

// Тест initStack: инициализация и повторная инициализация
static void test_initStack() {
    Stack s;

    initStack(&s);
    assert(s.top == nullptr);
    assert(isEmpty(&s) == true);

    Node *one = static_cast<Node*>(std::malloc(sizeof(Node)));
    assert(one != nullptr);
    one->data = 1;
    one->next = nullptr;
    s.top = one;

    initStack(&s);
    assert(s.top == nullptr);
    assert(isEmpty(&s) == true);

    std::free(one);

    std::cout << "test_initStack passed\n";
}

// Тест push: одиночные и множественные добавления, проверка top и размера
static void test_push() {
    Stack s;
    initStack(&s);

    push(&s, 10);
    assert(getTop(&s) != nullptr && getTop(&s)->data == 10);
    assert(count_nodes(&s) == 1);

    push(&s, 20);
    assert(getTop(&s) != nullptr && getTop(&s)->data == 20);
    push(&s, 30);
    assert(getTop(&s) != nullptr && getTop(&s)->data == 30);
    assert(count_nodes(&s) == 3);

    for (int i = 0; i < 50; ++i) {
        push(&s, 1000 + i);
    }
    assert(count_nodes(&s) == 53);

    destroyStack(&s);
    s.top = nullptr;

    std::cout << "test_push passed\n";
}

// Тест pop: pop на пустом, pop до опустошения, повторные pop
static void test_pop() {
    Stack s;
    initStack(&s);

    pop(&s);
    assert(isEmpty(&s) == true);
    assert(getTop(&s) == nullptr);

    push(&s, 1);
    push(&s, 2);
    push(&s, 3);
    assert(count_nodes(&s) == 3);

    pop(&s);
    assert(count_nodes(&s) == 2);
    assert(getTop(&s) != nullptr && getTop(&s)->data == 2);

    pop(&s);
    pop(&s);
    assert(isEmpty(&s) == true);
    assert(getTop(&s) == nullptr);

    pop(&s);
    pop(&s);
    assert(isEmpty(&s) == true);

    destroyStack(&s);
    s.top = nullptr;

    std::cout << "test_pop passed\n";
}

// Тест getTop: корректность указателя и его изменение после мутаций
static void test_getTop() {
    Stack s;
    initStack(&s);

    assert(getTop(&s) == nullptr);

    push(&s, 42);
    const Node *t1 = getTop(&s);
    assert(t1 != nullptr && t1->data == 42);

    push(&s, 84);
    const Node *t2 = getTop(&s);
    assert(t2 != nullptr && t2->data == 84);
    assert(t2 != t1);

    pop(&s);
    const Node *t3 = getTop(&s);
    assert(t3 != nullptr && t3->data == 42);

    destroyStack(&s);
    s.top = nullptr;

    std::cout << "test_getTop passed\n";
}

// Тест isEmpty: жизненный цикл состояния пустоты
static void test_isEmpty() {
    Stack s;
    initStack(&s);
    assert(isEmpty(&s) == true);

    push(&s, 7);
    assert(isEmpty(&s) == false);

    pop(&s);
    assert(isEmpty(&s) == true);

    push(&s, 11);
    destroyStack(&s);
    assert(isEmpty(&s) == true);

    std::cout << "test_isEmpty passed\n";
}

// Тест searchByValue: пустой стек, верхний, средний, нижний, не найдено, дубликаты
static void test_searchByValue() {
    Stack s;
    initStack(&s);

    assert(searchByValue(&s, 9999) == nullptr);

    push(&s, 3);
    push(&s, 5);
    push(&s, 4);
    push(&s, 5);

    const Node *n_top = searchByValue(&s, 5);
    assert(n_top != nullptr);
    assert(n_top->data == 5);
    assert(n_top == getTop(&s));

    const Node *n_mid = searchByValue(&s, 4);
    assert(n_mid != nullptr && n_mid->data == 4);

    const Node *n_bot = searchByValue(&s, 3);
    assert(n_bot != nullptr && n_bot->data == 3);

    assert(searchByValue(&s, 42) == nullptr);

    destroyStack(&s);
    s.top = nullptr;

    std::cout << "test_searchByValue passed\n";
}

// Тест searchByIndex: корректные индексы, отрицательные и выход за пределы, поведение после pop
static void test_searchByIndex() {
    Stack s;
    initStack(&s);

    for (int i = 1; i <= 5; ++i) {
        push(&s, i);
    }
    assert(count_nodes(&s) == 5);

    const Node *n0 = searchByIndex(&s, 0);
    assert(n0 != nullptr && n0->data == 5);

    const Node *n2 = searchByIndex(&s, 2);
    assert(n2 != nullptr && n2->data == 3);

    const Node *n4 = searchByIndex(&s, 4);
    assert(n4 != nullptr && n4->data == 1);

    assert(searchByIndex(&s, -1) == nullptr);
    assert(searchByIndex(&s, 5) == nullptr);
    assert(searchByIndex(&s, 100) == nullptr);

    pop(&s);
    pop(&s);
    assert(count_nodes(&s) == 3);
    const Node *after_pop_index1 = searchByIndex(&s, 1);
    assert(after_pop_index1 != nullptr && after_pop_index1->data == 2);

    destroyStack(&s);
    s.top = nullptr;

    std::cout << "test_searchByIndex passed\n";
}

// Тест traverseStack: выполнение на пустом и непустом стеке (без проверки вывода)
static void test_traverseStack() {
    Stack s;
    initStack(&s);

    traverseStack(&s);

    push(&s, 100);
    push(&s, 200);
    push(&s, 300);
    traverseStack(&s);

    destroyStack(&s);
    s.top = nullptr;

    std::cout << "test_traverseStack passed\n";
}

// Тест destroyStack: освобождение и повторный вызов, проверка top и isEmpty
static void test_destroyStack() {
    Stack s;
    initStack(&s);

    push(&s, 9);
    push(&s, 8);

    destroyStack(&s);
    assert(s.top == nullptr);

    destroyStack(&s);
    assert(s.top == nullptr);

    assert(isEmpty(&s) == true);
    assert(getTop(&s) == nullptr);

    std::cout << "test_destroyStack passed\n";
}

int main() {

    test_createNode();
    test_initStack();
    test_push();
    test_pop();
    test_getTop();
    test_isEmpty();
    test_searchByValue();
    test_searchByIndex();
    test_traverseStack();
    test_destroyStack();

    return 0;
}

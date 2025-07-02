
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>

#include "Serialize.h"

void SerializeCollections()
{
    // tuple
    // std::tuple<int, bool> tuple;
    // Serialize::ToFile(tuple, "std_tuple");
    // Serialize::FromFile("std_tuple", tuple);

    // Deque
    std::deque<int> deque;
    auto iteratorDeque = deque.begin();

    for (size_t i = 0; i < 10; i++)
    {
        deque.emplace_back(i + 1);
    }

    Serialize::ToFile(deque, "std_deque.json");
    deque.clear();
    Serialize::FromFile("std_deque.json", deque);

    // priority_queue
    std::priority_queue<int> priority_queue;
    for (int i = 0; i < 10; i++)
    {
        priority_queue.push(i + 1);
    }
    Serialize::ToFile(priority_queue, "std_priority_queue.json");
    while (!priority_queue.empty())
    {
        priority_queue.pop();
    }
    Serialize::FromFile("std_priority_queue.json", priority_queue);

    // Queue
    std::queue<int> queue;
    for (size_t i = 0; i < 10; i++)
    {
        queue.push(i + 1);
    }
    Serialize::ToFile(queue, "std_queue.json");
    while (!queue.empty())
    {
        queue.pop();
    }
    Serialize::FromFile("std_queue.json", queue);

    // Stack
    std::stack<int> stack;
    for (size_t i = 0; i < 10; i++)
    {
        stack.push(i + 1);
    }
    Serialize::ToFile(stack, "std_stack.json");
    while (!stack.empty())
    {
        stack.pop();
    }
    Serialize::FromFile("std_stack.json", stack);

    // Array
    std::array<int, 10> arr;
    for (size_t i = 0; i < 10; i++)
    {
        arr[i] = i + 1;
    }
    Serialize::ToFile(arr, "std_array.json");
    for (size_t i = 0; i < 10; i++)
    {
        arr[i] = 0;
    }
    Serialize::FromFile("std_array.json", arr);

    // multiset
    std::multiset<int> multiset;
    for (int i = 0; i < 10; i++)
    {
        multiset.insert(i + 1);
    }
    Serialize::ToFile(multiset, "std_multiset.json");
    multiset.clear();
    Serialize::FromFile("std_multiset.json", multiset);

    // set
    std::set<int> set;
    set.begin();
    for (int i = 0; i < 10; i++)
    {
        set.insert(i + 1);
    }
    Serialize::ToFile(set, "std_set.json");
    set.clear();
    Serialize::FromFile("std_set.json", set);

    // multimap
    std::multimap<int, bool> multimap;

    for (size_t i = 0; i < 10; i++)
    {
        multimap.insert({ i + 1, i % 2 });
    }
    Serialize::ToFile(multimap, "std_multimap.json");
    multimap.clear();
    Serialize::FromFile("std_multimap.json", multimap);

    // Forward list
    std::forward_list<int> forward_list;
    std::forward_list<int>* ptr = &forward_list;
    for (size_t i = 0; i < 10; i++)
    {
        forward_list.emplace_front(i + 1);
    }
    for (auto& it : forward_list)
    {
        auto var = it;
    }

    Serialize::ToFile(forward_list, "std_forward_list.json");
    forward_list.clear();
    Serialize::FromFile("std_forward_list.json", forward_list);

    // List
    std::list<int> list;

    for (size_t i = 0; i < 10; i++)
    {
        list.emplace_back(i + 1);
    }
    Serialize::ToFile(list, "std_list.json");
    list.clear();
    Serialize::FromFile("std_list.json", list);
}
#include <vector>
#include <atomic>
#include <thread>
#include <iostream>

std::vector<int> queue_data;
std::atomic<int> count;

void populate_queue()
{
    std::cout << "populate_queue()==> \n";
    unsigned const number_of_items=20;
    queue_data.clear();
    for(unsigned i=0;i<number_of_items;++i)
    {
        queue_data.push_back(i);
    }
    
    count.store(number_of_items,std::memory_order_release);
    std::cout << "populate_queue()<== \n";
}

void wait_for_more_items()
{
}

void process(int id, int input)
{
    std::cout << "process " << id << " input: " << input << " \n";
}

void consume_queue_items(int id)
{
    std::cout << "consume_queue_items(" << id << ")==> \n ";
    while(true)
    {
        int item_index;
        if((item_index=count.fetch_sub(1,std::memory_order_acquire))<=0)
        {
            // wait_for_more_items();
            continue;
        }
        process(id, queue_data.at(item_index-1));
    }
    std::cout << "consume_queue_items(" << id << ")<== \n ";
}

int main()
{
    std::thread a(populate_queue);
    std::thread b(consume_queue_items, 46);
    std::thread c(consume_queue_items, 47);
    a.join();
    b.join();
    c.join();
}

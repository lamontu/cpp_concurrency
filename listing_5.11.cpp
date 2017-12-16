#include <vector>
#include <atomic>
#include <thread>
#include <stdio.h>

std::vector<int> queue_data;
std::atomic<int> count;

void populate_queue()
{
    printf("populate_queue()==> \n");
    unsigned const number_of_items=20;
    queue_data.clear();
    for(unsigned i=0;i<number_of_items;++i)
    {
        queue_data.push_back(i);
    }
    
    count.store(number_of_items,std::memory_order_release);
    printf("populate_queue()<== \n");
}

void wait_for_more_items()
{
}

void process(int id, int input)
{
    printf("process %d input: %d \n", id, input);
}

void consume_queue_items(int id)
{
    printf("consume_queue_items(%d)==> \n", id);
    while(true)
    {
        int item_index;
        if((item_index=count.fetch_sub(1,std::memory_order_acquire))<=0)
        {
            // wait_for_more_items();
            std::this_thread::yield();
            continue;
        }
        process(id, queue_data.at(item_index-1));
    }
    printf("consume_queue_items(%d)<== \n", id);
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

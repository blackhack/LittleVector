#include <LittleVector.h>

void setup()
{
    Serial.begin(115200);

    LittleVector<int> my_vector;

    // Because I know that I will be storing at most 100 numbers,
    // I can call .reserve(100), so the program only needs to allocate memory once,
    // which is way faster and avoids memory fragmentation.
    
    my_vector.reserve(100);

    // We can see that our capacity is now 100, even though our size is still 0,
    // because we haven't inserted anything yet into the vector.

    Serial.println("Before inserting items: ");
    Serial.print("Size: ");
    Serial.println(my_vector.size());
    Serial.print("Capacity: ");
    Serial.println(my_vector.capacity());

    // Let's fill it
    for (int i = 0; i < 100; ++i)
        my_vector.push_back(i);

    // Now that we inserted 100 items, our size is the same as our capacity

    Serial.println("After inserting items: ");
    Serial.print("Size: ");
    Serial.println(my_vector.size());
    Serial.print("Capacity: ");
    Serial.println(my_vector.capacity());

    // But this doesn't mean we can't keep inserting items.
    // This will work just fine; the difference is that it will now
    // cause an allocation and copy of every item in the new memory region.
    // This is how it usually works, but because embedded systems
    // like Arduino Uno are usually very memory-limited,
    // this can be slow and, worse, it can fragment our memory.
    // But it only happens if we use it massively; some allocations are not harmful.

    my_vector.push_back(100);

    Serial.println("After inserting one item more: ");
    Serial.print("Size: ");
    Serial.println(my_vector.size());
    Serial.print("Capacity: ");
    Serial.println(my_vector.capacity());

    // By the way, our capacity doesn't grow one by one; it grows by a factor of 2.
    // This is used to reduce allocations and copies, but can use a lot of memory.
    // However, in this implementation, that can be disabled by calling:
    // my_vector.SetOverAllocation(false);
    // But it's still better to just reserve what we need, or at least an approximation of it.

    Serial.println("Iterating on our vector: ");
    Serial.print('|');
    for (auto number : my_vector)
    {
        Serial.print(number);
        Serial.print('|');
    }
}

void loop()
{
}

#include <LittleVector.h>

void setup()
{
    Serial.begin(115200);

    LittleVector<int> my_vector;
    my_vector.reserve(50);

    // Let's fill out vector
    for (int i = 0; i < 50; ++i)
        my_vector.push_back(i);

    // Iterate over our vector, checking and deleting even numbers.
    for (LittleVector<int>::iterator itr = my_vector.begin(); itr != my_vector.end();)
    {
        if ((*itr) % 2 == 0)
            itr = my_vector.erase(itr);
        else
            ++itr;
    }

    // Let's see what is inside our vector. This time, let's just use a range-based loop.
    // This is a nice way to access the contents without needing the iterator object.
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

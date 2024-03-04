#include <LittleVector.h>

void setup()
{
    Serial.begin(115200);

    {
        LittleVector<int> my_vector;
        my_vector.reserve(200);

        unsigned long start = micros();

        for (int i = 0; i < 200; ++i)
            my_vector.push_back(i);

        unsigned long end = micros();

        Serial.println("---- Using reserve ----");
        Serial.print("Elapse time: ");
        Serial.print(end - start);
        Serial.println(" us");
        Serial.print("Used vector memory: ");
        Serial.print(my_vector.capacity() * sizeof(int));
        Serial.println(" bytes");
    }

    {
        LittleVector<int> my_vector;

        unsigned long start = micros();

        for (int i = 0; i < 200; ++i)
            my_vector.push_back(i);

        unsigned long end = micros();

        Serial.println("---- Without reserve ----");
        Serial.print("Elapse time: ");
        Serial.print(end - start);
        Serial.println(" us");
        Serial.print("Used vector memory: ");
        Serial.print(my_vector.capacity() * sizeof(int));
        Serial.println(" bytes");
    }

    {
        LittleVector<int> my_vector;
        my_vector.SetOverAllocation(false);

        unsigned long start = micros();

        for (int i = 0; i < 200; ++i)
            my_vector.push_back(i);

        unsigned long end = micros();

        Serial.println("---- Without reserve and disabling over allocation ----");
        Serial.print("Elapse time: ");
        Serial.print(end - start);
        Serial.println(" us");
        Serial.print("Used vector memory: ");
        Serial.print(my_vector.capacity() * sizeof(int));
        Serial.println(" bytes");
    }
}

void loop()
{
}

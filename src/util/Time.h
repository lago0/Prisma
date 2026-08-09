#include <chrono>

class Time
{
    public:
        static double GetUnixTimestamp()
        {
            auto now = std::chrono::system_clock::now();

            auto duration = now.time_since_epoch();

            std::chrono::duration<double> decimal_seconds = duration;
            return decimal_seconds.count();
        }
};

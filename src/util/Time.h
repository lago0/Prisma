#include <chrono>

/**
 * @class Time
 * @brief Static class for time handling.
 * @author lago0
 */
class Time
{
    public:
        /**
         * @brief Gets unix timestamp.
         * @returns double with the current timestamp
         */
        static double GetUnixTimestamp()
        {
            auto now = std::chrono::system_clock::now();

            auto duration = now.time_since_epoch();

            std::chrono::duration<double> decimal_seconds = duration;
            return decimal_seconds.count();
        }
};

#pragma once

#include <functional>
#include <vector>

namespace Util 
{
    /**
     * @class Signal
     * @brief Class for event handling.
     * @author lago0
     */
    template <typename ...Args>
    class Signal
    {
        using Callback = std::function<void(Args...)>;

        public:
            Signal() = default;
            
            /**
             * @brief Emits the signal to all conected callbacks.
             * @param args All the arguments that will be sent to the callback
             * @author lago0
             */
            void Emit(Args... args)
            {
                for (const Callback& callback : callbacks)
                {
                    callback(args...);
                }
            }

            /**
             * @brief Connects a function, so it is called when signal is emmitted.
             * @param callback Function that will be called
             * @author lago0
             */
            void Connect(Callback callback)
            {
                callbacks.push_back(callback);
            }
        
        private:
            std::vector<Callback> callbacks;
    };
}

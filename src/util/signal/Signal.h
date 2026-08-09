#pragma once

#include <functional>
#include <vector>

namespace Util 
{
    template <typename ...Args>
    class Signal
    {
        using Callback = std::function<void(Args...)>;

        public:
            Signal() = default;
            
            void Emit(Args... args)
            {
                for (const Callback& callback : callbacks)
                {
                    callback(args...);
                }
            }

            void Connect(Callback callback)
            {
                callbacks.push_back(callback);
            }
        
        private:
            std::vector<Callback> callbacks;
    };
}

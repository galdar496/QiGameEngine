//
//  Timer.h
//  Qi Game Engine
//
//  Created by Cody White on 1/10/15.
//  Copyright (c) 2015 Cody White. All rights reserved.
//

#pragma once

#include <chrono>

namespace Qi
{

///
/// Wall clock timer.
///
class Timer
{
    public:
    
        ///
        /// Using a high-resolution clock for the timer.
        ///
        typedef std::chrono::high_resolution_clock Clock;
    
        Timer()  {}
        ~Timer() {}
    
        ///
        /// Start the timer.
        ///
        inline void start()
        {
            m_start_time = Clock::now();
        }
    
        ///
        /// Stop the timer.
        /// @return Time in seconds since the last call to start().
        ///
        inline float stop()
        {
            std::chrono::duration<float> duration = Clock::now() - m_start_time;
            float delta_time = duration.count();
            return delta_time;
        }

        /// Get the change in time since the last call to either start() or dt(). This function
        /// will restart the timer before returning.
        /// @return Elapsed time in seconds.
        ///
        inline float dt()
        {
            float delta_time = stop();
            start();
            return delta_time;
        }
        
    private:
        
        Clock::time_point m_start_time; ///< Time recorded with a call to start().
        
};
    
} // namespace Qi



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
        inline void Start()
        {
            m_startTime = Clock::now();
        }
    
        ///
        /// Stop the timer.
        /// @return Time in seconds since the last call to start().
        ///
        inline float Stop()
        {
            std::chrono::duration<float> duration = Clock::now() - m_startTime;
            float deltaTime = duration.count();
            return deltaTime;
        }

        /// Get the change in time since the last call to either start() or dt(). This function
        /// will restart the timer before returning.
        /// @return Elapsed time in seconds.
        ///
        inline float Dt()
        {
            float deltaTime = Stop();
            Start();
            return deltaTime;
        }
        
    private:
        
        Clock::time_point m_startTime; ///< Time recorded with a call to start().
        
};
    
} // namespace Qi



#pragma once

#include <chrono>
#include <array>

namespace app {
    /* fan stage data structure. do not change. */
    struct fan_stage {
        const unsigned int temperature;
        const unsigned int fan_speed_pct;
    };

    /*
     * fan stages. this would be the "fan curve".
     * this array is read pairwise (i, then i+1).
     *
     * the temperature readings from the gpu are checked against which one it's
     * closest towards (left or right value from the pairwise iteration over
     * fan_stages) and sets the temperature speed to that value.
     *
     * for example, between 60 (fan speed 55% in this config) and 70 (fan speed
     * 65% in this config), the fan speed will be set to 65% if the temperature
     * hits 66 C.
     *
     * this is probably not supposed to work like that, but ehh.
     *
     * in case the temperature readings don't test against any pair of
     * fan_stages, the fan speed will be set to the result of the following:
     *
     * fan_spd = max(min_gpu_fan_percent, min(round(current_temp / 10.0) * 10.0, 100)
     * 
     * temperature values should be sequential. do not mess this up.
     */
    static constexpr std::array fan_stages {
        fan_stage { .temperature =  0, .fan_speed_pct = 20 },
        fan_stage { .temperature = 20, .fan_speed_pct = 20 },
        fan_stage { .temperature = 30, .fan_speed_pct = 30 },
        fan_stage { .temperature = 35, .fan_speed_pct = 35 },
        fan_stage { .temperature = 40, .fan_speed_pct = 45 },
        fan_stage { .temperature = 50, .fan_speed_pct = 50 },
        fan_stage { .temperature = 60, .fan_speed_pct = 55 },
        fan_stage { .temperature = 70, .fan_speed_pct = 65 },
        fan_stage { .temperature = 80, .fan_speed_pct = 85 }
    };

    /*
     * every call to nvmlDeviceSetFanSpeed_v2 will have the fan speed pct
     * argument clamped to (min_gpu_fan_percent, x, 100u).
     *
     * do not change.
     */
    static constexpr unsigned int min_gpu_fan_percent { 25 };

    /*
     * interval between each reading of the GPU temperature.
     */
    using namespace std::literals::chrono_literals;
    static constexpr auto fan_control_thread_sleep_interval = 2s;
}

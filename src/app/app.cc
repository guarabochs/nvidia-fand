#include <app/app.hh>
#include <app/util.hh>
#include <app/fan_settings.hh>

#include <thread>
#include <atomic>
#include <ranges>
#include <memory>
#include <print>
#include <cmath>
#include <algorithm>

namespace app {
    app::app(const program_arguments&& args) : launch_arguments { std::move(args) } {
        nvml_call(nvmlInit_v2);
        
        nvmlDevice_t dev;
        nvml_call(nvmlDeviceGetHandleByIndex_v2, 0u, &dev);

        this->nvml_device.emplace(dev);

        constexpr size_t max_query_len = 128;
        std::unique_ptr<char[]> query_buffer { new char[max_query_len] };

        nvml_call(nvmlDeviceGetName, nvml_device.value(), query_buffer.get(), max_query_len);
        std::println("device 0 name is {}", query_buffer.get());
    }

    auto app::run() -> void {
        fan_control_thread_running = true;
        fan_control_thread_loop();
    }

    app::~app() {
        fan_control_thread_running = false;

        nvml_call(nvmlDeviceSetDefaultFanSpeed_v2, nvml_device.value(), 0);
        nvml_call(nvmlShutdown);
    }

    auto app::fan_control_thread_loop() -> void {
        while (fan_control_thread_running) {
            unsigned int temperature_reading {};
            unsigned int fan_speed_reading {};

            nvml_call(nvmlDeviceGetTemperature, nvml_device.value(), NVML_TEMPERATURE_GPU, &temperature_reading);
            nvml_call(nvmlDeviceGetFanSpeed_v2, nvml_device.value(), 0, &fan_speed_reading);

            bool suitable_temperature_range_found { false };
            for (const auto [left, right] : fan_stages | std::views::pairwise) {
                if (
                    temperature_reading >= left.temperature
                    && temperature_reading <= right.temperature
                ) {
                    const auto towards = right.temperature - ((right.temperature - left.temperature) / 2);
                    const bool towards_right = temperature_reading > towards;

                    unsigned int next_fan_speed =
                        std::clamp(min_gpu_fan_percent, towards_right ? right.fan_speed_pct : left.fan_speed_pct, 100u);

                    if (towards > 100) {
                        std::println("! erroneous division detected; falling back towards right");
                        next_fan_speed = right.fan_speed_pct;
                    }

                    std::println("- temperature reading is @ {}•C; setting fan towards {} (stage temp.: {}•C; fan spd: {}; twd: {}; next fan spd: {})",
                        temperature_reading,
                        towards_right ? "right" : "left",
                        towards_right ? right.temperature : left.temperature,
                        towards_right ? right.fan_speed_pct : left.fan_speed_pct,
                        towards,
                        next_fan_speed
                    );

                    nvml_call(nvmlDeviceSetFanSpeed_v2,
                        nvml_device.value(),
                        0, std::clamp(min_gpu_fan_percent, next_fan_speed, 100u)
                    );

                    suitable_temperature_range_found = true;
                    break;
                }
            }

            if (!suitable_temperature_range_found) {
                std::println("! no suitable temperature range found, falling back to temp/10 quantized");

                double next_fan_speed_dbl = 
                    std::round(static_cast<double>(temperature_reading) / 10.0) * 10;

                unsigned int next_fan_speed_uint =
                    std::clamp(min_gpu_fan_percent, static_cast<unsigned int>(next_fan_speed_dbl), 100u);

                nvml_call(nvmlDeviceSetFanSpeed_v2,
                    nvml_device.value(), 0, next_fan_speed_uint
                );
            }

            std::println("* temperature reading is {}•C; fan spd reading is {}%", temperature_reading, fan_speed_reading);
            std::this_thread::sleep_for(fan_control_thread_sleep_interval);
        }
    }
}

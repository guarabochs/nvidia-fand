#pragma once

#include <thread>
#include <atomic>
#include <concepts>
#include <array>

#include <app/util.hh>

#include <nvml.h>

namespace app {
    class nvml_error : public std::logic_error {
        public:
            explicit nvml_error(auto message) : logic_error(message) {}
    };

    template<typename ReturnType = nvmlReturn_t, typename... Args>
    auto nvml_call(auto call, Args... args) -> nvmlReturn_t  {
        const nvmlReturn_t ret = call(args...);

        if (ret != NVML_SUCCESS) {
            constexpr auto get_error_message = [](nvmlReturn_t return_code) {
                switch (return_code) {
                    case 0:   return "The operation was successful.";
                    case 1:   return "NVML was not first initialized with nvmlInit().";
                    case 2:   return "A supplied argument is invalid.";
                    case 3:   return "The requested operation is not available on target device.";
                    case 4:   return "The current user does not have permission for operation.";
                    case 5:   return "Deprecated: Multiple initializations are now allowed through ref counting.";
                    case 6:   return "A query to find an object was unsuccessful.";
                    case 7:   return "An input argument is not large enough.";
                    case 8:   return "A device's external power cables are not properly attached.";
                    case 9:   return "NVIDIA driver is not loaded.";
                    case 10:  return "User provided timeout passed.";
                    case 11:  return "NVIDIA Kernel detected an interrupt issue with a GPU.";
                    case 12:  return "NVML Shared Library couldn't be found or loaded.";
                    case 13:  return "Local version of NVML doesn't implement this function.";
                    case 14:  return "infoROM is corrupted";
                    case 15:  return "The GPU has fallen off the bus or has otherwise become inaccessible.";
                    case 16:  return "The GPU requires a reset before it can be used again.";
                    case 17:  return "The GPU control device has been blocked by the operating system/cgroups.";
                    case 18:  return "RM detects a driver/library version mismatch.";
                    case 19:  return "An operation cannot be performed because the GPU is currently in use.";
                    case 20:  return "Insufficient memory.";
                    case 21:  return "No data.";
                    case 22:  return "The requested vgpu operation is not available on target device, becasue ECC is enabled.";
                    case 23:  return "Ran out of critical resources, other than memory.";
                    case 24:  return "Ran out of critical resources, other than memory.";
                    case 25:  return "The provided version is invalid/unsupported.";
                    case 26:  return "The requested functionality has been deprecated.";
                    case 27:  return "The system is not ready for the request.";
                    case 28:  return "No GPUs were found.";
                    case 29:  return "Resource not in correct state to perform requested operation.";
                    case 999: return "An internal driver error occurred";
                }

                return "Unknown error.";
            };

            scream_and_abort(error_from_std_exception(
                nvml_error { std::format("nvml error {} ({})", static_cast<unsigned>(ret), get_error_message(ret)) }
            ));
        }

        return ret;
    }

    class app final {
        using consolidated_program_arguments = const std::vector<std::string_view>;

        // class members
        private:
            const consolidated_program_arguments launch_arguments;

            // ...
            std::optional<nvmlDevice_t> nvml_device;
            std::atomic<bool> fan_control_thread_running;

        public:
            explicit app(program_arguments&& arguments);
            ~app();

            // class functions
            auto run() -> void;

        private:
            // class private functions
            auto fan_control_thread_loop() -> void;
    };
}

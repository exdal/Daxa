#pragma once

#include "../DaxaCore.hpp"

#include <memory>
#include <vector>
#include <deque>
#include <span>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "Handle.hpp"
#include "Image.hpp"
#include "SwapchainImage.hpp"
#include "Signal.hpp"
#include "DeviceBackend.hpp"

namespace daxa {
	namespace gpu {

		VkSurfaceKHR createSurface(void* sdlWindowHandle, VkInstance instance);

		struct SwapchainCreateInfo{
			VkSurfaceKHR surface 				= VK_NULL_HANDLE;
			u32 width 							= 256;
			u32 height			 				= 256;
			VkPresentModeKHR presentMode 		= VK_PRESENT_MODE_FIFO_KHR;
			VkImageUsageFlags additionalUses 	= {};
			char const* debugName 				= {};
		};

		class Swapchain {
		public:
			Swapchain() 								= default;
			Swapchain(Swapchain&&) noexcept;
			Swapchain& operator=(Swapchain&&) noexcept;
			Swapchain(Swapchain const&) 				= delete;
			Swapchain& operator=(Swapchain const&) 		= delete;
			~Swapchain();

			void resize(VkExtent2D newSize);

			void setPresentMode(VkPresentModeKHR newPresentMode);

			SwapchainImage aquireNextImage();

			VkExtent2D getSize() const { return size; }

			VkFormat getVkFormat() const { return swapchainImageFormat; }

			std::string const& getDebugName() const { return debugName; }
		private:
			friend class Device;

			void construct(std::shared_ptr<DeviceBackend> deviceBackend, SwapchainCreateInfo ci);

			std::shared_ptr<DeviceBackend> 	deviceBackend 			= {};
			VkFence 						aquireFence 			= VK_NULL_HANDLE;
			VkPresentModeKHR 				presentMode 			= VK_PRESENT_MODE_FIFO_KHR;
			VkSurfaceKHR 					surface 				= VK_NULL_HANDLE;
			VkSwapchainKHR 					swapchain 				= VK_NULL_HANDLE; 
			VkFormat 						swapchainImageFormat 	= {};
			std::vector<ImageViewHandle> 	swapchainImages			= {};
			VkExtent2D 						size 					= {}; 
			VkImageUsageFlags 				additionalimageUses		= {};
			std::string 					debugName 				= {};
		};

		class SwapchainHandle : public SharedHandle<Swapchain>{};
	}
}

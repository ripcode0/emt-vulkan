### Vulkan-Hpp

* GitHub: https://github.com/KhronosGroup/Vulkan-Hpp
* 공식 Vulkan C++ 바인딩으로, Vulkan 코드를 현대적인 C++ 스타일로 작성할 수 있도록 도와줍니다.
* RAII 지원으로 리소스 관리를 편리하게 합니다.
* 보일러플레이트를 크게 줄일 수 있습니다.

```cpp
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
```

```cpp
int main() {
    try {
        // Vulkan 애플리케이션 정보
        vk::ApplicationInfo appInfo{
            "VulkanHppExample", VK_MAKE_VERSION(1, 0, 0),
            "No Engine", VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_3
        };

        // 필요한 확장 목록 얻기 (예: 윈도우 시스템과 연계할 경우)
        std::vector<const char*> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
        };

        // Vulkan 인스턴스 생성 정보
        vk::InstanceCreateInfo instanceInfo{
            {}, &appInfo,
            0, nullptr,                      // 레이어 없음
            static_cast<uint32_t>(extensions.size()), extensions.data()
        };

        // 인스턴스 생성
        vk::Instance instance = vk::createInstance(instanceInfo);

        std::cout << "Vulkan 인스턴스가 성공적으로 생성되었습니다.\n";

        // 물리 디바이스 목록 얻기
        auto physicalDevices = instance.enumeratePhysicalDevices();
        std::cout << "사용 가능한 Vulkan 디바이스 수: " << physicalDevices.size() << '\n';

        // 인스턴스 삭제
        instance.destroy();
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Vulkan 오류 발생: " << err.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::exception& ex) {
        std::cerr << "기타 오류 발생: " << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

* 이 예시는 vulkan.hpp를 통해 기존의 C 기반 Vulkan API를 보다 간결하고 객체지향적으로 다룰 수 있게 합니다.

```cpp
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>

int main() {
    try {
        // Vulkan 애플리케이션 정보 설정
        vk::ApplicationInfo appInfo{
            "Vulkan Physical Device", VK_MAKE_VERSION(1, 0, 0),
            "No Engine", VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_3
        };

        // Vulkan 인스턴스 생성
        vk::InstanceCreateInfo instanceInfo{{}, &appInfo};
        vk::Instance instance = vk::createInstance(instanceInfo);

        // 사용 가능한 모든 물리 디바이스 가져오기
        auto physicalDevices = instance.enumeratePhysicalDevices();
        if (physicalDevices.empty()) {
            throw std::runtime_error("사용 가능한 Vulkan GPU를 찾을 수 없습니다.");
        }

        std::cout << "발견된 GPU 수: " << physicalDevices.size() << '\n';

        // 물리 디바이스 정보 출력 및 선택
        for (size_t i = 0; i < physicalDevices.size(); ++i) {
            auto deviceProperties = physicalDevices[i].getProperties();
            std::cout << "Device [" << i << "] 이름: " 
                      << deviceProperties.deviceName << '\n';
            std::cout << "API Version: " 
                      << VK_VERSION_MAJOR(deviceProperties.apiVersion) << "."
                      << VK_VERSION_MINOR(deviceProperties.apiVersion) << "."
                      << VK_VERSION_PATCH(deviceProperties.apiVersion) << '\n';
        }

        // 가장 적합한 디바이스 선택 예시 (첫 번째 디바이스 선택)
        vk::PhysicalDevice physicalDevice = physicalDevices[0];
        std::cout << "선택된 GPU: " 
                  << physicalDevice.getProperties().deviceName << '\n';

        // Queue Family 정보 출력
        auto queueFamilies = physicalDevice.getQueueFamilyProperties();
        std::cout << "Queue Family 개수: " << queueFamilies.size() << '\n';

        for (size_t i = 0; i < queueFamilies.size(); ++i) {
            const auto& queueFamily = queueFamilies[i];
            std::cout << "Queue Family [" << i << "]\n";
            std::cout << "  Queue 개수: " << queueFamily.queueCount << '\n';
            std::cout << "  그래픽 지원 여부: " 
                      << ((queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) ? "Yes" : "No") << '\n';
        }

        // 정리
        instance.destroy();
    }
    catch (const vk::SystemError& err) {
        std::cerr << "Vulkan 에러: " << err.what() << '\n';
        return EXIT_FAILURE;
    }
    catch (const std::exception& ex) {
        std::cerr << "일반 예외: " << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

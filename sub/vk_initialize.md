### Vulkan 1.3

#### instance 초기화

### `vkEnumerateInstanceExtensionProperties`

| 항목        | 설명 |
|-------------|------|
| **목적**     | Vulkan **확장(Extension)** 리스트를 조회 |
| **대상**     | `VK_KHR_surface`, `VK_EXT_debug_utils` 같은 **기능 확장** |
| **파라미터** | `nullptr` → 전체 확장 조회<br>`layerName` → 특정 레이어의 확장만 조회 |
| **사용처**   | `VkInstanceCreateInfo.ppEnabledExtensionNames` 설정 전에 사용 |
| **예시 확장** | `VK_KHR_surface`, `VK_KHR_win32_surface`, `VK_EXT_debug_utils` 등 |

#### **특징**  
- 플랫폼별 기능 사용을 위해 **필수**
- 확장을 활성화하지 않으면 해당 기능을 사용할 수 없음  
- ex) Win32 창을 렌더링하려면 `VK_KHR_win32_surface` 확장이 필요

---

### `vkEnumerateInstanceLayerProperties`

| 항목        | 설명 |
|-------------|------|
| **목적**     | Vulkan **레이어(Layer)** 리스트를 조회 |
| **대상**     | 디버깅, 검증, 툴 용도의 레이어 |
| **파라미터** | 없음 (`nullptr`) |
| **사용처**   | `VK_LAYER_KHRONOS_validation` 등의 레이어 존재 확인 |
| **예시 레이어** | `VK_LAYER_KHRONOS_validation`, `VK_LAYER_LUNARG_api_dump` 등 |

#### **특징**  
- 기능을 추가하는 것이 아니라, **검증, 디버깅, 로깅**을 위한 도구  
- 보통 **개발 중에만 사용**하고, 릴리즈 빌드에서는 비활성화

---

### 차이 요약

| 항목       | Extension                             | Layer                                |
|------------|----------------------------------------|---------------------------------------|
| **역할**    | Vulkan 기능 확장 (surface, debug 등)   | 검증, 로그, 디버깅 기능 추가            |
| **필수 여부**| 대부분 필수 (`VK_KHR_surface` 등)       | 선택 사항 (디버깅 전용)                 |
| **등록 위치**| `ppEnabledExtensionNames`             | `ppEnabledLayerNames`                |
| **관련 API** | `vkEnumerateInstanceExtensionProperties` | `vkEnumerateInstanceLayerProperties` |

---

#### 예시 코드 스니펫

```cpp
uint32_t ext_count = 0;
vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
// ... 확장 목록 확인 후 조건부 추가

uint32_t layer_count = 0;
vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
// ... validation layer 사용 가능 여부 판단
```
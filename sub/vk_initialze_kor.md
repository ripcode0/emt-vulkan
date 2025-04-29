## Vulkan 초기화

`OpenGL`은 즉각적인 그래픽 명령 실행을 제공했던 것과 달리, `Vulkan`은 비교적 긴 초기화 단계를 거친다. 이 과정을 간소화하기 위해 `VkBootstrap` 라이브러리를 사용하면 보일러플레이트 코드를 많이 줄일 수 있다.

Vulkan은 명시적이며 "직접적인" 제어를 제공하는 API로, GPU 선택, 확장(extension) 로드, 초기 `VkInstance` 및 `VkDevice` 구조체를 생성해야 한다.

Vulkan에는 OpenGL과 같은 전역 상태 (global state) 가 없기 때문에, 모든 API 호출에 `VkDevice`나 `VkInstance`를 전달해야 한다. 본 가이드에서는 `vulkan-1.dll`에 직접 링크하여 함수를 로드하는 방식을 사용하지만, 함수 포인터를 수동으로 로드할 수도 있다. 확장 함수들은 별도로 로드해야 하며, Volk 라이브러리를 통해 이를 쉽게 수행할 수 있다.

---

### VkInstance

모든 작업의 시작점인 `VkInstance`는 Vulkan API의 컨텍스트를 나타낸다. 인스턴스 생성 시 다음을 고려한다:

- 검증(validation) 레이어 활성화
- 필수 인스턴스 확장(VK_KHR_surface 등) 활성화
- 에러 및 로그 출력을 위한 로거 설정

애플리케이션은 일반적으로 단 하나의 `VkInstance`만 생성하며, 이는 애플리케이션의 글로벌 Vulkan 컨텍스트로 작동한다.

---

### VkPhysicalDevice

`VkInstance`를 통해 시스템에서 사용 가능한 GPU 목록과 기능을 조회할 수 있다. 이 정보는 `VkPhysicalDevice` 핸들로 제공되며 GPU를 참조한다.

- 게임용 PC는 보통 단일 GPU만 존재하여 선택의 여지가 적다.
- 노트북은 일반적으로 CPU 내장 저전력 GPU와 독립된 고성능 GPU 두 가지를 제공한다.
- 사용자가 배터리 관리 목적으로 저전력 GPU 선택권을 가지는 것이 이상적이다.

`VkPhysicalDevice`에서 얻을 수 있는 정보:

- GPU 기능 지원 여부
- VRAM 용량
- 사용 가능한 확장(extension)

---

### VkDevice

선택한 `VkPhysicalDevice` 를 바탕으로 생성되는 `VkDevice`는 실제 GPU 드라이버로, 대부분의 `Vulkan` 명령에 필요하다.

- 활성화할 확장(extension)은 필요한 것만 사용하는 것이 권장된다(성능 영향 방지).
- 여러 개의 `VkDevice` 를 동시에 관리하여 다중 GPU를 활용할 수 있다(본 튜토리얼에서는 다루지 않음).

---

### Swapchain

화면에 렌더링할 때는 OS 또는 윈도우 시스템이 제공하는 `Swapchain`을 사용한다.

- 플랫폼마다 다르게 제공되며 `Vulkan` 핵심 스펙이 아닌 선택적 기능이다.
- 컴퓨트 셰이더나 오프라인 렌더링 사용 시 필요하지 않다.

`Swapchain`의 주요 특징:

- 윈도우 크기가 변경되면 `Swapchain`을 재생성해야 한다.
- 이미지 포맷은 플랫폼 및 GPU에 따라 달라질 수 있으므로 반드시 확인해야 한다.
- 일반적으로 더블 버퍼링(2장) 또는 트리플 버퍼링(3장)을 사용한다.

#### Swapchain의 Present Mode

Present Mode는 `Swapchain`과 화면 표시의 동기화 방식을 결정한다.

| Present Mode | 설명 |
|--------------|------|
| VK_PRESENT_MODE_IMMEDIATE_KHR | 즉시 표시(화면 찢어짐 발생 가능성 높음) |
| VK_PRESENT_MODE_FIFO_KHR | VSync 활성화로 화면 FPS에 맞춰 애플리케이션 FPS 제한 |
| VK_PRESENT_MODE_FIFO_RELAXED_KHR | VSync 방식이나, 화면 FPS보다 낮으면 즉시 표시하여 찢어짐 발생 가능 |
| VK_PRESENT_MODE_MAILBOX_KHR | 트리플 버퍼링 방식으로, 항상 최신 이미지 선택(VSync 강제성 없음) |

- 일반적으로 `MAILBOX` 또는 두 가지 `FIFO` 모드를 사용한다.
- 본 가이드에서는 렌더링 속도 제한과 FPS 과열 방지를 위해 `FIFO_RELAXED` 모드를 사용할 예정이며, 실제 애플리케이션은 주로 `MAILBOX` 모드를 선호한다.


### 프레임 동시성(Frame-in-flight)
* `Vulkan`에서는 `CPU`가 한 프레임의 커맨드를 작성하는 동안, `GPU`는 이전 프레임을 렌더링 중일 수 있습니다.

* 이렇게 하면 `CPU`가 `GPU`를 기다리지 않고 다음 프레임 준비를 동시에 진행할 수 있습니다.

* 하지만 CPU가 GPU보다 너무 앞서 나가면 안 되기 때문에, **제한을 둡니다.** 이 제한이 바로 `MAX_CONCURRENT_FRAMES`입니다.
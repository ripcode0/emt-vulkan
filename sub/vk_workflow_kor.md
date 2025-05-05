## Vulkan 기본 랜더링 워크플로우

### 1. 인스턴스와 물리 디바이스 초기화
* `Vulkan 인스턴스` 생성 (애플리케이션 정보 + 확장)
* `디버그 메시지 콜벡` 설정 (옵션)
* `표면(Surface)` 생성 (윈도우 시스템에 맞게)
* `물리 디바이스` 선택 (GPU 탐색 및 평가)

### 2. 논리 디바이스 및 큐 설정
* 그래픽 & 프레젠트 `큐 패밀리` 선택
* `논리 디바이스` 생성
* `디바이스 큐` 핸들 획득

### 3. 스왑체인 및 관련 이미지 설정
* `표면 포맷`, `프레젠트` 모드, 해상도 선택
* `스왑체인` 생성
* `스왑체인 이미지` 핸들 획득
* `이미지 뷰` 생성

### 4. 렌더 패스 및 프레임버퍼 준비
* `렌더패스` 정의 (컬러 어태치먼트) : `동적 랜더링`이면 패스해도 됨
* 프레임버퍼 생성 (렌더패스 + 이미지뷰 연결)

### 5. 그래픽 파이프라인 준비
* `셰이더 모듈` 로딩 (Vertex, Fragment)
* `파이프라인 레이아웃` 설정 (`디스크립터`, `푸시상수` 등)
* 그래픽 `파이프라인` 생성 (`입력 어셈블리`, `뷰포트`, `래스터라이저` 등 포함)

### 6. 커맨드 버퍼 및 싱크 객체 준비
* `커맨드 풀` 생성
* 프레임 수만큼 `커맨드 버퍼` 생성
* `세마포어` 및 `펜스` 생성

### 7. 정점 버퍼 준비
* `정점 버퍼` 생성 및 업로드 (삼각형 정점 좌표 등)

### 8. 렌더링 루프 실행
* 이전 `프레임` `펜스` 대기 및 리셋
* `스왑체인` 이미지 획득
* `커맨드 버퍼` 기록 시작
* 렌더패스 시작 (`vkCmdBeginRenderPass`) or 동적 렌더링(`vkCmdBeginRendering`)
* `파이프라인` 바인딩
* `정점 버퍼` 바인딩
* 드로우 호출
* `렌더 패스` 종료
* `커맨드` 제출 (세마포어 동기화 포함)
* 이미지 `프레젠트`
```cpp
while (!windowShouldClose()) {
    // 1. 이전 프레임 펜스 대기 및 리셋
    vkWaitForFences(...); vkResetFences(...);

    // 2. 스왑체인 이미지 획득
    vkAcquireNextImageKHR(...);

    // 3. 커맨드 버퍼 기록 시작
    vkBeginCommandBuffer(...);

    // 4. 렌더패스 시작 (`vkCmdBeginRenderPass`) or 동적 렌더링 (`vkCmdBeginRendering`)
    // 5. 파이프라인 바인딩: vkCmdBindPipeline(...)
    // 6. 정점 버퍼 바인딩: vkCmdBindVertexBuffers(...)
    // 7. 드로우 호출: vkCmdDraw(...);
    // 8. 렌더패스 종료: vkCmdEndRenderPass(...) or vkCmdEndRendering(...)

    vkEndCommandBuffer(...);

    // 9. 커맨드 제출: vkQueueSubmit(...)
    // waitSemaphore (image available), signalSemaphore (render finished)

    // 10. 프레젠트 요청: vkQueuePresentKHR(...)
}

```

### 9. 종료 처리
* `디바이스` 대기 및 자원 해제
* `디버그`, `스왑체인`, `디바이스`, `인스턴스` 파괴

[⬅ 메인으로 돌아가기](../README.md)


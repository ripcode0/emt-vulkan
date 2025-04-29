# Vulkan: Command Buffer

## 역할
`Command Buffer`는 GPU에 전달할 모든 작업(그리기, 복사, 상태 전환 등)을 기록하는 객체입니다.  
CPU가 명령을 쌓아놓고, GPU는 이를 실행합니다.

## 주요 명령 종류

### 1. 그리기 (Draw Commands)

- `vkCmdDraw`
- `vkCmdDrawIndexed`
- `vkCmdDrawIndirect`
- `vkCmdDrawIndexedIndirect`

> 정점 버퍼를 기반으로 실제 **그리기(렌더링)** 작업을 수행합니다.

### 2. 상태 전환 (State Changes)

- `vkCmdBindPipeline` : 파이프라인(Shader, Rasterizer 등) 바인딩
- `vkCmdBindDescriptorSets` : 리소스(Uniforms, Textures 등) 바인딩
- `vkCmdBindVertexBuffers` : 정점 버퍼 바인딩
- `vkCmdBindIndexBuffer` : 인덱스 버퍼 바인딩
- `vkCmdSetViewport`, `vkCmdSetScissor` : 뷰포트/시저 영역 설정

> 렌더링 파이프라인과 사용할 리소스들을 설정합니다.

### 3. 복사 (Copy Commands)

- `vkCmdCopyBuffer` : 버퍼 → 버퍼 복사
- `vkCmdCopyImage` : 이미지 → 이미지 복사
- `vkCmdCopyBufferToImage` : 버퍼 → 이미지 복사
- `vkCmdCopyImageToBuffer` : 이미지 → 버퍼 복사

> GPU 메모리 사이 데이터 전송을 수행합니다. (텍스처 업로드, 버퍼 데이터 이동 등)

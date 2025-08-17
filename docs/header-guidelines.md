# Goals
- 공개 API(Public headers)와 internal 구현(Private headers) 명확히 분리
- 순환 의존성 차단
- 변경 영향(재컴파일 범위) 최소화
- 모듈 단위로 확장 가능한 구조 유지
# Directory Structure
```
project/
  include/            # 공개 헤더 (Public API)
    <pkg>/
      foo.h
      bar.h
    common_types.h    # 공용 타입(필요 시)
  src/                # 구현 + 내부 전용 헤더
    <pkg>/
      foo.c
      foo_internal.h
      bar.c
      bar_internal.h
```
# Hierarchy & Dependency
```
[공용 타입 헤더]    (include/common_types.h 또는 <pkg>_types.h)
          ↑
[공용 함수 헤더]    (include/<pkg>/*.h)           ← Public API
          ↑
[내부 전용 헤더]    (src/include/<pkg>/*_internal.h)
          ↑
[구현 파일]        (src/<pkg>/*.c)
```
- `include` 방향은 항상 위로만 흐르게 유지
	- e.g. Public API에서 `*_internal.h` 포함 금지
- 공용 타입 헤더는 root에서 여러 모듈이 사용
# Include Rules
### `.c` 파일
1. 자기 공개 헤더를 가장 먼자 include
2. 필요 시 자기 내부 전용 헤더 (`*_internal.h`)
3. 필요한 다른 모듈의 공개 헤더
4. 표준/시스템 헤더 (`<...>`): 무거운 헤더는 가능하면 `.c`에서만
### Public Headers
1. 공개 헤더는 **공개 헤더 또는 표준 헤더만 포함**
	- `<stdio.h>`, `<time.h>` 같은 무거운/플랫폼 헤더는 가급적 .c에서만
2. **공개 헤더끼리 교차 include 금지**
3. 여러 모듈이 공유하는 타입은 공용 타입 헤더로 분리
### Private Headers
- `_internal.h` 네이밍
- 외부 타겟/패키지에 배포하지 않음
- `extern` 내부 전역 선언, 내부용 struct/헬퍼, `static inline` 유틸 등
# Forward Declarations
- 다른 모듈의 타입을 포인터로만 참조할 때는 `#include` 대신 전방 선언:
```
struct Foo; // 다른 모듈의 헤더
void use_foo(struct Foo *p); // 포인터/참조 OK
```
- 불필요한 의존/재컴파일을 줄이고 순환 참조 예방
# Self-Contained Rule
- 각 공개 헤더는 단독으로 include해도 컴파일 가능해야 함.
	- `.c`에서 자기 공개 헤더를 가장 먼저 include하여 검증
# Separate Build Path
- Public: `-I include`
- Private: 필요 시 `-I src/include` (외부 타겟에는 노출 금지)

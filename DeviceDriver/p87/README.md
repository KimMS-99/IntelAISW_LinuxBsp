# mknod()함수

디바이스 파일은 mknod라는 시스템 유틸리티를 통해 생성하거나 커널 옵션을 통해 자동으로 생성된다. 매우 드물지만 응용 프로그램이 디바이스 파일을 만들어야 할 때가 있다. 이때 쓰이는 함수가 유틸리티 명과 같은 mknod()다
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fnctl.h>
#include <unistd.h>

int mknod(const char *pathname, mode_t mode, dev_t dev);
```
- pathname : 작성할 디바이스 파일명의 문자열
- mode : 접근 허가 속성과 디바이스 타입
    -   디바이스 파일 종류
        - S_IFCHR : 문자 디바이스
        - S_IFBLK : 블록 디바이스
    - 접근 허가
        - S_IRWXU : 사용자는 읽기 쓰기 권하닝 있다.
        - S_IRWXG : 
# 디바이스 파일 생성 : mknod()함수

디바이스 파일은 `mknod`라는 시스템 유틸리티를 통해 생성하거나 커널 옵션을 통해 자동으로 생성된다. 매우 드물지만 응용 프로그램이 디바이스 파일을 만들어야 할 때가 있다. 이때 쓰이는 함수가 유틸리티 명과 같은 `mknod()`다
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
        - S_IRWXU : 사용자는 읽기 쓰기 권한이 있다.
        - S_IRWXG : 그룹은 읽기 쓰기 권한이 있다.

- dev : 실제로 만들어지는 디바이스의 주 번호와 부 번호의 값을 지정한다.

다음 명령을 보면 `mknod` 유틸리티를 통해 주 번호가 240이고, 부 번호가 1인 문자 디바이스 파일을 만들 수 있다.
```bash
[root@] # mknod /dev/test c 240 1
```
이와 동일한 처리를 하는 프로그램은 다음과 같다.
```c
mknod("dev/test", S_IRWXU|S_IRWXG|S_IFCHR, (240<<8) | 1);
```

# 에러 처리 : perror() 함수
저수준 파일 입출력 함수의 실행이 실패했을 때 참조되는 error 값은 정수형 변수이기 때문에 값 자체만으로는 그 의미를 쉽게 알 수 없다. 이때 에러의 내용을 알기 쉬운 문자열로 나타내는 함수이다.
```c
fd = open("/dev/ram", O_RDONLY);
if(fd < 0)
{
    perror("open");
}
```
이 예는 디바이스 파일을 열다가 실패했을 때의 일반적인 처리인데, 실패했을 때는 `perror()`에 지정한 문자열 "open"과 함께 실패 이유에 대한 error 번호를 해석하여 문자열로 표현한다. 디바이스 드라이버와 연관된 에러 번호의 의미는 커널 소스 디렉토리에서 include/asm/errno.h를 참고하면 된다.
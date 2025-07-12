# 모듈 사용법 및 설명

## ⦁ 실행 방법

```bash
ubuntu@ubuntu04:~/pi_bsp/drivers/p106$ make
# 모듈 파일을 실행하기 위한 커널 적재
pi@pi03:/mnt/nfs_ubuntu $ sudo insmod hello.ko
# 실행된 메시지 보기
pi@pi03:/mnt/nfs_ubuntu $ dmesg
# 모듈 제거
pi@pi03:/mnt/nfs_ubuntu $ rmmod hello
```

## ⦁ 모듈 유틸리티

디바이스 드라이버를 모듈로 만들어 커널에 적재하거나 제거하기 위해 다음과 같은 모듈 유틸리티를 사용할 수 있다.

- `insmod` : 모듈을 커널에 적재한다.
- `rmmod` : 커널에서 모듈을 제거한다.
- `lsmod` : 커널에 적재된 모듈 목록을 보여준다.
- `depmod` : 모듈간 의존성 정보를 생성한다.
- `modprobe` : 모듈을 커널에 적재하거나 제거한다.

### - insmod

**insmod**는 커널의 심볼 테이블 시스템을 통해 모듈로 작성한 커널 객체를 커널에 링크시키도록 도와주는 외부 유틸리티이다. 이 프로그램은 객체 형태의 디바이스 드라이버 모듈을 커널에 적재할 때 커널에 메모리 할당을 요구하고, 심볼 테이블을 참조하여 함수와 변수의 실제 변수를 객체 안에 할당한 뒤 이를 커널에 포함시킬 것을 요구한다.
```bash
[root@ ]# insmod [ -fkmpsxXv ][ -o module_name ] object_file [ symbol=value... ]
```
- 옵션
    -  -f: 현재 실행중인 커널과 모듈이 컴파일된 커널의 버전이 다르더라도 모듈을 적재함.
    - -k: 모듈에 auto-clean 플래그를 설정한다. 이 플래그는 kerneld에 의해 모듈이 일정 시간 동안(1분) 사용되지 않을 때 모듈을 삭제한다.
    - -m: 커널 패닉 상태일 때 로드맵을 출력해서 모듈을 디버그하기 쉽게 한다.
    - -o: 소스 객체 파일의 기본 이름에서 모듈을 디버그하기 쉽게 한다.
    - -p: 모듈이 커널에 성공적으로 적재될 수 있는지 검사한다.
    - -s: syslog에 출력한다.
    - -v: 실행되는 과정에서 발생한 메시지를 모두 출력한다.
    - -x: 심볼 내보내기 (export symbols)
    - -X: 심볼 내보내기 비활성화 (disable symbol export)

### - rmmod

커널에 등록된 모듈을 제거하는 명령이다. insmod와 달리 디바이스 드라이버 모듈명을 지정한다.

```bash
rmmod [-f] [-w] [-s] [-v] [modulename]
```

- 옵션
    - -v -verbose: 실행 과정에서 발생한 메시지를 모두 출력한다.
    - -f -force: 모듈이 제거될 수 없는 상황에서도 강제로 제거한다. 이때 커널의 안정성은 보장되지 않는다.
    - -w --waitL 모듈이 제거될 수 있는 상황이 될 때까지 기다렸다가 제거한다.
    - -s: syslog에 출력한다.

### - lsmod

커널에 등록된 모듈의 상태를 보여준다.
- 예시)
```bash
[root@ ]# lsmod
Module   Size     Usde By
Test     1344     0
Autofs   15328    0
parport_pc  20104   0
parport  40768    1     parport_pc
```
첫번째 열은 등록된 모듈명을 나타낸다. 두번째 열은 모듈이 커널에서 차지하고 있는 기억 공간의 크기를 나타낸다. 세번째 열은 사용중인지를 나타낸다. 네번째 열은 해당 모듈을 참조하고 있는 모듈명을 나타낸다. 

## ⦁ 모듈 초기화 함수와 제거 함수 지정
모듈은 컴파일 결과로 생성된 객체 파일이므로 모듈 유틸리티를 이용해 커널에 적재하고 링크하는 단계가 필요하다.

**module_init** 은 커널에 모듈을 적재할 때 커널이 호출하는 함수를 지정하는 매크로고, **module_exit** 는 커널에서 모듈을 제거할 때 호출하는 함수를 지정하는 매크로다.

## ⦁ 라이선스 등록
다음과 같은 형식으로 모듈의 라이선스를 표현해야 하는데
```c
MODULE_LICENSE("Daul BSD/GPL");
```
**MODULE_LICENSE** 로 사용할 수 있는 문자열과 그 의미는 아래 표와 같은데 특별히 기업 비밀을 보장해야 할 상황이 아니라면 "Dual BSD/GPL"을 사용하면 된다. 생략하거나 "Proprietary" 를 사용하면 커널 내부의 몇몇 API에 접근할 수 없지만, 크게 문제가 되지는 않는다.
- `MODULE_LICENSE`에 사용하는 문자열과 의미

| 라이선스 | 풀어쓰기
|:---:|:---:|
| GPL | GNU Public License v2 or later
| GPL v2 | GNU Public License v2
| GPL and additional rights | GNU Public License v2 rights and more
| Dual BSD/GPL | GNU Public License v2 or BSD license choice
| DUAL MPL/GPL | GNU Public License v2 or Mozilla license choice
| Proprieatry | Non free products
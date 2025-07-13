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

## ⦁ 라이선스 등록
다음과 같은 형식으로 모듈의 라이선스를 표현해야 하는데
```c
MODULE_LICENSE("Dual BSD/GPL");
```
**MODULE_LICENSE** 로 사용할 수 있는 문자열과 그 의미는 아래 표와 같은데 특별히 기업 비밀을 보장해야 할 상황이 아니라면 "Dual BSD/GPL"을 사용하면 된다. 생략하거나 "Proprietary" 를 사용하면 커널 내부의 몇몇 API에 접근할 수 없지만, 크게 문제가 되지는 않는다.
- `MODULE_LICENSE`에 사용하는 문자열과 의미

| 라이선스 | 풀어쓰기
|:---:|:---:|
| GPL | GNU Public License v2 or later
| GPL v2 | GNU Public License v2
| GPL and additional rights | GNU Public License v2 rights and more
| Dual BSD/GPL | GNU Public License v2 or BSD license choice
| Dual MPL/GPL | GNU Public License v2 or Mozilla license choice
| Proprieatry | Non free products

## ⦁ 모듈 초기화 함수와 제거 함수 지정
모듈은 컴파일 결과로 생성된 객체 파일이므로 모듈 유틸리티를 이용해 커널에 적재하고 링크하는 단계가 필요하다.

`module_init` 은 커널에 모듈을 적재할 때 커널이 호출하는 함수를 지정하는 매크로고, `module_exit` 는 커널에서 모듈을 제거할 때 호출하는 함수를 지정하는 매크로다.

- `module_init()`은 커널 모듈이 `insmod`를 통해 적재될 때 자동으로 호출되는 함수(초기화 함수)를 지정합니다.
- `module_exit()`은 모듈이 `rmmod`를 통해 제거될 때 자동으로 호출되는 함수(정리 함수)를 지정합니다.

예:
```c
static int __init my_init(void) {
    printk("Module loaded\n");
    return 0;
}

static void __exit my_exit(void) {
    printk("Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
```

## ⦁ 모듈 파라미터(Module Parameter)란?

디바이스 드라이버에는 고정된 내부 변수값을 사용하는 경우가 많은데, 보통 이런 변수값은 소스에서 값을 지정하여 사용한다. 그러나 필요한 경우 디바이스 드라이버가 실행되기 전에 시스템 상황에 따라 변경할 필요가 있다.

모듈 파라미터는 리눅스 커널 모듈을 커널에 적재(loading)할 때 외부에서 값을 전달해주는 기능이다. `insmod` 명령어를 통해 모듈의 특정 변수 값을 초기화하거나 동작 방식을 변경할 수 있어, 코드를 재컴파일하지 않고도 모듈의 동작을 유연하게 제어할 수 있다.

### - 왜 사용하는가?

- **유연성 및 편의성**: 디버그 메시지 출력 여부, 초기 설정값 등을 모듈 로드 시점에 쉽게 변경할 수 있다.
- **재사용성**: 동일한 모듈 코드를 파라미터만 다르게 설정하여 여러 다른 환경이나 목적에 맞게 사용할 수 있다.

### - 주요 매크로

- `module_param(name, type, perm)`: 단일 변수를 파라미터로 지정한다.
- `module_param_array(name, type, num, perm)`: 배열을 파라미터로 지정한다.

### - 매크로 인자

- `name`: 파라미터로 사용할 모듈 내의 전역 변수 이름
- `type`: 변수의 데이터 타입 (예: `int`, `charp`, `bool`)
- `perm`: `/sys/module/<모듈이름>/parameters/` 디렉토리에 생성될 파일의 권한 (예: `S_IRUGO`, `0644`)

### - 사용 예제 ([`ledkey_param.c`](../../p122_param/ledkey_param.c))

```c
// 파라미터로 사용할 변수 선언
static int onevalue = 1;
static char *twostring = NULL;

// 변수를 모듈 파라미터로 지정
module_param(onevalue, int, 0);
module_param(twostring, charp, 0);

// 모듈 초기화 함수에서 파라미터 사용
static int hello_init(void)
{
    // 커널 로그에 파라미터 값 출력
    printk(KERN_INFO "onevalue = %d, twostring = %s\n", onevalue, twostring);
    
    // 파라미터 값을 이용해 LED 초기 상태 설정
    gpioLedSet(onevalue);
    
    return 0;
}
```

### - 모듈 적재 시 파라미터 전달

`insmod` 명령어를 사용하여 모듈을 적재할 때 다음과 같이 파라미터 값을 전달할 수 있다.

```bash
# insmod ledkey_param.ko onevalue=15 twostring="hello world"
```

위 명령어를 실행하면 `ledkey_param` 모듈이 적재되면서 `onevalue` 변수는 `15`로, `twostring` 변수는 `"hello world"`로 초기화한다. 만약 파라미터를 전달하지 않으면 코드에 정의된 기본값이 사용된다.


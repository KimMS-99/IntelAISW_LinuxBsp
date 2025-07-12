
# 모듈 파라미터(Module Parameter)란?

디바이스 드라이버에는 고정된 내부 변수값을 사용하는 경우가 많은데, 보통 이런 변수값은 소스에서 값을 지정하여 사용한다. 그러나 필요한 경우 디바이스 드라이버가 실행되기 전에 시스템 상황에 따라 변경할 필요가 있다.

모듈 파라미터는 리눅스 커널 모듈을 커널에 적재(loading)할 때 외부에서 값을 전달해주는 기능이다. `insmod` 명령어를 통해 모듈의 특정 변수 값을 초기화하거나 동작 방식을 변경할 수 있어, 코드를 재컴파일하지 않고도 모듈의 동작을 유연하게 제어할 수 있다.

## 왜 사용하는가?

- **유연성 및 편의성**: 디버그 메시지 출력 여부, 초기 설정값 등을 모듈 로드 시점에 쉽게 변경할 수 있다.
- **재사용성**: 동일한 모듈 코드를 파라미터만 다르게 설정하여 여러 다른 환경이나 목적에 맞게 사용할 수 있다.

### 주요 매크로

- `module_param(name, type, perm)`: 단일 변수를 파라미터로 지정한다.
- `module_param_array(name, type, num, perm)`: 배열을 파라미터로 지정한다.

### 매크로 인자

- `name`: 파라미터로 사용할 모듈 내의 전역 변수 이름
- `type`: 변수의 데이터 타입 (예: `int`, `charp`, `bool`)
- `perm`: `/sys/module/<모듈이름>/parameters/` 디렉토리에 생성될 파일의 권한 (예: `S_IRUGO`, `0644`)

## 사용 예제 (`ledkey_param.c`)

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

## 모듈 적재 시 파라미터 전달

`insmod` 명령어를 사용하여 모듈을 적재할 때 다음과 같이 파라미터 값을 전달할 수 있다.

```bash
# insmod ledkey_param.ko onevalue=15 twostring="hello world"
```

위 명령어를 실행하면 `ledkey_param` 모듈이 적재되면서 `onevalue` 변수는 `15`로, `twostring` 변수는 `"hello world"`로 초기화한다. 만약 파라미터를 전달하지 않으면 코드에 정의된 기본값이 사용된다.


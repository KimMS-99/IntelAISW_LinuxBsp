# p106_ledkey 커널 모듈 분석

## `ledkey.c`

8개의 LED와 8개의 Key(버튼)를 GPIO로 제어하는 커널 모듈 코드.

### 주요 기능

- **GPIO 초기화**:
  - `gpioLedInit()`: 8개의 LED용 GPIO 핀을 요청하고, 출력으로 설정합니다.
  - `gpioKeyInit()`: 8개의 Key(버튼)용 GPIO 핀을 요청하고, 입력으로 설정합니다.
- **GPIO 제어**:
  - `gpioLedSet(long val)`: 8비트 정수값을 받아 각 비트에 해당하는 LED를 켜거나 끕니다.
  - `gpioKeyGet()`: 8개 Key의 상태를 읽어 8비트 정수값으로 반환합니다.
- **모듈 동작**:
  - 모듈이 로드될 때 (`module_init` -> `hello_init`):
    1. LED와 Key를 위한 GPIO 핀을 초기화합니다.
    2. Key(버튼)의 상태를 한 번 읽어서 그 값으로 LED 상태를 설정합니다.
  - 모듈이 언로드될 때 (`module_exit` -> `hello_exit`):
    1. 마지막으로 Key 상태를 읽어 LED를 업데이트합니다.
    2. 사용했던 모든 GPIO 핀의 점유를 해제합니다.

### 특징

- 이 모듈은 모듈이 로드되거나 언로드될 때만 Key의 상태를 확인하여 LED를 제어합니다. 모듈이 로드된 상태에서 Key를 조작해도 LED 상태가 실시간으로 변경되지는 않습니다.
- 소스 코드에 정의된 GPIO 핀 번호(`518`, `528` 등)는 특정 임베디드 보드에 맞춰진 값입니다.

## `Makefile`

`ledkey.c` 코드를 컴파일하여 커널 모듈(`ledkey.ko`)을 생성하기 위한 파일입니다.

### 주요 설정

- **크로스 컴파일**: `ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-` 설정을 통해 ARM 아키텍처용으로 크로스 컴파일하도록 설정되어 있습니다. 이는 x86 PC 환경에서 라즈베리파이와 같은 ARM 보드용 코드를 컴파일할 때 필요합니다.
- **커널 소스 경로**: `KDIR` 변수에 타겟 보드의 커널 소스 트리 경로를 지정해야 합니다.
- **빌드 및 배포**:
  - `make` (또는 `make default`): 모듈을 컴파일하고, 생성된 `ledkey.ko` 파일을 NFS 공유 폴더(`/srv/nfs_ubuntu`)로 복사하여 타겟 보드에서 쉽게 접근할 수 있도록 합니다.
  - `make clean`: 컴파일 과정에서 생성된 모든 파일을 삭제합니다.

## 요약

`p106_ledkey`는 ARM 타겟 보드용으로 개발된 간단한 GPIO 제어 커널 모듈입니다. 모듈이 로드/언로드 될 때 버튼 상태를 읽어 LED에 반영하는 기능을 하며, `Makefile`은 이 모듈을 크로스 컴파일하고 타겟 보드로 배포하는 과정을 자동화합니다.

### 주요 함수 설명

- **`gpio_request(unsigned int gpio, const char *label)`**:
  - **설명**: 커널에 특정 GPIO 핀을 사용하겠다고 요청하는 함수입니다. 다른 드라이버가 해당 핀을 사용 중이면 오류를 반환하여 충돌을 방지합니다.
  - **`ledkey.c`에서의 사용**: `gpioLedInit`과 `gpioKeyInit` 함수에서 각각 LED와 Key에 연결된 GPIO 핀들을 사용하기 위해 호출됩니다. `label` 인자로는 "led0", "key0"과 같이 각 핀의 용도를 식별할 수 있는 문자열을 전달합니다.

- **`gpio_direction_input(unsigned int gpio)`**:
  - **설명**: `gpio_request`로 선점한 GPIO 핀의 방향을 '입력'으로 설정합니다. 이 함수 호출 이후 해당 핀으로 들어오는 전기적 신호(HIGH/LOW)를 읽을 수 있습니다.
  - **`ledkey.c`에서의 사용**: `gpioKeyInit` 함수에서 Key(버튼)에 연결된 각 GPIO 핀을 입력으로 설정하여 버튼의 눌림 상태를 감지할 수 있도록 준비합니다.

- **`gpio_direction_output(unsigned int gpio, int value)`**:
  - **설명**: GPIO 핀의 방향을 '출력'으로 설정하고, 초기 출력값을 지정합니다.
  - **`ledkey.c`에서의 사용**: `gpioLedInit` 함수에서 LED에 연결된 각 GPIO 핀을 출력으로 설정하고, 초기 상태를 `LED_OFF`(0)으로 지정하여 모든 LED가 꺼진 상태에서 시작하도록 합니다.

- **`gpio_set_value(unsigned int gpio, int value)`**:
  - **설명**: 출력으로 설정된 GPIO 핀의 상태를 변경하는 함수입니다. `value`에 1(HIGH) 또는 0(LOW)을 전달하여 해당 핀에 연결된 장치(여기서는 LED)를 켜거나 끕니다.
  - **`ledkey.c`에서의 사용**: `gpioLedSet` 함수 내에서 사용됩니다. 8비트 `val` 값을 한 비트씩 검사하여 `(val >> i) & 0x01` 연산을 통해 각 LED를 켜거나 끌지를 결정하고, `gpio_set_value`를 호출하여 실제 LED의 상태를 변경합니다.

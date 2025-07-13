# Linux GPIO 인터페이스 주요 함수 설명

cat /sys/kernel/debug/gpio

## ⦁ `gpio_request()`

### - 선언
```c
int gpio_request(unsigned int gpio, const char *label);
```

### - 설명
`gpio_request()` 함수는 GPIO 핀을 사용하기 전, 해당 핀에 대한 소유권을 커널에 요청하는 가장 첫 단계로, 이는 커널의 GPIO 관리 시스템에 해당 핀을 특정 드라이버가 사용할 것임을 등록하는 과정이다. 이 과정을 통해 동일한 GPIO 핀에 여러 드라이버가 동시에 접근하여 발생하는 충돌(race condition)을 방지할 수 있다.

- **자원 관리**: 커널은 어떤 드라이버가 어떤 GPIO 핀을 사용 중인지 추적한다. 만약 요청한 핀이 이미 다른 드라이버에 의해 선점(request)된 상태라면, `gpio_request()`는 `-EBUSY`와 같은 에러 코드를 반환하여 실패를 알린다.
- **디버깅**: `label` 인자로 전달된 문자열은 디버깅 목적으로 사용된다. 예를 들어, `/sys/kernel/debug/gpio` 파일을 통해 현재 시스템의 모든 GPIO 핀 상태를 확인할 때, 이 `label`이 표시되어 어떤 드라이버가 해당 핀을 사용하고 있는지 쉽게 식별할 수 있다.

### - 매개변수
- `unsigned int gpio`: 시스템에서 사용하는 GPIO 핀의 고유 번호로, 이 번호는 하드웨어(SoC, 보드)에 따라 결정된다.
- `const char *label`: 해당 GPIO 핀의 용도를 설명하는 짧은 문자열이다. (예: "led_power", "button_reset").

### - 반환값
- **성공 시**: `0`을 반환.
- **실패 시**: 음수 값의 에러 코드를 반환. (예: `-EBUSY` - 이미 사용 중, `-EINVAL` - 잘못된 GPIO 번호)

### - 사용 후 처리
`gpio_request()`를 통해 성공적으로 선점한 핀은 드라이버가 더 이상 사용하지 않을 때(예: 모듈 언로드 시) 반드시 `gpio_free()` 함수를 호출하여 소유권을 해제해야 한다.

---

## ⦁ `gpio_direction_output()` / `gpio_direction_input()`

GPIO 핀을 선점한 후에는 해당 핀을 입력(Input)으로 사용할지, 출력(Output)으로 사용할지 방향을 설정해야 한다.

### - `gpio_direction_output()`
#### 선언
```c
int gpio_direction_output(unsigned int gpio, int value);
```
#### 설명
GPIO 핀의 방향을 **출력**으로 설정한다. 즉, 해당 핀을 통해 외부 장치(LED, 모터 등)로 신호(HIGH/LOW)를 내보낼 수 있게 된다.
두 번째 인자인 `value`는 핀이 출력으로 설정되는 순간 가지게 될 **초기 출력값**을 지정함. 이는 핀 방향이 설정되는 과정에서 의도치 않은 값(glitch)이 출력되어 연결된 장치가 오작동하는 것을 방지하는 중요한 역할을 한다.

#### 매개변수
- `unsigned int gpio`: 방향을 설정할 GPIO 핀 번호.
- `int value`: 초기 출력값. `0`은 LOW, `1` (또는 0이 아닌 값)은 HIGH를 의미.

### - `gpio_direction_input()`
#### 선언
```c
int gpio_direction_input(unsigned int gpio);
```
#### 설명
이 함수는 GPIO 핀의 방향을 **입력**으로 설정한다. 핀이 입력으로 설정되면, 외부 소스(버튼, 센서 등)로부터 들어오는 전기적 신호의 상태(HIGH/LOW)를 읽을 수 있게 된다. 내부적으로는 보통 하이 임피던스(high-impedance) 상태로 만들어져 회로에 영향을 주지 않고 전압 레벨을 감지한다.

#### 매개변수
- `unsigned int gpio`: 방향을 설정할 GPIO 핀 번호.

### - 반환값 (두 함수 공통)
- **성공 시**: `0`을 반환.
- **실패 시**: 음수 값의 에러 코드를 반환.

---

## ⦁ `gpio_set_value()`

### - 선언
```c
void gpio_set_value(unsigned int gpio, int value);
```

### - 설명
`gpio_set_value()` 함수는 **출력**으로 설정된 GPIO 핀의 논리 상태를 변경한다. 이 함수를 사용하여 LED를 켜거나 끄는 등 외부 장치를 직접 제어할 수 있다.

- **성능**: 이 함수는 속도가 매우 중요할 때 사용되도록 최적화되어 있다. 하지만, 컨텍스트에 따라 슬립(sleep)이 발생할 수 있으므로, 인터럽트 핸들러나 다른 atomic 컨텍스트에서는 사용하면 안된다. 이러한 환경에서는 `gpio_set_value_cansleep()`의 non-sleeping 버전을 사용해야 한다.
- **전제 조건**: 이 함수를 호출하기 전에 반드시 해당 `gpio` 핀이 `gpio_direction_output()`을 통해 출력으로 설정되어 있어야 한다.

### - 매개변수
- `unsigned int gpio`: 상태를 변경할 GPIO 핀 번호.
- `int value`: 설정할 논리 값. `0`은 LOW, `1` (또는 0이 아닌 값)은 HIGH를 의미.

### - 반환값
- 이 함수는 `void` 타입으로, 반환값이 없음. 실패 여부를 확인하지 않으므로 호출 전에 핀 설정이 올바른지 확인하는 것이 중요.

---

## ⦁ 사용 예시 (요약)

```c
// 1. GPIO 핀 요청
ret = gpio_request(LED_GPIO, "my_led");
if (ret) {
    printk("Failed to request GPIO %d
", LED_GPIO);
    return ret;
}

// 2. 방향을 출력으로 설정 (초기값은 LOW)
ret = gpio_direction_output(LED_GPIO, 0);
if (ret) {
    printk("Failed to set direction for GPIO %d
", LED_GPIO);
    gpio_free(LED_GPIO); // 실패 시 자원 해제
    return ret;
}

// 3. 값 설정 (LED 켜기)
gpio_set_value(LED_GPIO, 1);

// ... 사용 ...

// 4. 자원 해제
gpio_free(LED_GPIO);
```

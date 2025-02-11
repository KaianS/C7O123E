#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define LED_PIN 12  // Utilizado para o experimento com o LED RGB na GPIO 12

// Constantes para configuração do PWM
const uint16_t WRAP_PERIOD = 40000;   // Valor de wrap para o PWM (aprox. 50Hz com 20ms)
const float PWM_DIVISER = 64.0f;        // Divisor de clock para obter a frequência desejada

// Constantes de posição (em microsegundos)
#define MIN_PULSE 500    // 0° (500 µs)
#define MID_PULSE 1470   // 90° (1470 µs)
#define MAX_PULSE 2400   // 180° (2400 µs)
#define PERIOD_PWM 20000 // Período do PWM: 20ms

// Configuração inicial do PWM para o servo e o LED
void pwm_setup() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);

    uint slice_servo = pwm_gpio_to_slice_num(SERVO_PIN);
    uint slice_led = pwm_gpio_to_slice_num(LED_PIN);

    pwm_set_clkdiv(slice_servo, PWM_DIVISER);
    pwm_set_wrap(slice_servo, WRAP_PERIOD);

    pwm_set_clkdiv(slice_led, PWM_DIVISER);
    pwm_set_wrap(slice_led, WRAP_PERIOD);

    pwm_set_gpio_level(SERVO_PIN, 0);
    pwm_set_gpio_level(LED_PIN, 0);

    pwm_set_enabled(slice_servo, true);
    pwm_set_enabled(slice_led, true);
}

// Função que ajusta o ângulo do servomotor com base na largura do pulso (em µs)
void move_servo(uint16_t pulse_width) {
    // Converte o pulso para o nível PWM desejado:
    // duty = (pulse_width / 20000) * WRAP_PERIOD
    uint16_t duty_cycle = (pulse_width * WRAP_PERIOD) / PERIOD_PWM;
    pwm_set_gpio_level(SERVO_PIN, duty_cycle);
}

// Função que ajusta o brilho do LED (GPIO 12) conforme o pulso utilizado
void level_led(uint16_t pulse_width) {
    // Mapeia o pulso (500 a 2400 µs) para um duty cycle entre 0 e WRAP_PERIOD
    uint16_t duty_cycle = (pulse_width * WRAP_PERIOD) / MAX_PULSE;
    pwm_set_gpio_level(LED_PIN, duty_cycle);
}

int main() {
    stdio_init_all();
    pwm_setup();

    sleep_ms(2000); // Aguarda 2 segundos para estabilização

    // Movimentos fixos conforme os requisitos da atividade:
    printf("Posicionando 180°\n");
    move_servo(MAX_PULSE);
    level_led(MAX_PULSE);
    sleep_ms(5000);

    printf("Posicionando 90°\n");
    move_servo(MID_PULSE);
    level_led(MID_PULSE);
    sleep_ms(5000);

    printf("Posicionando 0°\n");
    move_servo(MIN_PULSE);
    level_led(MIN_PULSE);
    sleep_ms(5000);

    // Movimento suave de 0° → 180° com incremento de 5 µs
    printf("Movimento suave de 0° a 180°\n");
    for (uint16_t pulse = MIN_PULSE; pulse <= MAX_PULSE; pulse += 5) {
        move_servo(pulse);
        level_led(pulse);
        sleep_ms(10);
    }

    // Movimento suave de 180° → 0° com decremento de 5 µs
    printf("Movimento suave de 180° a 0°\n");
    for (uint16_t pulse = MAX_PULSE; pulse >= MIN_PULSE; pulse -= 5) {
        move_servo(pulse);
        level_led(pulse);
        sleep_ms(10);
    }

    // Loop infinito com os movimentos suaves repetidos
    while (true) {
        for (uint16_t pulse = MIN_PULSE; pulse <= MAX_PULSE; pulse += 5) {
            move_servo(pulse);
            level_led(pulse);
            sleep_ms(10);
        }
        for (uint16_t pulse = MAX_PULSE; pulse >= MIN_PULSE; pulse -= 5) {
            move_servo(pulse);
            level_led(pulse);
            sleep_ms(10);
        }
    }
}

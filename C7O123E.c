#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definição dos pinos
const int SERVO_PIN = 22;
const int RED_PIN = 12;
const int GREEN_PIN = 13;
const int BLUE_PIN = 14;

// Constantes para o servo motor
const float PWM_FREQUENCY = 50.0;       // Frequência do PWM em Hz
const float PWM_PERIOD_US = 20000.0;    // Período do PWM em microssegundos
const float PWM_WRAP = 4095.0;          // Valor máximo do contador PWM

// Funções de configuração
void pwm_configurar_servo() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Configurar a frequência do PWM para aproximadamente 50 Hz
    float clock = 125000000.0;  // Clock do sistema em Hz
    float divider = clock / PWM_FREQUENCY / PWM_WRAP;
    pwm_set_clkdiv(slice_num, divider);
    pwm_set_wrap(slice_num, PWM_WRAP);  // Define o valor máximo do contador
    pwm_set_enabled(slice_num, true);
}

void pwm_configurar_led(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice_num, 1.0);  // Sem divisão de clock
    pwm_set_wrap(slice_num, 255);    // Define o valor máximo do contador
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);  // Inicializa com nível baixo
    pwm_set_enabled(slice_num, true);
}

// Funções de controle
void servo_set_pulse_width(float pulse_width_us) {
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    float duty_cycle = pulse_width_us / PWM_PERIOD_US;
    uint16_t level = (uint16_t)(duty_cycle * PWM_WRAP);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
}

void led_rgb_set_color(uint8_t red, uint8_t green, uint8_t blue) {
    uint slice_num_red = pwm_gpio_to_slice_num(RED_PIN);
    uint slice_num_green = pwm_gpio_to_slice_num(GREEN_PIN);
    uint slice_num_blue = pwm_gpio_to_slice_num(BLUE_PIN);

    pwm_set_chan_level(slice_num_red, PWM_CHAN_A, red);
    pwm_set_chan_level(slice_num_green, PWM_CHAN_A, green);
    pwm_set_chan_level(slice_num_blue, PWM_CHAN_A, blue);
}

int main() {
    stdio_init_all();

    // Configurar PWM para o servo motor
    pwm_configurar_servo();

    // Configurar PWM para os pinos do LED RGB
    pwm_configurar_led(RED_PIN);
    pwm_configurar_led(GREEN_PIN);
    pwm_configurar_led(BLUE_PIN);

    // 1) GPIO 22, frequência de PWM para aproximadamente 50 Hz (já configurado em pwm_configurar_servo)

    // 2) Defina o ciclo ativo do módulo PWM para 2400 us (180 graus)
    servo_set_pulse_width(2400);
    led_rgb_set_color(255, 0, 0);  // Vermelho
    sleep_ms(5000);

    // 3) Defina o ciclo ativo do módulo PWM para 1470 us (90 graus)
    servo_set_pulse_width(1470);
    led_rgb_set_color(0, 255, 0);  // Verde
    sleep_ms(5000);

    // 4) Defina o ciclo ativo do módulo PWM para 500 us (0 graus)
    servo_set_pulse_width(500);
    led_rgb_set_color(0, 0, 255);  // Azul
    sleep_ms(5000);

    // 5) Movimentação periódica suave
    float pulse_width = 500.0;
    float increment = 5.0;
    while (true) {
        servo_set_pulse_width(pulse_width);

        // Mapear a largura do pulso para cores no LED RGB
        uint8_t red, green, blue;
        if (pulse_width < 1470) {
            red = 0;
            green = (uint8_t)((pulse_width - 500) / (1470 - 500) * 255);
            blue = 255 - green;
        } else {
            red = (uint8_t)((pulse_width - 1470) / (2400 - 1470) * 255);
            blue = 0;
            green = 255 - red;
        }
        led_rgb_set_color(red, green, blue);

        pulse_width += increment;
        if (pulse_width > 2400) {
            pulse_width = 2400;
            increment = -5.0;
        } else if (pulse_width < 500) {
            pulse_width = 500;
            increment = 5.0;
        }
        sleep_ms(10);
    }

    return 0;
}

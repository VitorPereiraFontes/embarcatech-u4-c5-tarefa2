#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Define o pino do botão A
#define A_BUTTON_PIN 5

// Define quais pinos GPIO correspondem a cada cor do LED RGB 
#define RED_LED_PIN 13
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12

// Define uma máscara de bits para a inicializaçao e configuração dos valores das GPIO's
const uint32_t GPIOS_MASK = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN) | (1 << A_BUTTON_PIN);
const uint32_t GPIOS_VALUES_MASK = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN) | (0 << A_BUTTON_PIN); // 1 - Saída | 0 - Entrada

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

int main(){
    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializando as GPIO's do LED RGB como saídas e com o nível lógico baixo e a GPIO do botão A como entrada com um resistor de pull-up
    gpio_init_mask(GPIOS_MASK); // Utiliza máscara de bits para inicializar as GPIO's
    gpio_set_dir_masked(GPIOS_MASK,GPIOS_VALUES_MASK); // Utiliza máscaras de bits para definir a direção de cada GPIO.
    gpio_pull_up(A_BUTTON_PIN); // Define que a GPIO do botão A terá um resistor de pull-up

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

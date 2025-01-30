#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Define quais pinos GPIO correspondem a cada cor do LED RGB 
#define RED_LED_PIN 13
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12

// Define uma máscara de bits para a inicializaçao e configuração do LED RGB
const uint32_t LEDS_GPIO_MASK = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN);

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

int main(){
    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializando as GPIO's do LED RGB como saídas e com o nível lógico baixo
    gpio_init_mask(LEDS_GPIO_MASK); // Utiliza máscara de bits para inicializar os 3 pinos na mesma chamada de função
    gpio_set_dir_masked(LEDS_GPIO_MASK,LEDS_GPIO_MASK); // Utiliza máscaras de bits para definir a direção de cada GPIO. Como todas serão saídas, apenas utilizei a mesma variável no segundo parâmetro.

    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);
    // For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

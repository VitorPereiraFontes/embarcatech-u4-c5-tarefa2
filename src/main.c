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
const uint32_t gpios_mask = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN) | (1 << A_BUTTON_PIN);
const uint32_t gpios_values_mask = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN) | (0 << A_BUTTON_PIN); // 1 - Saída | 0 - Entrada
uint32_t gpio_leds_mask = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN); // Mascara de bits para os pinos GPIO do LED RGB

// Inicializa uma variável de controle que armazena o estado do event de callback para o alarme
bool event_running = false;

// Define o intervalo de mudança entre os leds milissegundos
uint led_change_interval = 3000; // 3 segundos

// Define o intervalo de disparo do temporizador em milissegundos
uint alarm_interval = 100;

// Definindo uma estrutura de dados para o controle do evento
typedef struct{
    uint32_t led_gpios_values_mask; // Mascara de bits que informa qual cor do LED deverá ser habilitada
    uint counter; // Contabiliza quantas mudanças o LED já sofreu
}alarm_event_data_t;

// Inicializando a estrutura com o estado inicial
alarm_event_data_t initial_event_data = {
    .led_gpios_values_mask = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN), // Liga todos os LED's
    .counter = 0
};

int main(){
    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializando as GPIO's do LED RGB como saídas e com o nível lógico baixo e a GPIO do botão A como entrada com um resistor de pull-up
    gpio_init_mask(gpios_mask); // Utiliza máscara de bits para inicializar as GPIO's
    gpio_set_dir_masked(gpios_mask,gpios_values_mask); // Utiliza máscaras de bits para definir a direção de cada GPIO.
    gpio_pull_up(A_BUTTON_PIN); // Define que a GPIO do botão A terá um resistor de pull-up

    while (true) {
        // Verifica se o botão foi pressionado e se o evento não foi disparado ainda
        if (gpio_get(A_BUTTON_PIN) == 0 && !event_running){
            sleep_ms(50); // Delay de 50 ms para prevenir debouncing do botão

            // Verifica se o botão ainda está pressionado
            if (gpio_get(A_BUTTON_PIN) == 0){
                event_running = true ; // Altera o valor da variável de controle, inibindo o botão de ativar o evento

                printf("Pressionado!Agendando o temporizador para %d milissegundos no futuro...\n",alarm_interval);
            }
        }

        sleep_ms(80); // Delay para não sobrecarregar o microprocessador
    }
}

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

// Função de callback do temporizador.
int64_t alarm_callback(alarm_id_t id, void *event_data_uncasted) {    
    alarm_event_data_t *event_data = (alarm_event_data_t *)event_data_uncasted;  // Cast para o tipo correto

    uint next_alarm_id; // Variável para armazenar o id do próximo alarme

    printf("%d  -  Evento disparado! Alterando o estado do LED...\n",event_data->counter);

    // Altera o estado dos LED's com os dados passados por referência
    gpio_put_masked(gpio_leds_mask,event_data->led_gpios_values_mask);

    switch (event_data->counter){
        case 0:
            // Atualiza o estado dos LED's para a proxima alteração
            event_data->led_gpios_values_mask = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 0 << BLUE_LED_PIN); // Apaga o LED azul
            event_data->counter++; // Incrementa o contador de alterações do LED

            // Inicializando o temporizador de disparo único para que a proxima mudança no led seja realizada
            next_alarm_id = add_alarm_in_ms(led_change_interval, alarm_callback, event_data, false);

            if(next_alarm_id < 0){
                printf("Houve um erro ao tentar agendar o alarme, tente novamente!\n");
            }
        break;

        case 1:
            // Atualiza o estado dos LED's para a proxima alteração
            event_data->led_gpios_values_mask = (0 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 0 << BLUE_LED_PIN); // Apaga os LED's azul e vermelho
            event_data->counter++; // Incrementa o contador de alterações do LED

            // Inicializando o temporizador de disparo único para que a proxima mudança no led seja realizada
            next_alarm_id = add_alarm_in_ms(led_change_interval, alarm_callback, event_data, false);

            if(next_alarm_id < 0){
                printf("Houve um erro ao tentar agendar o alarme, tente novamente!\n");
            }
        break;
    
        case 2:
            event_data->led_gpios_values_mask = (0 << RED_LED_PIN) | (0 << GREEN_LED_PIN) | (0 << BLUE_LED_PIN); // Apaga todos os LED's
            event_data->counter++; //Reseta o contador de mudanças do LED

            // Inicializando o temporizador de disparo único para que a proxima mudança no led seja realizada
            next_alarm_id = add_alarm_in_ms(led_change_interval, alarm_callback, event_data, false); // Retorna para o estado inicial

            if(next_alarm_id < 0){
                printf("Houve um erro ao tentar agendar o alarme, tente novamente!\n");
            }
        break;

        default:
            event_data->led_gpios_values_mask = (1 << RED_LED_PIN) | (1 << GREEN_LED_PIN) | ( 1 << BLUE_LED_PIN); // Liga todos os LED's
            event_data->counter = 0; // Reseta o contador
            event_running = false; // Libera o envento ser disparado novamente pelo botão

            printf("Fim do evento! Para dispará-lo novamente, pressione o botão.\n");
        break;
    }

    return 0; // Retorna 0 para indicar que o temporizador não deverá ser re-agendado
}

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

                // Inicializando o temporizador de disparo único
                uint alarm_id = add_alarm_in_ms(alarm_interval, alarm_callback, &initial_event_data, false);

                if(alarm_id < 0){
                    printf("Houve um erro ao tentar agendar o alarme, tente novamente!\n");
                }
            }
        }

        sleep_ms(80); // Delay para não sobrecarregar o microprocessador
    }
}

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define RED_LED 13
#define YELLOW_LED 11
#define GREEN_LED 12
#define BUTTON 5

bool button_pressed = false;
int led_state = 0;
uint32_t last_button_press_time = 0;

// Função de callback para desligar os LEDs sequencialmente
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    if (led_state == 1) {
        gpio_put(GREEN_LED, 0);  // Desliga o LED verde
        led_state = 2;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    } else if (led_state == 2) {
        gpio_put(YELLOW_LED, 0); // Desliga o LED amarelo
        led_state = 3;
        add_alarm_in_ms(3000, turn_off_callback, NULL, false);
    } else if (led_state == 3) {
        gpio_put(RED_LED, 0);    // Desliga o LED vermelho
        button_pressed = false;  // Permite novo acionamento do botão
        led_state = 0;           // Reseta o estado para permitir novo ciclo
    }
    return 0;
}

// Função de callback para ligar os LEDs
int64_t turn_on_callback(alarm_id_t id, void *user_data) {
    gpio_put(RED_LED, 1);
    gpio_put(YELLOW_LED, 1);
    gpio_put(GREEN_LED, 1);  // Liga todos os LEDs
    led_state = 1;
    add_alarm_in_ms(3000, turn_off_callback, NULL, false); // Inicia a sequência de desligamento
    return 0;
}

// Função de callback para o botão com debounce
void button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if ((current_time - last_button_press_time) > 200) {  // Debounce de 200ms
        last_button_press_time = current_time;

        if (!button_pressed && led_state == 0) { // Permite reiniciar após desligar todos os LEDs
            button_pressed = true;
            add_alarm_in_ms(10, turn_on_callback, NULL, false);  // Inicia a sequência de LEDs
        }
    }
}

int main() {
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_init(YELLOW_LED);
    gpio_set_dir(YELLOW_LED, GPIO_OUT);
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    // Configuração do botão
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);
    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Inicializa LEDs desligados
    gpio_put(RED_LED, 0);
    gpio_put(YELLOW_LED, 0);
    gpio_put(GREEN_LED, 0);

    while (true) {
        printf("Aguardando botão\n");
        sleep_ms(1000);
    }
}

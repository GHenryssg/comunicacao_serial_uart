#include <stdlib.h>
#include <stdio.h>
#include <pico/multicore.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "matriz.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

// Estrutura para armazenar a configuração do hardware
typedef struct {
    i2c_inst_t* i2c_port;
    uint8_t sda_pin, scl_pin, display_addr;
    uint8_t red_led, green_led, blue_led;
    uint8_t btn_a, btn_b;
} HardwareConfig;

// Estrutura para armazenar o estado do sistema
typedef struct {
    char selected_char;
    bool led_matrix_state[NUM_PIXELS];
    bool green_led_on, blue_led_on;
    uint32_t last_press_a, last_press_b;
} SystemState;

// Definições globais
static const HardwareConfig config = {
    .i2c_port = i2c1,
    .sda_pin = 14, .scl_pin = 15, .display_addr = 0x3C,
    .red_led = 13, .green_led = 11, .blue_led = 12,
    .btn_a = 5, .btn_b = 6
};

static SystemState state = {0};
const uint8_t LED_BRIGHTNESS[] = {191, 0, 191}; // Intensidade RGB

// Prototipação de funções
void init_hardware(void);
void configure_display(ssd1306_t* display);
void process_button(uint gpio, uint32_t events);
void refresh_display(ssd1306_t* display, bool invert);
void input_task(void);

// Inicialização do hardware
void init_hardware(void) {
    i2c_init(config.i2c_port, 400000);
    gpio_set_function(config.sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(config.scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(config.sda_pin);
    gpio_pull_up(config.scl_pin);

    uint8_t leds[] = {config.red_led, config.green_led, config.blue_led};
    for (uint8_t i = 0; i < 3; i++) {
        gpio_init(leds[i]);
        gpio_set_dir(leds[i], GPIO_OUT);
        gpio_put(leds[i], 0);
    }

    uint8_t buttons[] = {config.btn_a, config.btn_b};
    for (uint8_t i = 0; i < 2; i++) {
        gpio_init(buttons[i]);
        gpio_set_dir(buttons[i], GPIO_IN);
        gpio_pull_up(buttons[i]);
    }
}

void configure_display(ssd1306_t* display) {
    ssd1306_init(display, WIDTH, HEIGHT, false, config.display_addr, config.i2c_port);
    ssd1306_config(display);
    ssd1306_fill(display, false);
    ssd1306_send_data(display);
}

void process_button(uint gpio, uint32_t events) {
    const uint32_t DEBOUNCE = 200000;
    uint32_t now = to_us_since_boot(get_absolute_time());

    if ((events & GPIO_IRQ_EDGE_FALL)) {
        if (gpio == config.btn_a && (now - state.last_press_a > DEBOUNCE)) {
            state.last_press_a = now;
            state.green_led_on = !state.green_led_on;
            gpio_put(config.green_led, state.green_led_on);
        } else if (gpio == config.btn_b && (now - state.last_press_b > DEBOUNCE)) {
            state.last_press_b = now;
            state.blue_led_on = !state.blue_led_on;
            gpio_put(config.blue_led, state.blue_led_on);
        }
    }
}

void refresh_display(ssd1306_t* display, bool invert) {
    ssd1306_fill(display, invert);
    ssd1306_draw_string(display, "Caractere", 12, 10);
    ssd1306_draw_string(display, "Escolhido", 12, 20);
    
    char ch[2] = {state.selected_char, '\0'};
    ssd1306_draw_string(display, ch, 99, 15);
    ssd1306_line(display, 98, 25, 109, 25, 1);

    char status[32];
    snprintf(status, sizeof(status), "Led Verde  %s", state.green_led_on ? "on " : "off");
    ssd1306_draw_string(display, status, 12, 35);

    snprintf(status, sizeof(status), "Led Azul   %s", state.blue_led_on ? "on " : "off");
    ssd1306_draw_string(display, status, 12, 45);

    ssd1306_send_data(display);
}

void input_task(void) {
    while (true) {
        int ch = getchar();
        if (ch != EOF) {
            state.selected_char = (char)ch;
        }
        sleep_ms(500);
    }
}

int main() {
    stdio_init_all();
    init_hardware();

    ssd1306_t display;
    configure_display(&display);

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, 0, offset, PIN_MATRIX);

    gpio_set_irq_enabled_with_callback(config.btn_a, GPIO_IRQ_EDGE_FALL, true, &process_button);
    gpio_set_irq_enabled_with_callback(config.btn_b, GPIO_IRQ_EDGE_FALL, true, &process_button);

    multicore_launch_core1(input_task);
    bool invert_display = true;

    while (true) {
        refresh_display(&display, invert_display);
        invert_display = !invert_display;
        set_character_matrix(state.selected_char, state.led_matrix_state);
        set_matrix_led(LED_BRIGHTNESS[0], LED_BRIGHTNESS[1], LED_BRIGHTNESS[2], state.led_matrix_state);
        sleep_ms(1000);
    }
    return 0;
}

#include "pico/stdlib.h"
#include "tusb.h"
#include "usb_descriptors.h"

#define PIN_15_B 15
#define PIN_26_X 26
#define PIN_27_Y 27
#define PIN_28_A 28

uint32_t bt_a, bt_b, bt_x, bt_y;
void hid_task();
void pins_init();
void gpio_callback(uint gpio, uint32_t events);

int main(void) {
    tusb_init();
    pins_init();


    while (true) {
        tud_task();
        hid_task();
    }

    return 0;
}


void pins_init() {
    gpio_init(PIN_28_A);
    gpio_set_dir(PIN_28_A, GPIO_IN);
    gpio_pull_up(PIN_28_A);

    gpio_init(PIN_15_B);
    gpio_set_dir(PIN_15_B, GPIO_IN);
    gpio_pull_up(PIN_15_B);

    gpio_init(PIN_26_X);
    gpio_set_dir(PIN_26_X, GPIO_IN);
    gpio_pull_up(PIN_26_X);

    gpio_init(PIN_27_Y);
    gpio_set_dir(PIN_27_Y, GPIO_IN);
    gpio_pull_up(PIN_27_Y);

    gpio_set_irq_enabled_with_callback(PIN_28_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(PIN_15_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(PIN_26_X, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(PIN_27_Y, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    irq_set_enabled(IO_IRQ_BANK0, true);
}

void gpio_callback(uint gpio, uint32_t events) {
    printf("gpio callback gpio=%hhu events=%u\n", gpio, events);
    bool activate = events == GPIO_IRQ_EDGE_FALL ? true : false;

    switch (gpio) {
        case PIN_28_A:
            bt_a = activate ? GAMEPAD_BUTTON_A : 0;
            break;
        case PIN_15_B:
            bt_b = activate ? GAMEPAD_BUTTON_B : 0;
            break;
        case PIN_26_X:
            bt_x = activate ? GAMEPAD_BUTTON_X : 0;
            break;
        case PIN_27_Y:
            bt_y = activate ? GAMEPAD_BUTTON_Y : 0;
            break;
        default:
    }
}

static void send_hid_report()
{
    if (!tud_hid_ready()) {
        return;
    }

    static bool had_buttons = false;
    uint32_t buttons = bt_a | bt_b | bt_x | bt_y;
    hid_gamepad_report_t report = {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = buttons
    };

    if (buttons) {
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        had_buttons = true;
    }
    else {
        if (had_buttons) {
            tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        }
        had_buttons = false;
    }
}

void hid_task()
{
    if (tud_suspended()) {
        tud_remote_wakeup();
    }

    send_hid_report();
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void) instance;
    (void) len;
}


void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;
    (void) report_id;
}

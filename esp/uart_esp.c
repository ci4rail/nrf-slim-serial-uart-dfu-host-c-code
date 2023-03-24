
#include "eloc_nrf_wrap.h"
#include "uart_drv.h"
#include "logging.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#ifndef CONFIG_EASYLOCATE_SIMULATION

#define UART_BUF_SIZE 256

int uart_drv_open(uart_drv_t *p_uart)
{
    easylocate_uart_config_t *c = &p_uart->conf;
    uart_config_t uart_config = {
        .baud_rate = c->baud,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

    if (uart_driver_install(c->uart_num, UART_BUF_SIZE, 0, 0, NULL, intr_alloc_flags) != ESP_OK) {
        return 1;
    }
    if (uart_param_config(c->uart_num, &uart_config) != ESP_OK) {
        return 1;
    }
    if (uart_set_pin(c->uart_num, c->tx, c->rx, -1, -1) != ESP_OK) {
        return 1;
    }

    return 0;
}

int uart_drv_close(uart_drv_t *p_uart)
{
    return 0;
}

int uart_drv_send(uart_drv_t *p_uart, const uint8_t *pData, uint32_t nSize)
{
    uart_port_t uart_num = p_uart->conf.uart_num;
    int length;

    length = uart_write_bytes(uart_num, pData, nSize);
    if (length != nSize) {
        logger_error("Cannot write TTY port!");

        return 1;
    }
    uart_wait_tx_done(uart_num, pdMS_TO_TICKS(1000));

    return 0;
}

int uart_drv_receive(uart_drv_t *p_uart, uint8_t *pData, uint32_t nSize, uint32_t *pSize)
{
    uart_port_t uart_num = p_uart->conf.uart_num;
    int32_t length;

    length = uart_read_bytes(uart_num, pData, nSize, pdMS_TO_TICKS(p_uart->conf.rx_timeout_ms));
    // logger_info_1("uart_drv_receive len=%d to=%d", length, p_uart->conf.rx_timeout_ms / portTICK_RATE_MS);

    if (length < 0) {
        logger_error("Cannot read TTY port!");
        return 1;
    }
    *pSize = length;

    return 0;
}
#endif  // CONFIG_EASYLOCATE_SIMULATION


#include "eloc_nrf_wrap.h"
#include "uart_drv.h"
#include "logging.h"
#include "driver/gpio.h"
#include "driver/uart.h"

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

    //??    ESP_ERROR_CHECK(uart_driver_install(c->uart_num, UART_BUF_SIZE, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(c->uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(c->uart_num, c->tx, c->rx, -1, -1));

    return err_code;
}

int uart_drv_close(uart_drv_t *p_uart)
{
    int err_code = 0;
    int fd = p_uart->tty_fd;

    if (fd >= 0) {
        if (close(fd)) {
            logger_error("Cannot close TTY port!");

            err_code = 1;
        }
    } else
        err_code = 1;

    return err_code;
}

int uart_drv_send(uart_drv_t *p_uart, const uint8_t *pData, uint32_t nSize)
{
    int err_code = 0;
    int32_t length;

    length = write(p_uart->tty_fd, pData, nSize);
    if (length != nSize) {
        logger_error("Cannot write TTY port!");

        err_code = 1;
    } else {
        if (tcdrain(p_uart->tty_fd)) {
            logger_error("Cannot drain TTY TX buffer!");

            err_code = 1;
        }
    }

    return err_code;
}

int uart_drv_receive(uart_drv_t *p_uart, uint8_t *pData, uint32_t nSize, uint32_t *pSize)
{
    int err_code = 0;
    int32_t length;

    length = read(p_uart->tty_fd, pData, nSize);
    if (length < 0) {
        logger_error("Cannot read TTY port!");

        err_code = 1;
    } else
        *pSize = length;

    return err_code;
}

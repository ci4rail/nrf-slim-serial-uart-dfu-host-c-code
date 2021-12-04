#include "eloc_nrf_wrap.h"
#include "uart_drv.h"

typedef struct {
    uart_drv_t *p_uart;

    uint8_t *p_img_dat;   //!< Image DAT pointer.
    uint32_t n_dat_size;  //!< Image DAT size.
    uint8_t *p_img_bin;   //!< Image BIN pointer.
    uint32_t n_bin_size;  //!< Image BIN size.
} nrf_dfu_img_param_t;

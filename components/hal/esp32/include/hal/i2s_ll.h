// Copyright 2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*******************************************************************************
 * NOTICE
 * The hal is not public api, don't use in application code.
 * See readme.md in hal/include/hal/readme.md
 ******************************************************************************/

// The LL layer for ESP32 I2S register operations

#pragma once

#include <stdbool.h>
#include "soc/i2s_periph.h"
#include "hal/i2s_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Get I2S hardware instance with giving i2s num
#define I2S_LL_GET_HW(num) (((num) == 0) ? (&I2S0) : (((num) == 1) ? (&I2S1) : NULL))

#define I2S_LL_AD_BCK_FACTOR           (2)
#define I2S_LL_PDM_BCK_FACTOR          (64)
#define I2S_LL_BASE_CLK                (2 * APB_CLK_FREQ)

#define I2S_LL_MCLK_DIVIDER_BIT_WIDTH  (6)
#define I2S_LL_MCLK_DIVIDER_MAX        ((1 << I2S_LL_MCLK_DIVIDER_BIT_WIDTH) - 1)

#define I2S_LL_EVENT_TX_EOF       (1 << 12)
#define I2S_LL_BCK_MAX_PRESCALE   (64)

/* I2S clock configuration structure */
typedef struct {
    uint16_t mclk_div; // I2S module clock devider, Fmclk = Fsclk /(mclk_div+b/a)
    uint16_t a;
    uint16_t b;        // The decimal part of module clock devider, the decimal is: b/a
} i2s_ll_clk_cal_t;

/**
 * @brief Enable DMA descriptor owner check
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en whether to enable owner check
 */
static inline void i2s_ll_dma_enable_owner_check(i2s_dev_t *hw, bool en)
{
    hw->lc_conf.check_owner = en;
}

/**
 * @brief Enable DMA descriptor write back
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en whether to enable write back
 */
static inline void i2s_ll_dma_enable_auto_write_back(i2s_dev_t *hw, bool en)
{
    hw->lc_conf.out_auto_wrback = en;
}

/**
 * @brief I2S DMA generate EOF event on data in FIFO poped out
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en True to enable, False to disable
 */
static inline void i2s_ll_dma_enable_eof_on_fifo_empty(i2s_dev_t *hw, bool en)
{
    hw->lc_conf.out_eof_mode = en;
}

/**
 * @brief I2S module general init, enable I2S clock.
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_enable_clock(i2s_dev_t *hw)
{
    if (hw->clkm_conf.clk_en == 0) {
        hw->clkm_conf.clk_en = 1;
        hw->conf2.val = 0;
    }
}

/**
 * @brief I2S tx msb right enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable tx msb right
 */
static inline void i2s_ll_tx_enable_msb_right(i2s_dev_t *hw, bool enable)
{
    hw->conf.tx_msb_right = enable;
}

/**
 * @brief I2S rx msb right enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable rx msb right
 */
static inline void i2s_ll_rx_enable_msb_right(i2s_dev_t *hw, bool enable)
{
    hw->conf.rx_msb_right = enable;
}

/**
 * @brief I2S tx right channel first
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable send right channel first
 */
static inline void i2s_ll_tx_enable_right_first(i2s_dev_t *hw, bool enable)
{
    hw->conf.tx_right_first = enable;
}

/**
 * @brief I2S rx right channel first
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable receive right channel first
 */
static inline void i2s_ll_rx_enable_right_first(i2s_dev_t *hw, bool enable)
{
    hw->conf.rx_right_first = enable;
}

/**
 * @brief I2S tx fifo module force enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable tx fifo module
 */
static inline void i2s_ll_tx_force_enable_fifo_mod(i2s_dev_t *hw, bool enable)
{
    hw->fifo_conf.tx_fifo_mod_force_en = enable;
}

/**
 * @brief I2S rx fifo module force enable
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable rx fifo module
 */
static inline void i2s_ll_rx_force_enable_fifo_mod(i2s_dev_t *hw, bool enable)
{
    hw->fifo_conf.rx_fifo_mod_force_en = enable;
}
/**
 * @brief Enable I2S TX slave mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param slave_en Set true to enable slave mode
 */
static inline void i2s_ll_tx_set_slave_mod(i2s_dev_t *hw, bool slave_en)
{
    hw->conf.tx_slave_mod = slave_en;
}

/**
 * @brief Enable I2S RX slave mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param slave_en Set true to enable slave mode
 */
static inline void i2s_ll_rx_set_slave_mod(i2s_dev_t *hw, bool slave_en)
{
    hw->conf.rx_slave_mod = slave_en;
}

/**
 * @brief Reset I2S TX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_reset(i2s_dev_t *hw)
{
    hw->conf.tx_reset = 1;
    hw->conf.tx_reset = 0;
}

/**
 * @brief Reset I2S RX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_reset(i2s_dev_t *hw)
{
    hw->conf.rx_reset = 1;
    hw->conf.rx_reset = 0;
}

/**
 * @brief Reset I2S TX FIFO
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_reset_fifo(i2s_dev_t *hw)
{
    hw->conf.tx_fifo_reset = 1;
    hw->conf.tx_fifo_reset = 0;
}

/**
 * @brief Reset I2S RX FIFO
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_reset_fifo(i2s_dev_t *hw)
{
    hw->conf.rx_fifo_reset = 1;
    hw->conf.rx_fifo_reset = 0;
}

/**
 * @brief Set TX source clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param src I2S source clock
 */
static inline void i2s_ll_tx_clk_set_src(i2s_dev_t *hw, i2s_clock_src_t src)
{
    //0: disable APLL clock, I2S module will using PLL_D2_CLK(160M) as source clock
    //1: Enable APLL clock, I2S module will using APLL as source clock
    hw->clkm_conf.clka_en = (src == I2S_CLK_APLL) ? 1 : 0;
}

/**
 * @brief Set RX source clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param src I2S source clock
 */
static inline void i2s_ll_rx_clk_set_src(i2s_dev_t *hw, i2s_clock_src_t src)
{
    //0: disable APLL clock, I2S module will using PLL_D2_CLK(160M) as source clock
    //1: Enable APLL clock, I2S module will using APLL as source clock
    hw->clkm_conf.clka_en = (src == I2S_CLK_APLL) ? 1 : 0;
}

/**
 * @brief Set I2S tx bck div num
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set tx bck div num
 */
static inline void i2s_ll_tx_set_bck_div_num(i2s_dev_t *hw, uint32_t val)
{
    hw->sample_rate_conf.tx_bck_div_num = val;
}

/**
 * @brief Configure I2S TX clock devider
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param set Pointer to I2S clock devider configuration paramater
 */
static inline void i2s_ll_tx_set_clk(i2s_dev_t *hw, i2s_ll_clk_cal_t *set)
{
    hw->clkm_conf.clkm_div_num = set->mclk_div;
    hw->clkm_conf.clkm_div_b = set->b;
    hw->clkm_conf.clkm_div_a = set->a;
}

/**
 * @brief Set I2S rx bck div num
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set rx bck div num
 */
static inline void i2s_ll_rx_set_bck_div_num(i2s_dev_t *hw, uint32_t val)
{
    hw->sample_rate_conf.rx_bck_div_num = val;
}

/**
 * @brief Configure I2S RX clock devider
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param set Pointer to I2S clock devider configuration paramater
 */
static inline void i2s_ll_rx_set_clk(i2s_dev_t *hw, i2s_ll_clk_cal_t *set)
{
    hw->clkm_conf.clkm_div_num = set->mclk_div;
    hw->clkm_conf.clkm_div_b = set->b;
    hw->clkm_conf.clkm_div_a = set->a;
}

/**
 * @brief Enable interrupt by mask
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mask Interrupt event mask
 * @param en true to enable, false to disable
 */
static inline void i2s_ll_enable_intr(i2s_dev_t *hw, uint32_t mask, bool en)
{
    if (en) {
        hw->int_ena.val |= mask;
    } else {
        hw->int_ena.val &= ~mask;
    }
}

/**
 * @brief Enable TX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_enable_intr(i2s_dev_t *hw)
{
    hw->int_ena.out_eof = 1;
    hw->int_ena.out_dscr_err = 1;
}

/**
 * @brief Disable TX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_disable_intr(i2s_dev_t *hw)
{
    hw->int_ena.out_eof = 0;
    hw->int_ena.out_dscr_err = 0;
}

/**
 * @brief Enable RX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_enable_intr(i2s_dev_t *hw)
{
    hw->int_ena.in_suc_eof = 1;
    hw->int_ena.in_dscr_err = 1;
}

/**
 * @brief Disable RX interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_disable_intr(i2s_dev_t *hw)
{
    hw->int_ena.in_suc_eof = 0;
    hw->int_ena.in_dscr_err = 0;
}

/**
 * @brief Get interrupt status register address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @return interrupt status register address
 */
static inline volatile void *i2s_ll_get_intr_status_reg(i2s_dev_t *hw)
{
    return &hw->int_st;
}

/**
 * @brief Get I2S interrupt status
 *
 * @param hw Peripheral I2S hardware instance address.
 * @return
 *        - module interrupt status
 */
static inline uint32_t i2s_ll_get_intr_status(i2s_dev_t *hw)
{
    return hw->int_st.val;
}

/**
 * @brief Clear I2S interrupt status
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param clr_mask Interrupt mask to clear interrupt status
 */
static inline void i2s_ll_clear_intr_status(i2s_dev_t *hw, uint32_t clr_mask)
{
    hw->int_clr.val = clr_mask;
}

/**
 * @brief Reset dma out
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_reset_dma(i2s_dev_t *hw)
{
    hw->lc_conf.out_rst = 1;
    hw->lc_conf.out_rst = 0;
}

/**
 * @brief Reset dma in
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_reset_dma(i2s_dev_t *hw)
{
    hw->lc_conf.in_rst = 1;
    hw->lc_conf.in_rst = 0;
}

/**
 * @brief Start out link
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_start_out_link(i2s_dev_t *hw)
{
    hw->out_link.start = 1;
}

/**
 * @brief Set I2S out link address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set out link address
 */
static inline void i2s_ll_set_out_link_addr(i2s_dev_t *hw, uint32_t val)
{
    hw->out_link.addr = val;
}

/**
 * @brief Start TX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_start(i2s_dev_t *hw)
{
    hw->conf.tx_start = 1;
}

/**
 * @brief Start RX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_start(i2s_dev_t *hw)
{
    hw->conf.rx_start = 1;
}

/**
 * @brief Configure TX DMA descriptor address and start TX DMA
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param link_addr DMA descriptor link address.
 */
static inline void i2s_ll_tx_start_link(i2s_dev_t *hw, uint32_t link_addr)
{
    i2s_ll_set_out_link_addr(hw, link_addr);
    i2s_ll_start_out_link(hw);
}

/**
 * @brief Configure RX DMA descriptor address and start RX DMA
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param link_addr DMA descriptor link address.
 */
static inline void i2s_ll_rx_start_link(i2s_dev_t *hw, uint32_t link_addr)
{
    hw->in_link.addr = link_addr;
    hw->in_link.start = 1;
}

/**
 * @brief Stop TX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_stop(i2s_dev_t *hw)
{
    hw->conf.tx_start = 0;
}

/**
 * @brief Stop RX module
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_stop(i2s_dev_t *hw)
{
    hw->conf.rx_start = 0;
}

/**
 * @brief Stop out link
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_tx_stop_link(i2s_dev_t *hw)
{
    hw->out_link.stop = 1;
}

/**
 * @brief Stop in link
 *
 * @param hw Peripheral I2S hardware instance address.
 */
static inline void i2s_ll_rx_stop_link(i2s_dev_t *hw)
{
    hw->in_link.stop = 1;
}

/**
 * @brief Get I2S out eof descriptor address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param eof_addr Pointer to accept out eof des address
 */
static inline void i2s_ll_tx_get_eof_des_addr(i2s_dev_t *hw, uint32_t *eof_addr)
{
    *eof_addr = hw->out_eof_des_addr;
}

/**
 * @brief Get I2S in eof descriptor address
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param eof_addr Pointer to accept in eof des address
 */
static inline void i2s_ll_rx_get_eof_des_addr(i2s_dev_t *hw, uint32_t *eof_addr)
{
    *eof_addr = hw->in_eof_des_addr;
}

/**
 * @brief Configure the received length to trigger in_suc_eof interrupt
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param eof_num the byte length to trigger in_suc_eof interrupt
 */
static inline void i2s_ll_rx_set_eof_num(i2s_dev_t *hw, int eof_num)
{
    // On ESP32, the eof_num count in words.
    hw->rx_eof_num = eof_num / 4;
}

/**
 * @brief Set I2S tx bits mod
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set tx bits mod
 */
static inline void i2s_ll_tx_set_bits_mod(i2s_dev_t *hw, uint32_t val)
{
    hw->sample_rate_conf.tx_bits_mod = val;
}

/**
 * @brief Congfigure TX chan bit and audio data bit, on ESP32, sample_bit should equals to data_bit
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param chan_bit The chan bit width
 * @param data_bit The audio data bit width
 */
static inline void i2s_ll_tx_set_sample_bit(i2s_dev_t *hw, uint8_t chan_bit, int data_bit)
{
    hw->fifo_conf.tx_fifo_mod = (chan_bit <= I2S_BITS_PER_SAMPLE_16BIT ? 0 : 2);
    hw->sample_rate_conf.tx_bits_mod = data_bit;
}

/**
 * @brief Congfigure RX chan bit and audio data bit, on ESP32, sample_bit should equals to data_bit
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param chan_bit The chan bit width
 * @param data_bit The audio data bit width
 */
static inline void i2s_ll_rx_set_sample_bit(i2s_dev_t *hw, uint8_t chan_bit, int data_bit)
{
    hw->fifo_conf.rx_fifo_mod = (chan_bit <= I2S_BITS_PER_SAMPLE_16BIT ? 0 : 2);
    hw->sample_rate_conf.rx_bits_mod = data_bit;
}

/**
 * @brief Set whether to continue I2S signal on bus when TX FIFO is empty
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param en whether to stop when tx fifo is empty
 */
static inline void i2s_ll_tx_stop_on_fifo_empty(i2s_dev_t *hw, bool en)
{
    hw->conf1.tx_stop_en = en;
}

/**
 * @brief Set whether to bypass the internal PCM module
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param bypass whether to bypass the PCM module
 */
static inline void i2s_ll_tx_bypass_pcm(i2s_dev_t *hw, bool bypass)
{
    hw->conf1.tx_pcm_bypass = bypass;
}

/**
 * @brief Enable I2S DMA
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param ena Set true to enable DMA
 */
static inline void i2s_ll_enable_dma(i2s_dev_t *hw, bool ena)
{
    hw->fifo_conf.dscr_en = ena;
}

/**
 * @brief Configure TX WS signal width
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param width WS width in BCK cycle
 */
static inline void i2s_ll_tx_set_ws_width(i2s_dev_t *hw, int width)
{
    hw->conf.tx_short_sync = width == 1 ? 1 : 0;
}

/**
 * @brief Configure RX WS signal width
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param width WS width in BCK cycle
 */
static inline void i2s_ll_rx_set_ws_width(i2s_dev_t *hw, int width)
{
    hw->conf.rx_short_sync = width == 1 ? 1 : 0;
}

/**
 * @brief Enable TX MSB shift, the data will be launch at the first BCK clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param msb_shift_enable Set true to enable MSB shift
 */
static inline void i2s_ll_tx_enable_msb_shift(i2s_dev_t *hw, bool msb_shift_enable)
{
    hw->conf.tx_msb_shift = msb_shift_enable;
}

/**
 * @brief Enable RX MSB shift, the data will be launch at the first BCK clock
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param msb_shift_enable Set true to enable MSB shift
 */
static inline void i2s_ll_rx_enable_msb_shift(i2s_dev_t *hw, bool msb_shift_enable)
{
    hw->conf.rx_msb_shift = msb_shift_enable;
}

/**
 * @brief Set I2S tx chan mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param val value to set tx chan mode
 */
static inline void i2s_ll_tx_set_chan_mod(i2s_dev_t *hw, uint32_t val)
{
    hw->conf_chan.tx_chan_mod = val;
}

/**
 * @brief Enable TX mono mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mono_ena Set true to enable mono mde.
 */
static inline void i2s_ll_tx_enable_mono_mode(i2s_dev_t *hw, bool mono_ena)
{
    int data_bit = hw->sample_rate_conf.tx_bits_mod;
    hw->fifo_conf.tx_fifo_mod = data_bit <= I2S_BITS_PER_SAMPLE_16BIT ? mono_ena : 2 + mono_ena;
    hw->conf_chan.tx_chan_mod = mono_ena;
}

/**
 * @brief Enable RX mono mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param mono_ena Set true to enable mono mde.
 */
static inline void i2s_ll_rx_enable_mono_mode(i2s_dev_t *hw, bool mono_ena)
{
    int data_bit = hw->sample_rate_conf.rx_bits_mod;
    hw->fifo_conf.rx_fifo_mod = data_bit <= I2S_BITS_PER_SAMPLE_16BIT ? mono_ena : 2 + mono_ena;
    hw->conf_chan.rx_chan_mod = mono_ena;
}

/**
 * @brief Enable I2S loopback mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param loopback_en Set true to enable loopback mode.
 */
static inline void i2s_ll_enable_loop_back(i2s_dev_t *hw, bool loopback_en)
{
    hw->conf.sig_loopback = loopback_en;
}



/******************************I2S PDM Configurations*************************************/
/**
 * @brief Configure RX PDM downsample
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param dsr PDM downsample configuration paramater
 */
static inline void i2s_ll_rx_set_pdm_dsr(i2s_dev_t *hw, i2s_pdm_dsr_t dsr)
{
    hw->pdm_conf.rx_sinc_dsr_16_en = dsr;
}

/**
 * @brief Get RX PDM downsample configuration
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param dsr Pointer to accept PDM downsample configuration
 */
static inline void i2s_ll_rx_get_pdm_dsr(i2s_dev_t *hw, i2s_pdm_dsr_t *dsr)
{
    *dsr = hw->pdm_conf.rx_sinc_dsr_16_en;
}

/**
 * @brief Enable I2S TX PDM mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param pdm_ena Set true to enable TX PDM mode
 */
static inline void i2s_ll_tx_enable_pdm(i2s_dev_t *hw, bool pdm_ena)
{
    hw->pdm_conf.tx_pdm_en = pdm_ena;
    hw->pdm_conf.pcm2pdm_conv_en = pdm_ena;
}

/**
 * @brief Enable I2S RX PDM mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param pdm_ena Set true to enable RX PDM mode
 */
static inline void i2s_ll_rx_enable_pdm(i2s_dev_t *hw, bool pdm_ena)
{
    hw->pdm_conf.rx_pdm_en = pdm_ena;
    hw->pdm_conf.pdm2pcm_conv_en = pdm_ena;
}

/**
 * @brief Set I2S TX PDM prescale
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param prescale I2S TX PDM prescale
 */
static inline void i2s_ll_tx_set_pdm_prescale(i2s_dev_t *hw, bool prescale)
{
    hw->pdm_conf.tx_prescale = prescale;
}

/**
 * @brief Set I2S TX PDM high pass filter scaling
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param sig_scale I2S TX PDM signal scaling before transmit to the filter
 */
static inline void i2s_ll_tx_set_pdm_hp_scale(i2s_dev_t *hw, i2s_pdm_sig_scale_t sig_scale)
{
    hw->pdm_conf.tx_hp_in_shift = sig_scale;
}

/**
 * @brief Set I2S TX PDM low pass filter scaling
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param sig_scale I2S TX PDM signal scaling before transmit to the filter
 */
static inline void i2s_ll_tx_set_pdm_lp_scale(i2s_dev_t *hw, i2s_pdm_sig_scale_t sig_scale)
{
    hw->pdm_conf.tx_lp_in_shift = sig_scale;
}

/**
 * @brief Set I2S TX PDM sinc filter scaling
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param sig_scale I2S TX PDM signal scaling before transmit to the filter
 */
static inline void i2s_ll_tx_set_pdm_sinc_scale(i2s_dev_t *hw, i2s_pdm_sig_scale_t sig_scale)
{
    hw->pdm_conf.tx_sinc_in_shift = sig_scale;
}

/**
 * @brief Set I2S TX PDM sigma-delta filter scaling
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param sig_scale I2S TX PDM signal scaling before transmit to the filter
 */
static inline void i2s_ll_tx_set_pdm_sd_scale(i2s_dev_t *hw, i2s_pdm_sig_scale_t sig_scale)
{
    hw->pdm_conf.tx_sigmadelta_in_shift = sig_scale;
}

/**
 * @brief Configure I2S TX PDM sample rate
 *        Fpdm = 64*Fpcm*fp/fs
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param fp The fp value of TX PDM filter module group0.
 * @param fs The fs value of TX PDM filter module group0.
 */
static inline void i2s_ll_tx_set_pdm_fpfs(i2s_dev_t *hw, uint32_t fp, uint32_t fs)
{
    hw->pdm_freq_conf.tx_pdm_fp = fp;
    hw->pdm_freq_conf.tx_pdm_fs = fs;
    hw->pdm_conf.tx_sinc_osr2 = fp / fs;
}

/**
 * @brief Get I2S TX PDM fp configuration paramater
 *
 * @param hw Peripheral I2S hardware instance address.
 * @return
 *        - fp configuration paramater
 */
static inline uint32_t i2s_ll_tx_get_pdm_fp(i2s_dev_t *hw)
{
    return hw->pdm_freq_conf.tx_pdm_fp;
}

/**
 * @brief Get I2S TX PDM fs configuration paramater
 *
 * @param hw Peripheral I2S hardware instance address.
 * @return
 *        - fs configuration paramater
 */
static inline uint32_t i2s_ll_tx_get_pdm_fs(i2s_dev_t *hw)
{
    return hw->pdm_freq_conf.tx_pdm_fs;
}




/****************************I2S ADC/DAC Configurations***********************************/
/**
 * @brief Enable I2S LCD mode
 * @note  Have to enable LCD mode to use build in ADC/DAC
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable LCD mode.
 */
static inline void i2s_ll_enable_lcd(i2s_dev_t *hw, bool enable)
{
    hw->conf2.lcd_en = enable;
}

/**
 * @brief Enable I2S camera mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable camera mode.
 */
static inline void i2s_ll_enable_camera(i2s_dev_t *hw, bool enable)
{
    hw->conf2.camera_en = enable;
}

/**
 * @brief Enable I2S build in ADC mode
 *
 * @param hw Peripheral I2S hardware instance address.
 * @param enable Set true to enable build in ADC
 */
static inline void i2s_ll_enable_builtin_adc(i2s_dev_t *hw, bool enable)
{
    hw->conf2.lcd_en = enable;
    hw->conf2.camera_en = 0;
    hw->conf.rx_right_first = 0;
    hw->conf.rx_msb_shift = 0;
    hw->conf.rx_mono = 0;
    hw->conf.rx_short_sync = 0;
    hw->fifo_conf.rx_fifo_mod = enable;
    hw->conf_chan.rx_chan_mod = enable;
}

/**
 * @brief Enable I2S build in DAC mode
 *
 * @param hw Peripheral I2S hardware instance address.
 *  * @param enable Set true to enable build in DAC
 */
static inline void i2s_ll_enable_builtin_dac(i2s_dev_t *hw, bool enable)
{
    hw->conf2.lcd_en = enable;
    hw->conf2.camera_en = 0;
    hw->conf.tx_right_first = enable;
    hw->conf.tx_msb_shift = 0;
    hw->conf.tx_short_sync = 0;
}

#ifdef __cplusplus
}
#endif

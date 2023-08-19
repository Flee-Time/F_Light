#include "modules/cc1101/cc1101.h"
#include "modules/spi/f_spi.h"
#include <assert.h>
#include <string.h>

/*
 *
 * These codes were taken and adapted from the flipper zero cc1101 driver.
 * Todo : Add code to initialize the cc1101 chip.
 *
*/

extern SPI_HandleTypeDef hspi1;

// Deprecated -- Note to self : find a diffrent way to switch gpio pins from mode to mode.

/*
void GD0_Input()
{
    GPIOB->MODER &= ~(GPIO_MODER_MODER0);
}

void GD0_Output()
{
    GPIOB->MODER |= GPIO_MODER_MODER0_0;
}

void GD0_Analog()
{
    GPIOB->MODER |= GPIO_MODER_MODER0;
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
}
*/

CC1101Status cc1101_strobe(uint8_t strobe) 
{
    uint8_t tx[1] = {strobe};
    CC1101Status rx[1] = {0};

    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
        ;
    spi_trx(hspi1, tx, (uint8_t*)rx, 1, CC1101_TIMEOUT);

    assert(rx[0].CHIP_RDYn == 0);
    return rx[0];
}

CC1101Status cc1101_write_reg(uint8_t reg, uint8_t data) 
{
    uint8_t tx[2] = {reg, data};
    CC1101Status rx[2] = {0};

    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
        ;
    spi_trx(hspi1, tx, (uint8_t*)rx, 2, CC1101_TIMEOUT);

    assert((rx[0].CHIP_RDYn | rx[1].CHIP_RDYn) == 0);
    return rx[1];
}

CC1101Status cc1101_read_reg(uint8_t reg, uint8_t* data) 
{
    assert(sizeof(CC1101Status) == 1);
    uint8_t tx[2] = {reg | CC1101_READ, 0};
    CC1101Status rx[2] = {0};

    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
        ;
    spi_trx(hspi1, tx, (uint8_t*)rx, 2, CC1101_TIMEOUT);

    assert((rx[0].CHIP_RDYn) == 0);
    *data = *(uint8_t*)&rx[1];
    return rx[0];
}

uint32_t cc1101_set_frequency(uint32_t value) 
{
    uint64_t real_value = (uint64_t)value * CC1101_FDIV / CC1101_QUARTZ;

    // Sanity check
    assert((real_value & CC1101_FMASK) == real_value);

    cc1101_write_reg(CC1101_FREQ2, (real_value >> 16) & 0xFF);
    cc1101_write_reg(CC1101_FREQ1, (real_value >> 8) & 0xFF);
    cc1101_write_reg(CC1101_FREQ0, (real_value >> 0) & 0xFF);

    uint64_t real_frequency = real_value * CC1101_QUARTZ / CC1101_FDIV;

    return (uint32_t)real_frequency;
}

uint32_t cc1101_set_intermediate_frequency(uint32_t value) 
{
    uint64_t real_value = value * CC1101_IFDIV / CC1101_QUARTZ;
    assert((real_value & 0xFF) == real_value);

    cc1101_write_reg(CC1101_FSCTRL0, (real_value >> 0) & 0xFF);

    uint64_t real_frequency = real_value * CC1101_QUARTZ / CC1101_IFDIV;

    return (uint32_t)real_frequency;
}

void cc1101_set_pa_table(const uint8_t value[8]) 
{
    uint8_t tx[9] = {CC1101_PATABLE | CC1101_BURST}; //-V1009
    CC1101Status rx[9] = {0};

    memcpy(&tx[1], &value[0], 8);

    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
        ;
    spi_trx(hspi1, tx, (uint8_t*)rx, sizeof(rx), CC1101_TIMEOUT);

    assert((rx[0].CHIP_RDYn | rx[8].CHIP_RDYn) == 0);
}

uint8_t cc1101_write_fifo(const uint8_t* data, uint8_t size) 
{
    uint8_t buff_tx[64];
    uint8_t buff_rx[64];
    buff_tx[0] = CC1101_FIFO | CC1101_BURST;
    memcpy(&buff_tx[1], data, size);

    // Start transaction
    // Wait IC to become ready
    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
        ;
    // Tell IC what we want
    spi_trx(hspi1, buff_tx, (uint8_t*)buff_rx, size + 1, CC1101_TIMEOUT);

    return size;
}

uint8_t cc1101_read_fifo(uint8_t* data, uint8_t* size) 
{
    uint8_t buff_trx[2];
    buff_trx[0] = CC1101_FIFO | CC1101_READ | CC1101_BURST;

    // Start transaction
    // Wait IC to become ready
    while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))
        ;

    // First byte - packet length
    spi_trx(hspi1, buff_trx, buff_trx, 2, CC1101_TIMEOUT);

    // Check that the packet is placed in the receive buffer
    if(buff_trx[1] > 64) {
        *size = 64;
    } else {
        *size = buff_trx[1];
    }
    spi_trx(hspi1, NULL, data, *size, CC1101_TIMEOUT);

    return *size;
}

/*-----------------------------------------------------------------------------------*/

uint8_t cc1101_get_partnumber() 
{
    uint8_t partnumber = 0;
    cc1101_read_reg(CC1101_STATUS_PARTNUM | CC1101_BURST, &partnumber);
    return partnumber;
}

uint8_t cc1101_get_version() 
{
    uint8_t version = 0;
    cc1101_read_reg(CC1101_STATUS_VERSION | CC1101_BURST, &version);
    return version;
}

uint8_t cc1101_get_rssi() 
{
    uint8_t rssi = 0;
    cc1101_read_reg(CC1101_STATUS_RSSI | CC1101_BURST, &rssi);
    return rssi;
}

void cc1101_reset() 
{
    cc1101_strobe(CC1101_STROBE_SRES);
}

CC1101Status cc1101_get_status() 
{
    return cc1101_strobe(CC1101_STROBE_SNOP);
}

void cc1101_shutdown() 
{
    cc1101_strobe(CC1101_STROBE_SPWD);
}

void cc1101_calibrate() 
{
    cc1101_strobe(CC1101_STROBE_SCAL);
}

void cc1101_switch_to_idle() 
{
    cc1101_strobe(CC1101_STROBE_SIDLE);
}

void cc1101_switch_to_rx() 
{
    cc1101_strobe(CC1101_STROBE_SRX);
    //GD0_Input();
}

void cc1101_switch_to_tx() 
{
    cc1101_strobe(CC1101_STROBE_STX);
    //GD0_Output();
}

void cc1101_flush_rx() 
{
    cc1101_strobe(CC1101_STROBE_SFRX);
}

void cc1101_flush_tx() 
{
    cc1101_strobe(CC1101_STROBE_SFTX);
}

void cc1101_wake()
{
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}
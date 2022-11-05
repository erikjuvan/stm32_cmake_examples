/**
 * @file     printf_support.c
 * @brief    Function to enable printing of characters via UART when using printf.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "usart.h"
#include "printf_support.h"

////////////////////////////////////////////////////////////////////////////////
// External variables
////////////////////////////////////////////////////////////////////////////////
extern UART_HandleTypeDef UartHandle;


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Final function in printf chain that actually prints characters.
 *
 * @param[in]    ch    Character to print.
 * @return       Number of characters printed.
 */
int __io_putchar(int ch)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
#ifdef LOG_VIA_UART
    ret = HAL_UART_Transmit(&UartHandle, (uint8_t*) &ch, 1, 25);
#endif
	if (ret == HAL_OK)
	{
		return (int)1;
	}
	else
	{
		return (int)0;
	}
}

/**
 * Get character from stream.
 *
 * @return       Received character.
 */
int __io_getchar(void)
{
	uint8_t ch;

	if (HAL_UART_Receive(&UartHandle, &ch, 1, 25) == HAL_OK)
	{
	    return (int)ch;
	}
	else
	{
	    return (int)0;
	}
}

/**
 * Call __io_putchar.
 *
 * @param[in]    c    Character to print.
 * @return       Number of printed characters.
 */
uint8_t _putc(char c)
{
    __io_putchar(c);
    return ((uint8_t) c);
}

/**
 * Wrapper calling _putc.
 *
 * @param[in]    s    String to print.
 * @return       Always 1.
 */
uint8_t _puts(char *s)
{
    for ( ; *s != '\0'; s++ )
    {
        _putc(*s);
    }
    return 1;
}

/**
 * Configure USART used for printing.
 *
 * @return       None.
 */
void USART_Configuration(void)
{
#ifdef LOG_VIA_UART
	MX_USART5_UART_Init();
#endif
}

/**
 * Reconfigure USART used for printing.
 *
 * @return       None.
 */
void USART_Reconfiguration(void)
{
	USART_Configuration();
}

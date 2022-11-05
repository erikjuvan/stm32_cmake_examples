/**
 * @file     error.c
 * @brief    Error handler that forwards error type to USER
 * @author   Erik Juvan
 * @date     13.07.2022
 * @version  V1.0.0
 */

/**
 * @addtogroup ERROR
 * @{ <!-- BEGIN GROUP -->
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "error.h"
#include "func_table.h"
#include "trace.h"


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#if (CONF_ERROR_LOG == 1)
#define ERROR_LOG  /**< Debug macro for TRACE */
#endif

#define ERROR_VERSION         "1.0.0"       /**< Module version. */
#define ERROR_NAME            "Safe Error"  /**< Module name. */
#define ERROR_NAME_SHORT      "SER"         /**< SAFE Module short name. */


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Report error to USER app.
 *
 * @param[in]  error  Type of error.
 * @return     None.
 */
void error_handler(safe_error_t error)
{
#ifdef LOG_VIA_UART
    static safe_error_t prev_error = eSAFE_ERROR_NONE;
    static const char* txt[] = {
        "eSAFE_ERROR_NONE",
        "eSAFE_ERROR_RESET",
        "eSAFE_ERROR_DISABLED"
    };
    if (prev_error != error)
    {
        TRACE_ERR("%s ::: error_handler %s", ERROR_NAME_SHORT, txt[(int)error]);
        prev_error = error;
    }
#endif

    pg_user_func_table->fp_safe_error_handler(error);
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

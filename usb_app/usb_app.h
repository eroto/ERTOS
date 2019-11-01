/*
 * usb_app.h
 *
 *  Created on: 06/10/2019
 *      Author: uid87057
 */

#ifndef USB_APP_H_
#define USB_APP_H_

#include "stdint.h"
#include "usb.h"

#define MAX_USB_TX_RETRY_CTR		10u


/*USB satus sent to send to applicaiton for error information and debug*/
static usb_status_t APP_USB_status = kStatus_USB_Error;


usb_status_t usb_app_GetUSBstatus(void);

#endif /* USB_APP_H_ */

/*
 * Copyright (C) 2012 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : RTL8367/RTL8367C switch high-level API
 *
 * Feature : The file includes the following modules and sub-modules
 *           (1) OAM (802.3ah) configuration
 *
 */

#ifndef __DAL_RTL8367C_OAM_H__
#define __DAL_RTL8367C_OAM_H__

#include "../../oam.h"
/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */


/*
 * Macro Declaration
 */


/*
 * Function Declaration
 */

/* Function Name:
 *      dal_rtl8367c_oam_init
 * Description:
 *      Initialize oam module.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      Must initialize oam module before calling any oam APIs.
 */
extern rtk_api_ret_t dal_rtl8367c_oam_init(void);

/* Function Name:
 *      dal_rtl8367c_oam_state_set
 * Description:
 *      This API set OAM state.
 * Input:
 *      enabled     -OAMstate
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error parameter
 * Note:
 *      This API set OAM state.
 */
extern rtk_api_ret_t dal_rtl8367c_oam_state_set(rtk_enable_t enabled);

/* Function Name:
 *      dal_rtl8367c_oam_state_get
 * Description:
 *      This API get OAM state.
 * Input:
 *      None.
 * Output:
 *      pEnabled        - H/W IGMP state
 * Return:
 *      RT_ERR_OK              - OK
 *      RT_ERR_FAILED          - Failed
 *      RT_ERR_SMI             - SMI access error
 *      RT_ERR_INPUT           - Error parameter
 * Note:
 *      This API set current OAM state.
 */
extern rtk_api_ret_t dal_rtl8367c_oam_state_get(rtk_enable_t *pEnabled);


/* Module Name : OAM */

/* Function Name:
 *      dal_rtl8367c_oam_parserAction_set
 * Description:
 *      Set OAM parser action
 * Input:
 *      port    - port id
 *      action  - parser action
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern rtk_api_ret_t dal_rtl8367c_oam_parserAction_set(rtk_port_t port, rtk_oam_parser_act_t action);

/* Function Name:
 *      dal_rtl8367c_oam_parserAction_set
 * Description:
 *      Get OAM parser action
 * Input:
 *      port    - port id
 * Output:
 *      pAction  - parser action
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern rtk_api_ret_t dal_rtl8367c_oam_parserAction_get(rtk_port_t port, rtk_oam_parser_act_t *pAction);


/* Function Name:
 *      dal_rtl8367c_oam_multiplexerAction_set
 * Description:
 *      Set OAM multiplexer action
 * Input:
 *      port    - port id
 *      action  - parser action
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern rtk_api_ret_t dal_rtl8367c_oam_multiplexerAction_set(rtk_port_t port, rtk_oam_multiplexer_act_t action);

/* Function Name:
 *      dal_rtl8367c_oam_multiplexerAction_set
 * Description:
 *      Get OAM multiplexer action
 * Input:
 *      port    - port id
 * Output:
 *      pAction  - parser action
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_PORT_ID      - invalid port id
 * Note:
 *      None
 */
extern rtk_api_ret_t dal_rtl8367c_oam_multiplexerAction_get(rtk_port_t port, rtk_oam_multiplexer_act_t *pAction);


#endif /* __DAL_RTL8367C_OAM_H__ */


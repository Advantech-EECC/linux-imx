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
 * $Revision: $
 * $Date: $
 *
 * Purpose : Declaration of RLDP and RLPP API
 *
 * Feature : The file have include the following module and sub-modules
 *           1) RLDP and RLPP configuration and status
 *
 */


/*
 * Include Files
 */
#include "../../rtk_switch.h"
#include "../../rtk_error.h"
#include "dal_rtl8367c_rldp.h"


#include "rtl8367c_asicdrv.h"
#include "rtl8367c_asicdrv_rldp.h"


/* Function Name:
 *      dal_rtl8367c_rldp_config_set
 * Description:
 *      Set RLDP module configuration
 * Input:
 *      pConfig - configuration structure of RLDP
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_config_set(rtk_rldp_config_t *pConfig)
{
    rtk_api_ret_t retVal;
    ether_addr_t magic;
    rtk_uint32 pmsk;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if (pConfig->rldp_enable >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if (pConfig->trigger_mode >= RTK_RLDP_TRIGGER_END)
        return RT_ERR_INPUT;

    if (pConfig->compare_type >= RTK_RLDP_CMPTYPE_END)
        return RT_ERR_INPUT;

    if (pConfig->num_check >= RTK_RLDP_NUM_MAX)
        return RT_ERR_INPUT;

    if (pConfig->interval_check >= RTK_RLDP_INTERVAL_MAX)
        return RT_ERR_INPUT;

    if (pConfig->num_loop >= RTK_RLDP_NUM_MAX)
        return RT_ERR_INPUT;

    if (pConfig->interval_loop >= RTK_RLDP_INTERVAL_MAX)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_getAsicRldpTxPortmask(&pmsk))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpTxPortmask(0x00))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpTxPortmask(pmsk))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldp(pConfig->rldp_enable))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpTriggerMode(pConfig->trigger_mode))!=RT_ERR_OK)
        return retVal;

    memcpy(&magic, &pConfig->magic, sizeof(ether_addr_t));
    if ((retVal = rtl8367c_setAsicRldpMagicNum(magic))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpCompareRandomNumber(pConfig->compare_type))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpCompareRandomNumber(pConfig->compare_type))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpCheckingStatePara(pConfig->num_check, pConfig->interval_check))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicRldpLoopStatePara(pConfig->num_loop, pConfig->interval_loop))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_rtl8367c_rldp_config_get
 * Description:
 *      Get RLDP module configuration
 * Input:
 *      None
 * Output:
 *      pConfig - configuration structure of RLDP
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_config_get(rtk_rldp_config_t *pConfig)
{
    rtk_api_ret_t retVal;
    ether_addr_t magic;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if ((retVal = rtl8367c_getAsicRldp(&pConfig->rldp_enable))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicRldpTriggerMode(&pConfig->trigger_mode))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicRldpMagicNum(&magic))!=RT_ERR_OK)
        return retVal;
    memcpy(&pConfig->magic, &magic, sizeof(ether_addr_t));

    if ((retVal = rtl8367c_getAsicRldpCompareRandomNumber(&pConfig->compare_type))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicRldpCompareRandomNumber(&pConfig->compare_type))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicRldpCheckingStatePara(&pConfig->num_check, &pConfig->interval_check))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicRldpLoopStatePara(&pConfig->num_loop, &pConfig->interval_loop))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}


/* Function Name:
 *      dal_rtl8367c_rldp_portConfig_set
 * Description:
 *      Set per port RLDP module configuration
 * Input:
 *      port   - port number to be configured
 *      pPortConfig - per port configuration structure of RLDP
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_portConfig_set(rtk_port_t port, rtk_rldp_portConfig_t *pPortConfig)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmsk;
    rtk_uint32 phyPort;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if (pPortConfig->tx_enable>= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    phyPort = rtk_switch_port_L2P_get(port);
    if (phyPort == UNDEFINE_PHY_PORT)
        return RT_ERR_PORT_ID;

    if ((retVal = rtl8367c_getAsicRldpTxPortmask(&pmsk))!=RT_ERR_OK)
        return retVal;

    if (pPortConfig->tx_enable)
    {
         pmsk |=(1<<phyPort);
    }
    else
    {
         pmsk &= ~(1<<phyPort);
    }

    if ((retVal = rtl8367c_setAsicRldpTxPortmask(pmsk))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;

}

/* Function Name:
 *      dal_rtl8367c_rldp_portConfig_get
 * Description:
 *      Get per port RLDP module configuration
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortConfig - per port configuration structure of RLDP
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_portConfig_get(rtk_port_t port, rtk_rldp_portConfig_t *pPortConfig)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmsk;
    rtk_portmask_t logicalPmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if ((retVal = rtl8367c_getAsicRldpTxPortmask(&pmsk))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtk_switch_portmask_P2L_get(pmsk, &logicalPmask)) != RT_ERR_OK)
        return retVal;


    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortConfig->tx_enable = ENABLED;
    }
    else
    {
         pPortConfig->tx_enable = DISABLED;
    }

    return RT_ERR_OK;
}



/* Function Name:
 *      dal_rtl8367c_rldp_status_get
 * Description:
 *      Get RLDP module status
 * Input:
 *      None
 * Output:
 *      pStatus - status structure of RLDP
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_status_get(rtk_rldp_status_t *pStatus)
{
    rtk_api_ret_t retVal;
    ether_addr_t seed;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if ((retVal = rtl8367c_getAsicRldpRandomNumber(&seed))!=RT_ERR_OK)
        return retVal;
    memcpy(&pStatus->id, &seed, sizeof(ether_addr_t));

    return RT_ERR_OK;
}

/* Function Name:
 *      dal_rtl8367c_rldp_portStatus_get
 * Description:
 *      Get RLDP module status
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortStatus - per port status structure of RLDP
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_portStatus_get(rtk_port_t port, rtk_rldp_portStatus_t *pPortStatus)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmsk;
    rtk_portmask_t logicalPmask;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if ((retVal = rtl8367c_getAsicRldpLoopedPortmask(&pmsk))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtk_switch_portmask_P2L_get(pmsk, &logicalPmask)) != RT_ERR_OK)
        return retVal;

    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortStatus->loop_status = RTK_RLDP_LOOPSTS_LOOPING;
    }
    else
    {
         pPortStatus->loop_status  = RTK_RLDP_LOOPSTS_NONE;
    }

    if ((retVal = rtl8367c_getAsicRldpEnterLoopedPortmask(&pmsk))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtk_switch_portmask_P2L_get(pmsk, &logicalPmask)) != RT_ERR_OK)
        return retVal;

    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortStatus->loop_enter = RTK_RLDP_LOOPSTS_LOOPING;
    }
    else
    {
         pPortStatus->loop_enter  = RTK_RLDP_LOOPSTS_NONE;
    }

    if ((retVal = rtl8367c_getAsicRldpLeaveLoopedPortmask(&pmsk))!=RT_ERR_OK)
        return retVal;
    if ((retVal = rtk_switch_portmask_P2L_get(pmsk, &logicalPmask)) != RT_ERR_OK)
        return retVal;

    if (logicalPmask.bits[0] & (1<<port))
    {
         pPortStatus->loop_leave = RTK_RLDP_LOOPSTS_LOOPING;
    }
    else
    {
         pPortStatus->loop_leave  = RTK_RLDP_LOOPSTS_NONE;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      dal_rtl8367c_rldp_portStatus_clear
 * Description:
 *      Clear RLDP module status
 * Input:
 *      port    - port number to be clear
 *      pPortStatus - per port status structure of RLDP
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      Clear operation effect loop_enter and loop_leave only, other field in
 *      the structure are don't care
 */
rtk_api_ret_t dal_rtl8367c_rldp_portStatus_set(rtk_port_t port, rtk_rldp_portStatus_t *pPortStatus)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmsk;
    rtk_uint32 phyPort;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    phyPort = rtk_switch_port_L2P_get(port);
    if (phyPort == UNDEFINE_PHY_PORT)
        return RT_ERR_PORT_ID;

    pmsk = (pPortStatus->loop_enter) << phyPort;
    if ((retVal = rtl8367c_setAsicRldpEnterLoopedPortmask(pmsk))!=RT_ERR_OK)
        return retVal;

    pmsk = (pPortStatus->loop_leave) << phyPort;
    if ((retVal = rtl8367c_setAsicRldpLeaveLoopedPortmask(pmsk))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}


/* Function Name:
 *      dal_rtl8367c_rldp_portLoopPair_get
 * Description:
 *      Get RLDP port loop pairs
 * Input:
 *      port    - port number to be get
 * Output:
 *      pPortmask - per port related loop ports
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_INPUT
 *      RT_ERR_NULL_POINTER
 * Note:
 *      None
 */
rtk_api_ret_t dal_rtl8367c_rldp_portLoopPair_get(rtk_port_t port, rtk_portmask_t *pPortmask)
{
    rtk_api_ret_t retVal;
    rtk_uint32 pmsk;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if ((retVal = rtl8367c_getAsicRldpLoopedPortPair(rtk_switch_port_L2P_get(port), &pmsk))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtk_switch_portmask_P2L_get(pmsk, pPortmask)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}


/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
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
 * Purpose : RTL8367C switch high-level API for RTL8367C
 * Feature : Port isolation related functions
 *
 */

#include "rtl8367c_asicdrv_portIsolation.h"
/* Function Name:
 *      rtl8367c_setAsicPortIsolationPermittedPortmask
 * Description:
 *      Set permitted port isolation portmask
 * Input:
 *      port            - Physical port number (0~10)
 *      permitPortmask  - port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number
 *      RT_ERR_PORT_MASK    - Invalid portmask
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicPortIsolationPermittedPortmask(rtk_uint32 port, rtk_uint32 permitPortmask)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if( permitPortmask > RTL8367C_PORTMASK)
        return RT_ERR_PORT_MASK;

    return rtl8367c_setAsicReg(RTL8367C_PORT_ISOLATION_PORT_MASK_REG(port), permitPortmask);
}
/* Function Name:
 *      rtl8367c_getAsicPortIsolationPermittedPortmask
 * Description:
 *      Get permitted port isolation portmask
 * Input:
 *      port                - Physical port number (0~10)
 *      pPermitPortmask     - port mask
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortIsolationPermittedPortmask(rtk_uint32 port, rtk_uint32 *pPermitPortmask)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicReg(RTL8367C_PORT_ISOLATION_PORT_MASK_REG(port), pPermitPortmask);
}
/* Function Name:
 *      rtl8367c_setAsicPortIsolationEfid
 * Description:
 *      Set port isolation EFID
 * Input:
 *      port    - Physical port number (0~10)
 *      efid    - EFID (0~7)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number
 *      RT_ERR_OUT_OF_RANGE - Input parameter out of range
 * Note:
 *      EFID is used in individual learning in filtering database
 */
ret_t rtl8367c_setAsicPortIsolationEfid(rtk_uint32 port, rtk_uint32 efid)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if( efid > RTL8367C_EFIDMAX)
        return RT_ERR_OUT_OF_RANGE;

    return rtl8367c_setAsicRegBits(RTL8367C_PORT_EFID_REG(port), RTL8367C_PORT_EFID_MASK(port), efid);
}
/* Function Name:
 *      rtl8367c_getAsicPortIsolationEfid
 * Description:
 *      Get port isolation EFID
 * Input:
 *      port    - Physical port number (0~10)
 *      pEfid   - EFID (0~7)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK           - Success
 *      RT_ERR_SMI          - SMI access error
 *      RT_ERR_PORT_ID      - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicPortIsolationEfid(rtk_uint32 port, rtk_uint32 *pEfid)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBits(RTL8367C_PORT_EFID_REG(port), RTL8367C_PORT_EFID_MASK(port), pEfid);
}


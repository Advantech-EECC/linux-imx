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
 * Feature : Green ethernet related functions
 *
 */
#include "rtl8367c_asicdrv_green.h"

/* Function Name:
 *      rtl8367c_getAsicGreenPortPage
 * Description:
 *      Get per-Port ingress page usage per second
 * Input:
 *      port    - Physical port number (0~7)
 *      pPage   - page number of ingress packet occuping per second
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      Ingress traffic occuping page number per second for high layer green feature usage
 */
ret_t rtl8367c_getAsicGreenPortPage(rtk_uint32 port, rtk_uint32* pPage)
{
    ret_t retVal;
    rtk_uint32 regData;
    rtk_uint32 pageMeter;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    retVal = rtl8367c_getAsicReg(RTL8367C_PAGEMETER_PORT_REG(port), &regData);
    if(retVal != RT_ERR_OK)
        return retVal;

   pageMeter = regData;

    retVal = rtl8367c_getAsicReg(RTL8367C_PAGEMETER_PORT_REG(port) + 1, &regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    pageMeter = pageMeter + (regData << 16);

    *pPage = pageMeter;
    return RT_ERR_OK;
}
/* Function Name:
 *      rtl8367c_setAsicGreenTrafficType
 * Description:
 *      Set traffic type for each priority
 * Input:
 *      priority    - internal priority (0~7)
 *      traffictype - high/low traffic type, 1:high priority traffic type, 0:low priority traffic type
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicGreenTrafficType(rtk_uint32 priority, rtk_uint32 traffictype)
{

    if(priority > RTL8367C_PRIMAX)
        return RT_ERR_QOS_INT_PRIORITY;

    return rtl8367c_setAsicRegBit(RTL8367C_REG_HIGHPRI_CFG, priority, (traffictype?1:0));
}
/* Function Name:
 *      rtl8367c_getAsicGreenTrafficType
 * Description:
 *      Get traffic type for each priority
 * Input:
 *      priority    - internal priority (0~7)
 *      pTraffictype - high/low traffic type, 1:high priority traffic type, 0:low priority traffic type
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK               - Success
 *      RT_ERR_SMI              - SMI access error
 *      RT_ERR_QOS_INT_PRIORITY - Invalid priority
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicGreenTrafficType(rtk_uint32 priority, rtk_uint32* pTraffictype)
{
    if(priority > RTL8367C_PRIMAX)
        return RT_ERR_QOS_INT_PRIORITY;

    return rtl8367c_getAsicRegBit(RTL8367C_REG_HIGHPRI_CFG, priority, pTraffictype);
}

/* Function Name:
 *      rtl8367c_setAsicGreenHighPriorityTraffic
 * Description:
 *      Set indicator which ASIC had received high priority traffic
 * Input:
 *      port            - Physical port number (0~7)
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_setAsicGreenHighPriorityTraffic(rtk_uint32 port)
{
    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(RTL8367C_REG_HIGHPRI_INDICATOR, port, 1);
}


/* Function Name:
 *      rtl8367c_getAsicGreenHighPriorityTraffic
 * Description:
 *      Get indicator which ASIC had received high priority traffic or not
 * Input:
 *      port        - Physical port number (0~7)
 *      pIndicator  - Have received high priority traffic indicator. If 1 means ASCI had received high priority in 1second checking priod
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK       - Success
 *      RT_ERR_SMI      - SMI access error
 *      RT_ERR_PORT_ID  - Invalid port number
 * Note:
 *      None
 */
ret_t rtl8367c_getAsicGreenHighPriorityTraffic(rtk_uint32 port, rtk_uint32* pIndicator)
{
    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(RTL8367C_REG_HIGHPRI_INDICATOR, port, pIndicator);
}

/*
@func rtk_int32 | rtl8367c_setAsicGreenEthernet | Set green ethernet function.
@parm rtk_uint32 | green | Green feature function usage 1:enable 0:disable.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@comm
    The API can set Green Ethernet function to reduce power consumption. While green feature is enabled, ASIC will automatic
 detect the cable length and then select different power mode for best performance with minimums power consumption. Link down
 ports will enter power savining mode in 10 seconds after the cable disconnected if power saving function is enabled.
*/
ret_t rtl8367c_setAsicGreenEthernet(rtk_uint32 port, rtk_uint32 green)
{
    ret_t retVal;
    rtk_uint32 checkCounter;
    rtk_uint32 regData;
    rtk_uint32 phy_status;
    rtk_uint32 patchData[6][2] = { {0x809A, 0x8911}, {0x80A3, 0x9233}, {0x80AC, 0xA444}, {0x809F, 0x6B20}, {0x80A8, 0x6B22}, {0x80B1, 0x6B23} };
    rtk_uint32 idx;
    rtk_uint32 data;

    if (green > 1)
        return RT_ERR_INPUT;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &data)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    /* Stop Watchdog */
    switch (data)
    {
        case 0x0652:
        case 0x6368:
        case 0x0801:
        case 0x6511:
            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB83E, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x00A9;
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xB83E, regData)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB840, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x00A9;
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xB840, regData)) != RT_ERR_OK)
                return retVal;
            break;
        default:
            break;;
    }

    /* 0xa420[2:0] */
    if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xA420, &regData)) != RT_ERR_OK)
        return retVal;
    phy_status = (regData & 0x0007);

    if(phy_status == 3)
    {
        /* 0xb820[4] = 1 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB820, &regData)) != RT_ERR_OK)
            return retVal;

        regData |= (0x0001 << 4);

        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xB820, regData)) != RT_ERR_OK)
            return retVal;

        /* wait 0xb800[6] = 1 */
        checkCounter = 100;
        while(checkCounter)
        {
            retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB800, &regData);
            if( (retVal != RT_ERR_OK) || ((regData & 0x0040) != 0x0040) )
            {
                checkCounter --;
                if(0 == checkCounter)
                    return RT_ERR_BUSYWAIT_TIMEOUT;
            }
            else
                checkCounter = 0;
        }
    }

    switch (data)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            if(green)
            {
                for(idx = 0; idx < 6; idx++ )
                {
                    if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA436, patchData[idx][0])) != RT_ERR_OK)
                        return retVal;

                    if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA438, patchData[idx][1])) != RT_ERR_OK)
                        return retVal;
                }
            }
            break;
        default:
            break;;
    }



    /* 0xa436 = 0x8011 */
    if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA436, 0x8011)) != RT_ERR_OK)
        return retVal;

    /* wr 0xa438[15] = 0: disable, 1: enable */
    if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xA438, &regData)) != RT_ERR_OK)
        return retVal;

    if(green)
        regData |= 0x8000;
    else
        regData &= 0x7FFF;

    if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA438, regData)) != RT_ERR_OK)
        return retVal;

    if(green == 0)
    {
        /* Disable Green, write analog parameter as long cable parameter */
        /* 0xBCC2   [14:12]     0x4
           0xBCC2   [10:8]      0x4
           0xBCC2   [7:4]       0x4
           0xBCC2   [3:0]       0x4 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xBCC2, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= 0x8800;
        regData |= 0x4444;
        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xBCC2, regData)) != RT_ERR_OK)
            return retVal;

        /* 0xBCCA   [9:8]       0x3
           0xBCCA   [7]         0x0 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xBCCA, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= 0xFF7F;
        regData |= 0x0300;
        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xBCCA, regData)) != RT_ERR_OK)
            return retVal;

        /* 0xBCC0   [2:0]       0x0 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xBCC0, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= 0xFFF8;
        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xBCC0, regData)) != RT_ERR_OK)
            return retVal;

        /* 0xA80E   [11:10]     0x2 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xA80E, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= 0xF3FF;
        regData |= 0x0800;
        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA80E, regData)) != RT_ERR_OK)
            return retVal;

        /* 0xBCD6   [2:0]       0x3 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xBCD6, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= 0xFFF8;
        regData |= 0x0003;
        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xBCD6, regData)) != RT_ERR_OK)
            return retVal;
    }

    /* Re-nway */
    if((retVal = rtl8367c_getAsicPHYReg(port, 0, &regData)) != RT_ERR_OK)
        return retVal;

    regData |= 0x0200;
    if((retVal = rtl8367c_setAsicPHYReg(port, 0, regData)) != RT_ERR_OK)
        return retVal;

    if(phy_status == 3)
    {
        /* 0xb820[4] = 0  */
        if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB820, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= ~(0x0001 << 4);

        if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xB820, regData)) != RT_ERR_OK)
            return retVal;

        /* wait 0xb800[6] = 0 */
        checkCounter = 100;
        while(checkCounter)
        {
            retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB800, &regData);
            if( (retVal != RT_ERR_OK) || ((regData & 0x0040) != 0x0000) )
            {
                checkCounter --;
                if(0 == checkCounter)
                    return RT_ERR_BUSYWAIT_TIMEOUT;
            }
            else
                checkCounter = 0;
        }
    }

    /* Start Watchdog */
    switch (data)
    {
        case 0x0652:
        case 0x6368:
        case 0x0801:
        case 0x6511:
            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB83E, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x0048;
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xB83E, regData)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xB840, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x00FA;
            if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xB840, regData)) != RT_ERR_OK)
                return retVal;
            break;
        default:
            break;;
    }

    return RT_ERR_OK;
}

/*
@func rtk_int32 | rtl8367c_getAsicGreenEthernet | Get green ethernet function.
@parm rtk_uint32 | *green | Green feature function usage 1:enable 0:disable.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@comm
    The API can set Green Ethernet function to reduce power consumption. While green feature is enabled, ASIC will automatic
 detect the cable length and then select different power mode for best performance with minimums power consumption. Link down
 ports will enter power savining mode in 10 seconds after the cable disconnected if power saving function is enabled.
*/
ret_t rtl8367c_getAsicGreenEthernet(rtk_uint32 port, rtk_uint32* green)
{
    ret_t retVal;
    rtk_uint32 regData;

    /* 0xa436 = 0x8011 */
    if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA436, 0x8011)) != RT_ERR_OK)
        return retVal;

    /* wr 0xa438[15] = 0: disable, 1: enable */
    if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xA438, &regData)) != RT_ERR_OK)
        return retVal;

    if(regData & 0x8000)
        *green = ENABLED;
    else
        *green = DISABLED;

    return RT_ERR_OK;
}


/*
@func ret_t | rtl8367c_setAsicPowerSaving | Set power saving mode
@parm rtk_uint32 | phy | phy number
@parm rtk_uint32 | enable | enable power saving mode.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@rvalue RT_ERR_PORT_ID | Invalid port number.
@comm
    The API can set power saving mode per phy.
*/
ret_t rtl8367c_setAsicPowerSaving(rtk_uint32 phy, rtk_uint32 enable)
{
    rtk_api_ret_t retVal;
    rtk_uint32 phyData;
    rtk_uint32 regData;
    rtk_uint32 phy_status;
    rtk_uint32 checkCounter;
    rtk_uint32 data;

    if (enable > 1)
        return RT_ERR_INPUT;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &data)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    /* Stop Watchdog */
    switch (data)
    {
        case 0x0652:
        case 0x6368:
        case 0x0801:
        case 0x6511:
            if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB83E, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x00A9;
            if((retVal = rtl8367c_setAsicPHYOCPReg(phy, 0xB83E, regData)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB840, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x00A9;
            if((retVal = rtl8367c_setAsicPHYOCPReg(phy, 0xB840, regData)) != RT_ERR_OK)
                return retVal;
            break;
        default:
            break;;
    }

    /* 0xa420[2:0] */
    if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xA420, &regData)) != RT_ERR_OK)
        return retVal;

    phy_status = (regData & 0x0007);

    if(phy_status == 3)
    {
        /* 0xb820[4] = 1 */
        if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB820, &regData)) != RT_ERR_OK)
            return retVal;

        regData |= (0x0001 << 4);

        if((retVal = rtl8367c_setAsicPHYOCPReg(phy, 0xB820, regData)) != RT_ERR_OK)
            return retVal;

        /* wait 0xb800[6] = 1 */
        checkCounter = 100;
        while(checkCounter)
        {
            retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB800, &regData);
            if( (retVal != RT_ERR_OK) || ((regData & 0x0040) != 0x0040) )
            {
                checkCounter --;
                if(0 == checkCounter)
                {
                     return RT_ERR_BUSYWAIT_TIMEOUT;
                }
            }
            else
                checkCounter = 0;
        }
    }

    if ((retVal = rtl8367c_getAsicPHYReg(phy,PHY_POWERSAVING_REG,&phyData))!=RT_ERR_OK)
        return retVal;

    phyData = phyData & ~(0x0001 << 2);
    phyData = phyData | (enable << 2);

    if ((retVal = rtl8367c_setAsicPHYReg(phy,PHY_POWERSAVING_REG,phyData))!=RT_ERR_OK)
        return retVal;

    if(phy_status == 3)
    {
        /* 0xb820[4] = 0  */
        if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB820, &regData)) != RT_ERR_OK)
            return retVal;

        regData &= ~(0x0001 << 4);

        if((retVal = rtl8367c_setAsicPHYOCPReg(phy, 0xB820, regData)) != RT_ERR_OK)
            return retVal;

        /* wait 0xb800[6] = 0 */
        checkCounter = 100;
        while(checkCounter)
        {
            retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB800, &regData);
            if( (retVal != RT_ERR_OK) || ((regData & 0x0040) != 0x0000) )
            {
                checkCounter --;
                if(0 == checkCounter)
                {
                    return RT_ERR_BUSYWAIT_TIMEOUT;
                }
            }
            else
                checkCounter = 0;
        }
    }

    /* Start Watchdog */
    switch (data)
    {
        case 0x0652:
        case 0x6368:
        case 0x0801:
        case 0x6511:
            if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB83E, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x0048;
            if((retVal = rtl8367c_setAsicPHYOCPReg(phy, 0xB83E, regData)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicPHYOCPReg(phy, 0xB840, &regData)) != RT_ERR_OK)
                return retVal;

            regData &= 0xFF00;
            regData |= 0x00FA;
            if((retVal = rtl8367c_setAsicPHYOCPReg(phy, 0xB840, regData)) != RT_ERR_OK)
                return retVal;
            break;
        default:
            break;;
    }

    return RT_ERR_OK;
}

/*
@func ret_t | rtl8367c_getAsicPowerSaving | Get power saving mode
@parm rtk_uint32 | port | The port number
@parm rtk_uint32* | enable | enable power saving mode.
@rvalue RT_ERR_OK | Success.
@rvalue RT_ERR_SMI | SMI access error.
@rvalue RT_ERR_PORT_ID | Invalid port number.
@comm
    The API can get power saving mode per phy.
*/
ret_t rtl8367c_getAsicPowerSaving(rtk_uint32 phy, rtk_uint32* enable)
{
    rtk_api_ret_t retVal;
    rtk_uint32 phyData;

    if(NULL == enable)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPHYReg(phy,PHY_POWERSAVING_REG,&phyData))!=RT_ERR_OK)
        return retVal;

    if ((phyData & 0x0004) > 0)
        *enable = 1;
    else
        *enable = 0;

    return RT_ERR_OK;
}


//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_Base.c
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol base data management and calculation.
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include "hisense_Base.h"
#include "hisense_LinkL.h"
#include "Platform/proc.h"

const uint16_t  CRC16Map[256] =
{
    0x0000, 0x8911, 0x1223, 0x9B32, 0x2446, 0xAD57, 0x3665, 0xBF74,
    0x488C, 0xC19D, 0x5AAF, 0xD3BE, 0x6CCA, 0xE5DB, 0x7EE9, 0xF7F8,
    0x8110, 0x0801, 0x9333, 0x1A22, 0xA556, 0x2C47, 0xB775, 0x3E64,
    0xC99C, 0x408D, 0xDBBF, 0x52AE, 0xEDDA, 0x64CB, 0xFFF9, 0x76E8,
    0x0221, 0x8B30, 0x1002, 0x9913, 0x2667, 0xAF76, 0x3444, 0xBD55,
    0x4AAD, 0xC3BC, 0x588E, 0xD19F, 0x6EEB, 0xE7FA, 0x7CC8, 0xF5D9,
    0x8331, 0x0A20, 0x9112, 0x1803, 0xA777, 0x2E66, 0xB554, 0x3C45,
    0xCBBD, 0x42AC, 0xD99E, 0x508F, 0xEFFB, 0x66EA, 0xFDD8, 0x74C9,
    0x0442, 0x8D53, 0x1661, 0x9F70, 0x2004, 0xA915, 0x3227, 0xBB36,
    0x4CCE, 0xC5DF, 0x5EED, 0xD7FC, 0x6888, 0xE199, 0x7AAB, 0xF3BA,
    0x8552, 0x0C43, 0x9771, 0x1E60, 0xA114, 0x2805, 0xB337, 0x3A26,
    0xCDDE, 0x44CF, 0xDFFD, 0x56EC, 0xE998, 0x6089, 0xFBBB, 0x72AA,
    0x0663, 0x8F72, 0x1440, 0x9D51, 0x2225, 0xAB34, 0x3006, 0xB917,
    0x4EEF, 0xC7FE, 0x5CCC, 0xD5DD, 0x6AA9, 0xE3B8, 0x788A, 0xF19B,
    0x8773, 0x0E62, 0x9550, 0x1C41, 0xA335, 0x2A24, 0xB116, 0x3807,
    0xCFFF, 0x46EE, 0xDDDC, 0x54CD, 0xEBB9, 0x62A8, 0xF99A, 0x708B,
    0x0884, 0x8195, 0x1AA7, 0x93B6, 0x2CC2, 0xA5D3, 0x3EE1, 0xB7F0,
    0x4008, 0xC919, 0x522B, 0xDB3A, 0x644E, 0xED5F, 0x766D, 0xFF7C,
    0x8994, 0x0085, 0x9BB7, 0x12A6, 0xADD2, 0x24C3, 0xBFF1, 0x36E0,
    0xC118, 0x4809, 0xD33B, 0x5A2A, 0xE55E, 0x6C4F, 0xF77D, 0x7E6C,
    0x0AA5, 0x83B4, 0x1886, 0x9197, 0x2EE3, 0xA7F2, 0x3CC0, 0xB5D1,
    0x4229, 0xCB38, 0x500A, 0xD91B, 0x666F, 0xEF7E, 0x744C, 0xFD5D,
    0x8BB5, 0x02A4, 0x9996, 0x1087, 0xAFF3, 0x26E2, 0xBDD0, 0x34C1,
    0xC339, 0x4A28, 0xD11A, 0x580B, 0xE77F, 0x6E6E, 0xF55C, 0x7C4D,
    0x0CC6, 0x85D7, 0x1EE5, 0x97F4, 0x2880, 0xA191, 0x3AA3, 0xB3B2,
    0x444A, 0xCD5B, 0x5669, 0xDF78, 0x600C, 0xE91D, 0x722F, 0xFB3E,
    0x8DD6, 0x04C7, 0x9FF5, 0x16E4, 0xA990, 0x2081, 0xBBB3, 0x32A2,
    0xC55A, 0x4C4B, 0xD779, 0x5E68, 0xE11C, 0x680D, 0xF33F, 0x7A2E,
    0x0EE7, 0x87F6, 0x1CC4, 0x95D5, 0x2AA1, 0xA3B0, 0x3882, 0xB193,
    0x466B, 0xCF7A, 0x5448, 0xDD59, 0x622D, 0xEB3C, 0x700E, 0xF91F,
    0x8FF7, 0x06E6, 0x9DD4, 0x14C5, 0xABB1, 0x22A0, 0xB992, 0x3083,
    0xC77B, 0x4E6A, 0xD558, 0x5C49, 0xE33D, 0x6A2C, 0xF11E, 0x780F
};

const uint32_t CRC32Map[256] =
{
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};


/*
Local functions:
*/






/*
Public API functions:
*/

void hisense_Base_InitCRC32(uint32_t *result_CRC32)
{
    *(result_CRC32) = 0xffffffff;
}

void hisense_Base_CalculateCRC32(uint32_t *result_CRC32, uint32_t dataLength, uint8_t *dataBytes)
{
    uint8_t temp;
    uint32_t counter = 0;
    for (;counter < dataLength;counter++)
    {
        temp = (uint8_t)(*result_CRC32);
        *result_CRC32 >>= 8;
        *result_CRC32 ^= CRC32Map[temp^*(dataBytes + counter)];
        proc_clearWDT();
    }
}


void hisense_Base_FinishCalculateCRC32(uint32_t *result_CRC32)
{
    *(result_CRC32) ^= 0xffffffff;
}

uint32_t hisense_Base_CalculateWholeArrayCRC32(uint32_t dataLength, uint8_t *dataBytes)
{
    uint32_t result_CRC32;

    result_CRC32 = 0;
    hisense_Base_InitCRC32(&result_CRC32);
    hisense_Base_CalculateCRC32(&result_CRC32, dataLength, dataBytes);
    hisense_Base_FinishCalculateCRC32(&result_CRC32);
    return result_CRC32;
}

void hisense_Base_ClearLocalBuffer(uint8_t *buffer, uint16_t length)
{
    uint16_t counter;

    for (counter = 0; counter < length; counter++)
    {
        *(buffer + counter) = 0;
    }
}

void hisense_Base_ShiftBytesToRight(uint8_t *buffer, uint16_t index, uint16_t bufferLength, uint16_t maxBufferLength)
{
    uint16_t counter;

    if (buffer == NULL)
    {
        return;
    }
    if (maxBufferLength <= bufferLength)
    {
        return;
    }

    for (counter = bufferLength; counter > index; counter--)
    {
        if (counter - 1 != 0)
        {
            *(buffer + counter) = *(buffer + counter - 1);
        }
        else
        {
            break;
        }
    }
}

void hisense_Base_HandleStuffedBytes(uint8_t *buffer, uint16_t *length, uint16_t maxBufferLength)
{
    uint16_t counter;
    uint16_t buffer_empty_space;

    if (*length == 0)
    {
        return;
    }
    if (*length <= 2)
    {
        return;
    }
    if (maxBufferLength <= *length)
    {
        return;
    }
    buffer_empty_space = maxBufferLength - (*length);

    for (counter = 2; counter < *length - 2; counter++)
    {
        if (buffer[counter] == HISENSE_LL_STUFFING_BYTE)
        {
            if (buffer_empty_space > 0)
            {
                hisense_Base_ShiftBytesToRight(buffer, counter, *length, maxBufferLength);
                counter++;  /* shifted is also 0xF4 so we will skip next one. */
                (*length)++;
                buffer_empty_space = maxBufferLength - (*length);
            }
            else
            {
                /* No more space left in buffer to perform shifting. Stuffing not complete !!! */
                break;
            }
        }
    }
}

NETBUF* hisense_Base_HandleStuffedBytes_nb(NETBUF* pnb)
{
    NETBUF_LEN nblen;
    NETBUF* src;
    NETBUF* dst;
    uint16_t position;
    uint16_t pktlen;
    uint16_t i;
    uint8_t* data;
    

    if (pnb == NULL)
    {
        return NULL;
    }

    dst = netbuf_gethdr();
    if (dst == NULL)
    {
        return NULL;
    }

    src = pnb;
    position = 0;
    pktlen = src->nb_pktlen;

    nblen = src->nb_len;
    while ((nblen > 0) && (src != NULL))
    {
        for (i = 0; i < nblen; i++)
        {
            data = NBTOD(src, BYTE*);
            if (netbuf_append(dst, 1, data+i) != 1)
            {
                netbuf_freem(dst);
                return NULL;
            }
            
            if ((position >= 2) && (position < pktlen - 2))
            {
                if (*(data+i) == HISENSE_LL_STUFFING_BYTE)
                {
                    if (netbuf_append(dst, 1, data+i) != 1)
                    {
                        netbuf_freem(dst);
                        return NULL;
                    }
                }
            }
            position++;
        }

        src = netbuf_free(src);
        if (src != NULL)
        {
            nblen = src->nb_len;
        }
    }

    return dst;

}

void hisense_Base_CopyData(uint8_t *source, uint8_t *destination, uint16_t length)
{
    uint16_t counter;

    if ((source == NULL) || (destination == NULL))
    {
        return;
    }

    for (counter = 0; counter < length; counter++)
    {
        *(destination + counter) = *(source + counter);
    }
}

uint8_t hisense_Base_ConvertIntToBCD(uint8_t value)
{
    return (uint8_t)(((value / 10) << 4) | (value % 10));
}

uint8_t hisense_Base_ConvertBCDToInt(uint8_t bcdByte)
{
    return (uint8_t)((((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F));
}

/* [] END OF FILE */

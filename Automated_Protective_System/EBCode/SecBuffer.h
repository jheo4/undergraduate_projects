/*
 * Copyright@ Samsung Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/*!
 * \file      SecBuffer.h
 * \brief     header file for SecBuffer
 * \author    Sangwoo, Park(sw5771.park@samsung.com)
 * \date      2011/06/02
 *
 * <b>Revision History: </b>
 * - 2010/06/03 : Sangwoo, Park(sw5771.park@samsung.com) \n
 *   Initial version
 *
 */

/**
 * @page SecBuffer
 *
 * @section Introduction
 * SecBuffer is common struct for buffer
 *
 * @section Copyright
 *  Copyright (c) 2008-2011 Samsung Electronics Co., Ltd.All rights reserved. \n
 *  Proprietary and Confidential
 *
 * @image html samsung.png
 */

#ifndef __SEC_BUFFER_H__
#define __SEC_BUFFER_H__

#include <sys/types.h>
//! Buffer information
struct SecBuffer
{
    //! Buffer type

    //! Buffer virtual address
    union {
        char *p;       //! single address.
        char *extP[3]; //! Y Cb Cr.
    } virt;

    //! Buffer physical address
    union {
        unsigned int p;       //! single address.
        unsigned int extP[3]; //! Y Cb Cr.
    } phys;

    //! Buffer reserved id
    union {
        unsigned int p;       //! \n
        unsigned int extP[3]; //! \n
    } reserved;

    //! Buffer size
    union {
        unsigned int s;
        unsigned int extS[3];
    } size;
};

#endif //__SEC_BUFFER_H__


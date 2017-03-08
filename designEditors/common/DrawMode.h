//-----------------------------------------------------------------------------
// File: DrawMode.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Joni-Matti M��tt�
// Date: 6.5.2011
//
// Description:
// Draw mode enumeration.
//-----------------------------------------------------------------------------

#ifndef DRAWMODE_H
#define DRAWMODE_H

//-----------------------------------------------------------------------------
//! DrawMode enumeration.
//-----------------------------------------------------------------------------
enum DrawMode
{
    MODE_NONE           = 0x00,
    MODE_SELECT         = 0x01,
    MODE_CONNECT        = 0x02,
    MODE_INTERFACE      = 0x04,
    MODE_DRAFT          = 0x08,
    MODE_TOGGLE_OFFPAGE = 0x10,
    MODE_LABEL          = 0x20
};

//-----------------------------------------------------------------------------

#endif // DRAWMODE_H

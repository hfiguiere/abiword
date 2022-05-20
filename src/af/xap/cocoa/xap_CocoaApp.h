/* -*- mode: C++; tab-width: 4; c-basic-offset: 4; -*- */

/* AbiSource Application Framework
 * Copyright (C) 1998 AbiSource, Inc.
 * Copyright (C) 2001, 2003-2004, 2009-2022 Hubert Figuière
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#pragma once

//  BAD HACK BAD HACK I can't include Foundation/NSTimer.h
//  because AP_Args somehow won't compile Obj-C.
typedef double NSTimeInterval;

#include <sys/stat.h>
#include <unistd.h>

#include "xap_App.h"
#include "xap_CocoaDialogFactory.h"
#include "xap_Cocoa_TB_CFactory.h"

class XAP_Args;
class XAP_CocoaToolbar_Icons;
class AV_View;
class EV_CocoaMenuBar;

/*****************************************************************
******************************************************************
** Only one of these is created by the application.
******************************************************************
*****************************************************************/

class XAP_CocoaApp : public XAP_App
{
public:
    XAP_CocoaApp(const char* szAppName, const char* app_id);
    virtual ~XAP_CocoaApp();

    virtual const char* getDefaultEncoding() const override;

    virtual bool initialize(const char* szKeyBindingsKey, const char* szKeyBindingsDefaultValue) override;
    virtual void reallyExit() override;
    virtual void notifyFrameCountChange() override;

    virtual const char* getUserPrivateDirectory() const override;
    virtual bool findAbiSuiteBundleFile(std::string& path, const char* filename, const char* subdir = nullptr) const; // checks only bundle
    virtual bool findAbiSuiteLibFile(std::string& path, const char* filename, const char* subdir = nullptr) const override;
    virtual bool findAbiSuiteAppFile(std::string& path, const char* filename, const char* subdir = nullptr) const override; // doesn't check user-dir

    virtual void setSelectionStatus(AV_View* pView) = 0;
    virtual void clearSelection() = 0;
    virtual bool getCurrentSelection(const char** formatList,
        void** ppData, UT_uint32* pLen,
        const char** pszFormatFound)
        = 0;

    typedef enum {
        GEOMETRY_FLAG_POS = 1 << 0,
        GEOMETRY_FLAG_SIZE = 1 << 1
    } windowGeometryFlags;

    struct windowGeometry {
        int x, y;
        UT_uint32 width, height;
        XAP_CocoaApp::windowGeometryFlags flags;
    };

    virtual void setGeometryCocoa(UT_sint32 x, UT_sint32 y, UT_uint32 width, UT_uint32 height,
        windowGeometryFlags flags);
    virtual void getGeometryCocoa(UT_sint32* x, UT_sint32* y, UT_uint32* width, UT_uint32* height,
        windowGeometryFlags* flags) const;

    void setTimeOfLastEvent(NSTimeInterval eventTime);
    NSTimeInterval getTimeOfLastEvent() const { return m_eventTime; };
    virtual BidiSupportType theOSHasBidiSupport() const override;
    EV_CocoaMenuBar* getCocoaMenuBar(void) const { return m_pCocoaMenu; };
    XAP_Frame* _getFrontFrame(void);
    XAP_CocoaToolbar_Icons* getToolbarIcons() const { return m_pCocoaToolbarIcons; }
    virtual XAP_DialogFactory* getDialogFactory() const override;
    virtual XAP_Toolbar_ControlFactory* getControlFactory() const override;

protected:
    virtual const char* _findNearestFont(const char* pszFontFamily,
        const char* pszFontStyle,
        const char* pszFontVariant,
        const char* pszFontWeight,
        const char* pszFontStretch,
        const char* pszFontSize,
        const char* pszLang);
    bool _loadFonts();
    void _setAbiSuiteLibDir();

private:
    XAP_CocoaToolbar_Icons* m_pCocoaToolbarIcons;
    AP_CocoaDialogFactory* m_dialogFactory;
    AP_CocoaToolbar_ControlFactory* m_controlFactory;

    windowGeometry m_geometry;
    NSTimeInterval m_eventTime; // e->time field of a recent X event
        // (we use this to sync clipboard
        // operations with the server).

    // Private structure to hold Obj-C types we can't see here
    struct Priv;

    Priv* m_priv;

protected: // TODO move that to private
    EV_CocoaMenuBar* m_pCocoaMenu;
    const char* m_szMenuLayoutName;
    const char* m_szMenuLabelSetName;
};

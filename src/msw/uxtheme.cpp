///////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/uxtheme.cpp
// Purpose:     implements wxUxThemeEngine class: support for XP themes
// Author:      John Platts, Vadim Zeitlin
// Modified by:
// Created:     2003
// Copyright:   (c) 2003 John Platts, Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"


#if wxUSE_UXTHEME

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/toplevel.h"
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/module.h"
#endif //WX_PRECOMP

#include "wx/dynlib.h"

#include "wx/msw/uxtheme.h"

bool wxUxThemeIsActive()
{
    return ::IsAppThemed() && ::IsThemeActive();
}

/* static */
HTHEME
wxUxThemeHandle::DoOpenThemeData(HWND hwnd, const wchar_t *classes, int dpi)
{
    // If DPI is the default one, we can use the old function.
    if ( dpi != STD_DPI )
    {
        // We need to use OpenThemeDataForDpi() which is not available under
        // all supported systems, so try to load it dynamically if not done yet.
        typedef HTHEME (WINAPI *OpenThemeDataForDpi_t)(HWND, LPCWSTR, UINT);
        static OpenThemeDataForDpi_t s_pfnOpenThemeDataForDpi = nullptr;
        static bool s_initDone = false;

        if ( !s_initDone )
        {
            wxLoadedDLL dllUxTheme(wxS("uxtheme.dll"));
            wxDL_INIT_FUNC(s_pfn, OpenThemeDataForDpi, dllUxTheme);
            s_initDone = true;
        }

        if ( s_pfnOpenThemeDataForDpi )
            return s_pfnOpenThemeDataForDpi(hwnd, classes, dpi);
    }

    return ::OpenThemeData(hwnd, classes);
}

wxColour wxUxThemeHandle::GetColour(int part, int prop, int state) const
{
    COLORREF col;

    HRESULT hr = ::GetThemeColor(m_hTheme, part, state, prop, &col);
    if ( FAILED(hr) )
    {
        wxLogApiError(
            wxString::Format("GetThemeColor(%i, %i, %i)", part, state, prop),
            hr
        );
        return wxColour{};
    }

    return wxRGBToColour(col);
}
#else
bool wxUxThemeIsActive()
{
    return false;
}
#endif // wxUSE_UXTHEME

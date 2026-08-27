/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "text.h"

#include "context.h"
#include "init.h"
#include "constants.h"

monotime_t __libtmrenderer_textblinkperiod;

NError tmrenderer_gettextblinkperiod(monotime_t *period)
{
    ENSURE_INIT;
    *period = v_textblinkperiod;
    return NError_Success;
}

NError tmrenderer_settextblinkperiod(monotime_t period)
{
    ENSURE_INIT;
    v_textblinkperiod = period ? period : DEFAULTTEXTBLINKPERIOD;
    return NError_Success;
}

NError tmrenderer_getkeystate(int keycode, int *state)
{
    ENSURE_INIT;
    *state = glfwGetKey(context.window, keycode);
    return NError_Success;
}

NError tmrenderer_getkeyname(int keycode, const char **keyname)
{
    ENSURE_INIT;
    *keyname = glfwGetKeyName(keycode, 0);
    return NError_Success;
}

NError tmrenderer_setkeycallback(TMRendererKeyInputCallback callback)
{
    ENSURE_INIT;
    context.keycallback = callback;
    return NError_Success;
}

NError tmrenderer_setcharcallback(TMRendererCharInputCallback callback)
{
    ENSURE_INIT;
    context.charcallback = callback;
    return NError_Success;
}
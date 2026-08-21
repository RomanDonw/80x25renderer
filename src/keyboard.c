/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "init.h"

//static TMRendererKeyInputCallback

NError tmrenderer_getkeystate(int keycode, int *state)
{
    ENSURE_INIT;
    *state = glfwGetKey(ctxn.window, keycode);
    return NError_Success;
}

NError tmrenderer_getkeyname(int keycode, const char **keyname)
{
    ENSURE_INIT;
    *keyname = glfwGetKeyName(keycode, 0);
    return NError_Success;
}

NError tmrenderer_setkeycallback(void (*callback)(int keycode, int scancode, int action, int mods))
{
    
}

NError tmrenderer_setcharcallback(void (*callback)(unsigned int codepoint))
{
    
}
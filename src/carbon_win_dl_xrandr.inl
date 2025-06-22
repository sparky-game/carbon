// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

static void *carbon_win__dl_Xrandr;

CARBON_WIN__DLDECL(XRRGetCrtcInfo);
XRRCrtcInfo *XRRGetCrtcInfo(Display *dpy, XRRScreenResources *resources, RRCrtc crtc) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetCrtcInfo);
  return XRRGetCrtcInfo_ptr(dpy, resources, crtc);
}

CARBON_WIN__DLDECL(XRRFreeOutputInfo);
void XRRFreeOutputInfo(XRROutputInfo *outputInfo) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRFreeOutputInfo);
  return XRRFreeOutputInfo_ptr(outputInfo);
}

CARBON_WIN__DLDECL(XRRGetScreenResources);
XRRScreenResources *XRRGetScreenResources(Display *dpy, Window window) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetScreenResources);
  return XRRGetScreenResources_ptr(dpy, window);
}

CARBON_WIN__DLDECL(XRRFreeScreenResources);
void XRRFreeScreenResources(XRRScreenResources *resources) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRFreeScreenResources);
  return XRRFreeScreenResources_ptr(resources);
}

CARBON_WIN__DLDECL(XRRSetCrtcConfig);
Status XRRSetCrtcConfig(Display *dpy,
                        XRRScreenResources *res,
                        RRCrtc crtc,
                        Time timestamp,
                        int x,
                        int y,
                        RRMode mode,
                        Rotation rotation,
                        RROutput *outputs,
                        int noutputs) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRSetCrtcConfig);
  return XRRSetCrtcConfig_ptr(dpy, res, crtc, timestamp, x, y, mode, rotation, outputs, noutputs);
}

CARBON_WIN__DLDECL(XRRGetScreenResourcesCurrent);
XRRScreenResources *XRRGetScreenResourcesCurrent(Display *dpy, Window window) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetScreenResourcesCurrent);
  return XRRGetScreenResourcesCurrent_ptr(dpy, window);
}

CARBON_WIN__DLDECL(XRRFreeCrtcInfo);
void XRRFreeCrtcInfo(XRRCrtcInfo *crtcInfo) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRFreeCrtcInfo);
  return XRRFreeCrtcInfo_ptr(crtcInfo);
}

CARBON_WIN__DLDECL(XRRGetOutputInfo);
XRROutputInfo *XRRGetOutputInfo(Display *dpy, XRRScreenResources *res, RROutput output) {
  CARBON_WIN__DLSYM(carbon_win__dl_Xrandr, XRRGetOutputInfo);
  return XRRGetOutputInfo_ptr(dpy, res, output);
}

// Local Variables:
// mode: c
// End:

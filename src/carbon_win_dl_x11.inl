// SPDX-License-Identifier: AGPL-3.0-only
// Copyright (C) Wasym A. Alonso. All Rights Reserved.

#pragma once

static void *carbon_win__dl_X11;

CARBON_WIN__DLDECL(XSendEvent);
Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSendEvent);
  return XSendEvent_ptr(display, w, propagate, event_mask, event_send);
}

CARBON_WIN__DLDECL(XTranslateCoordinates);
Bool XTranslateCoordinates(Display *display,
                           Window src_w,
                           Window dest_w,
                           int src_x,
                           int src_y,
                           int *dest_x_return,
                           int *dest_y_return,
                           Window *child_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XTranslateCoordinates);
  return XTranslateCoordinates_ptr(display, src_w, dest_w, src_x, src_y, dest_x_return, dest_y_return, child_return);
}

CARBON_WIN__DLDECL(XGetKeyboardControl);
int XGetKeyboardControl(Display *display, XKeyboardState *values_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetKeyboardControl);
  return XGetKeyboardControl_ptr(display, values_return);
}

CARBON_WIN__DLDECL(XResizeWindow);
int XResizeWindow(Display *display, Window w, unsigned width, unsigned height) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XResizeWindow);
  return XResizeWindow_ptr(display, w, width, height);
}

CARBON_WIN__DLDECL(XCreateFontCursor);
Cursor XCreateFontCursor(Display *display, unsigned int shape) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateFontCursor);
  return XCreateFontCursor_ptr(display, shape);
}

CARBON_WIN__DLDECL(XrmGetResource);
Bool XrmGetResource(XrmDatabase database,
                    const char *str_name,
                    const char *str_class,
                    char **str_type_return,
                    XrmValue *value_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XrmGetResource);
  return XrmGetResource_ptr(database, str_name, str_class, str_type_return, value_return);
}

CARBON_WIN__DLDECL(XStoreName);
int XStoreName(Display *display, Window w, const char *window_name) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XStoreName);
  return XStoreName_ptr(display, w, window_name);
}

CARBON_WIN__DLDECL(XEventsQueued);
int XEventsQueued(Display *display, int mode) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XEventsQueued);
  return XEventsQueued_ptr(display, mode);
}

CARBON_WIN__DLDECL(XPending);
int XPending(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XPending);
  return XPending_ptr(display);
}

CARBON_WIN__DLDECL(XkbGetState);
Status XkbGetState(Display *display, unsigned int device_spec, XkbStatePtr state_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XkbGetState);
  return XkbGetState_ptr(display, device_spec, state_return);
}

CARBON_WIN__DLDECL(XIconifyWindow);
Status XIconifyWindow(Display *display, Window w, int screen_number) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XIconifyWindow);
  return XIconifyWindow_ptr(display, w, screen_number);
}

CARBON_WIN__DLDECL(XFreeEventData);
void XFreeEventData(Display *display, XGenericEventCookie *cookie) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeEventData);
  return XFreeEventData_ptr(display, cookie);
}

CARBON_WIN__DLDECL(XGetWMNormalHints);
Status XGetWMNormalHints(Display *display, Window w, XSizeHints *hints_return, long *supplied_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetWMNormalHints);
  return XGetWMNormalHints_ptr(display, w, hints_return, supplied_return);
}

CARBON_WIN__DLDECL(XFreeColors);
int XFreeColors(Display *display, Colormap colormap, unsigned long pixels[], int npixels, unsigned long planes) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeColors);
  return XFreeColors_ptr(display, colormap, pixels, npixels, planes);
}

CARBON_WIN__DLDECL(XDestroyRegion);
int XDestroyRegion(Region r) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDestroyRegion);
  return XDestroyRegion_ptr(r);
}

CARBON_WIN__DLDECL(XFree);
int XFree(void *data) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFree);
  return XFree_ptr(data);
}

CARBON_WIN__DLDECL(XMoveWindow);
int XMoveWindow(Display *display, Window w, int x, int y) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMoveWindow);
  return XMoveWindow_ptr(display, w, x, y);
}

CARBON_WIN__DLDECL(XDeleteProperty);
int XDeleteProperty(Display *display, Window w, Atom property) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDeleteProperty);
  return XDeleteProperty_ptr(display, w, property);
}

CARBON_WIN__DLDECL(XrmGetStringDatabase);
XrmDatabase XrmGetStringDatabase(const char *data) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XrmGetStringDatabase);
  return XrmGetStringDatabase_ptr(data);
}

CARBON_WIN__DLDECL(XSetWMHints);
int XSetWMHints(Display *display, Window w, XWMHints *wmhints) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMHints);
  return XSetWMHints_ptr(display, w, wmhints);
}

CARBON_WIN__DLDECL(XPeekEvent);
int XPeekEvent(Display *display, XEvent *event_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XPeekEvent);
  return XPeekEvent_ptr(display, event_return);
}

CARBON_WIN__DLDECL(XDefaultVisual);
Visual *XDefaultVisual(Display *display, int screen) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefaultVisual);
  return XDefaultVisual_ptr(display, screen);
}

CARBON_WIN__DLDECL(XSetWMNormalHints);
void XSetWMNormalHints(Display *display, Window w, XSizeHints *hints) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMNormalHints);
  XSetWMNormalHints_ptr(display, w, hints);
}

CARBON_WIN__DLDECL(XGrabPointer);
int XGrabPointer(Display *display,
                 Window grab_window,
                 Bool owner_events,
                 unsigned int event_mask,
                 int pointer_mode,
                 int keyboard_mode,
                 Window confine_to,
                 Cursor cursor,
                 Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGrabPointer);
  return XGrabPointer_ptr(display, grab_window, owner_events, event_mask, pointer_mode, keyboard_mode, confine_to, cursor, time);
}

CARBON_WIN__DLDECL(XInitThreads);
Status XInitThreads(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XInitThreads);
  return XInitThreads_ptr();
}

CARBON_WIN__DLDECL(XWidthOfScreen);
int XWidthOfScreen(Screen *screen) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XWidthOfScreen);
  return XWidthOfScreen_ptr(screen);
}

CARBON_WIN__DLDECL(XDefaultScreen);
int XDefaultScreen(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefaultScreen);
  return XDefaultScreen_ptr(display);
}

CARBON_WIN__DLDECL(XWarpPointer);
int XWarpPointer(Display *display,
                 Window src_w,
                 Window dest_w,
                 int src_x,
                 int src_y,
                 unsigned int src_width,
                 unsigned int src_height,
                 int dest_x,
                 int dest_y) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XWarpPointer);
  return XWarpPointer_ptr(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y);
}

CARBON_WIN__DLDECL(XCreateRegion);
Region XCreateRegion(void) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateRegion);
  return XCreateRegion_ptr();
}

CARBON_WIN__DLDECL(XHeightOfScreen);
int XHeightOfScreen(Screen *screen) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XHeightOfScreen);
  return XHeightOfScreen_ptr(screen);
}

CARBON_WIN__DLDECL(XDestroyWindow);
int XDestroyWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDestroyWindow);
  return XDestroyWindow_ptr(display, w);
}

CARBON_WIN__DLDECL(XOpenDisplay);
Display *XOpenDisplay(const char *display_name) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XOpenDisplay);
  return XOpenDisplay_ptr(display_name);
}

CARBON_WIN__DLDECL(XSetWMProtocols);
Status XSetWMProtocols(Display *display, Window w, Atom *protocols, int count) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMProtocols);
  return XSetWMProtocols_ptr(display, w, protocols, count);
}

CARBON_WIN__DLDECL(XQueryPointer);
Bool XQueryPointer(Display *display,
                   Window w,
                   Window *root_return,
                   Window *child_return,
                   int *root_x_return,
                   int *root_y_return,
                   int *win_x_return,
                   int *win_y_return,
                   unsigned int *mask_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XQueryPointer);
  return XQueryPointer_ptr(display, w, root_return, child_return, root_x_return, root_y_return, win_x_return, win_y_return, mask_return);
}

CARBON_WIN__DLDECL(XUngrabPointer);
int XUngrabPointer(Display *display, Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XUngrabPointer);
  return XUngrabPointer_ptr(display, time);
}

CARBON_WIN__DLDECL(XGetSelectionOwner);
Window XGetSelectionOwner(Display *display, Atom selection) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetSelectionOwner);
  return XGetSelectionOwner_ptr(display, selection);
}

CARBON_WIN__DLDECL(XNextEvent);
int XNextEvent(Display *display, XEvent *event_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XNextEvent);
  return XNextEvent_ptr(display, event_return);
}

CARBON_WIN__DLDECL(XResourceManagerString);
char *XResourceManagerString(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XResourceManagerString);
  return XResourceManagerString_ptr(display);
}

CARBON_WIN__DLDECL(XMapRaised);
int XMapRaised(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMapRaised);
  return XMapRaised_ptr(display, w);
}

CARBON_WIN__DLDECL(XInternAtom);
Atom XInternAtom(Display *display, const char *atom_name, Bool only_if_exists) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XInternAtom);
  return XInternAtom_ptr(display, atom_name, only_if_exists);
}

CARBON_WIN__DLDECL(XGetEventData);
Status XGetEventData(Display *display, XGenericEventCookie *cookie) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetEventData);
  return XGetEventData_ptr(display, cookie);
}

CARBON_WIN__DLDECL(XChangeProperty);
int XChangeProperty(Display *display,
                    Window w,
                    Atom property,
                    Atom type,
                    int format,
                    int mode,
                    const unsigned char *data,
                    int nelements) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XChangeProperty);
  return XChangeProperty_ptr(display, w, property, type, format, mode, data, nelements);
}

CARBON_WIN__DLDECL(XDisplayName);
char *XDisplayName(const char *name) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDisplayName);
  return XDisplayName_ptr(name);
}

CARBON_WIN__DLDECL(XCreateWindow);
Window XCreateWindow(Display *display,
                     Window parent,
                     int x,
                     int y,
                     unsigned int width,
                     unsigned int height,
                     unsigned int border_width,
                     int depth,
                     unsigned int class,
                     Visual *visual,
                     unsigned long valuemask,
                     XSetWindowAttributes *attributes) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateWindow);
  return XCreateWindow_ptr(display, parent, x, y, width, height, border_width, depth, class, visual, valuemask, attributes);
}

CARBON_WIN__DLDECL(XSync);
int XSync(Display *display, Bool discard) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSync);
  return XSync_ptr(display, discard);
}

CARBON_WIN__DLDECL(XSelectInput);
int XSelectInput(Display *display, Window w, long event_mask) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSelectInput);
  return XSelectInput_ptr(display, w, event_mask);
}

CARBON_WIN__DLDECL(XSetClassHint);
int XSetClassHint(Display *display, Window w, XClassHint *class_hint) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetClassHint);
  return XSetClassHint_ptr(display, w, class_hint);
}

CARBON_WIN__DLDECL(XSetWMSizeHints);
void XSetWMSizeHints(Display *display, Window w, XSizeHints *hints, Atom property) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetWMSizeHints);
  return XSetWMSizeHints_ptr(display, w, hints, property);
}

CARBON_WIN__DLDECL(XSetSelectionOwner);
int XSetSelectionOwner(Display *display, Atom selection, Window owner, Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetSelectionOwner);
  return XSetSelectionOwner_ptr(display, selection, owner, time);
}

CARBON_WIN__DLDECL(XDefineCursor);
int XDefineCursor(Display *display, Window w, Cursor cursor) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefineCursor);
  return XDefineCursor_ptr(display, w, cursor);
}

CARBON_WIN__DLDECL(XCreateGC);
GC XCreateGC(Display *display, Drawable d, unsigned long valuemask, XGCValues *values) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateGC);
  return XCreateGC_ptr(display, d, valuemask, values);
}

CARBON_WIN__DLDECL(XUnmapWindow);
int XUnmapWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XUnmapWindow);
  return XUnmapWindow_ptr(display, w);
}

CARBON_WIN__DLDECL(XDefaultRootWindow);
Window XDefaultRootWindow(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XDefaultRootWindow);
  return XDefaultRootWindow_ptr(display);
}

CARBON_WIN__DLDECL(XFlush);
int XFlush(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFlush);
  return XFlush_ptr(display);
}

CARBON_WIN__DLDECL(XCreateColormap);
Colormap XCreateColormap(Display *display, Window w, Visual *visual, int alloc) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateColormap);
  return XCreateColormap_ptr(display, w, visual, alloc);
}

CARBON_WIN__DLDECL(XGetWindowAttributes);
int XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetWindowAttributes);
  return XGetWindowAttributes_ptr(display, w, window_attributes_return);
}

CARBON_WIN__DLDECL(XrmDestroyDatabase);
void XrmDestroyDatabase(XrmDatabase db) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XrmDestroyDatabase);
  XrmDestroyDatabase_ptr(db);
}

CARBON_WIN__DLDECL(XPutImage);
int XPutImage(Display *display,
              Drawable d,
              GC gc,
              XImage *image,
              int src_x,
              int src_y,
              int dest_x,
              int dest_y,
              unsigned int width,
              unsigned int height) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XPutImage);
  return XPutImage_ptr(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height);
}

CARBON_WIN__DLDECL(XMatchVisualInfo);
Status XMatchVisualInfo(Display *display, int screen, int depth, int class, XVisualInfo *visual_info_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMatchVisualInfo);
  return XMatchVisualInfo_ptr(display, screen, depth, class, visual_info_return);
}

CARBON_WIN__DLDECL(XFreeCursor);
int XFreeCursor(Display *display, Cursor cursor) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeCursor);
  return XFreeCursor_ptr(display, cursor);
}

CARBON_WIN__DLDECL(XMapWindow);
int XMapWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XMapWindow);
  return XMapWindow_ptr(display, w);
}

CARBON_WIN__DLDECL(XFreeGC);
int XFreeGC(Display *display, GC gc) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XFreeGC);
  return XFreeGC_ptr(display, gc);
}

CARBON_WIN__DLDECL(XGetWindowProperty);
int XGetWindowProperty(Display *display,
                       Window w,
                       Atom property,
                       long long_offset,
                       long long_length,
                       Bool delete,
                       Atom req_type,
                       Atom *actual_type_return,
                       int *actual_format_return,
                       unsigned long *nitems_return,
                       unsigned long *bytes_after_return,
                       unsigned char **prop_return) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XGetWindowProperty);
  return XGetWindowProperty_ptr(display, w, property, long_offset, long_length, delete, req_type, actual_type_return, actual_format_return, nitems_return, bytes_after_return, prop_return);
}

CARBON_WIN__DLDECL(XRaiseWindow);
int XRaiseWindow(Display *display, Window w) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XRaiseWindow);
  return XRaiseWindow_ptr(display, w);
}

CARBON_WIN__DLDECL(XCloseDisplay);
int XCloseDisplay(Display *display) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCloseDisplay);
  return XCloseDisplay_ptr(display);
}

CARBON_WIN__DLDECL(XCreateImage);
XImage *XCreateImage(Display *display,
                     Visual *visual,
                     unsigned int depth,
                     int format,
                     int offset,
                     char *data,
                     unsigned int width,
                     unsigned int height,
                     int bitmap_pad,
                     int bytes_per_line) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreateImage);
  return XCreateImage_ptr(display, visual, depth, format, offset, data, width, height, bitmap_pad, bytes_per_line);
}

CARBON_WIN__DLDECL(XCreatePixmap);
Pixmap XCreatePixmap(Display *display,
                     Drawable d,
                     unsigned int width,
                     unsigned int height,
                     unsigned int depth) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XCreatePixmap);
  return XCreatePixmap_ptr(display, d, width, height, depth);
}

CARBON_WIN__DLDECL(XkbKeycodeToKeysym);
KeySym XkbKeycodeToKeysym(Display *display, KeyCode kc, int group, int level) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XkbKeycodeToKeysym);
  return XkbKeycodeToKeysym_ptr(display, kc, group, level);
}

CARBON_WIN__DLDECL(XSetInputFocus);
int XSetInputFocus(Display *display, Window focus, int revert_to, Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XSetInputFocus);
  return XSetInputFocus_ptr(display, focus, revert_to, time);
}

CARBON_WIN__DLDECL(XConvertSelection);
int XConvertSelection(Display *display,
                      Atom selection,
                      Atom target,
                      Atom property,
                      Window requestor,
                      Time time) {
  CARBON_WIN__DLSYM(carbon_win__dl_X11, XConvertSelection);
  return XConvertSelection_ptr(display, selection, target, property, requestor, time);
}

// Local Variables:
// mode: c
// End:

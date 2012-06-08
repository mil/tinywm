/* TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <X11/Xlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;

    if(!(dpy = XOpenDisplay(0x0))) return 1;

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;
    for(;;)
    {
        XNextEvent(dpy, &ev);
        if(ev.type == KeyPress && ev.xkey.subwindow != None)
            XRaiseWindow(dpy, ev.xkey.subwindow);
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }
        else if(ev.type == MotionNotify && start.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
						int x, y;
						int width = attr.width, height = attr.height;

						if (start.button == 3) {
							if (attr.width + xdiff > 0) { width = attr.width + xdiff;
							} else if (attr.width + xdiff == 0) { width = 1;
							} else { width = (xdiff * -1) - attr.width;
								x = attr.x + xdiff + attr.width; }

							if (attr.height + ydiff > 0) { height = attr.height + ydiff;
							} else if (attr.height == 0) { height = 1;
							} else { height = (ydiff * -1) - attr.height; 
								y = attr.y + ydiff + attr.height; }
						} else {
							x = attr.x + xdiff;
							y = attr.y + ydiff;
						}
            XMoveResizeWindow(dpy, start.subwindow, x, y, width, height);
        }
        else if(ev.type == ButtonRelease)
        {
            start.subwindow = None;
        }
    }
}

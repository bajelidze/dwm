#include <X11/XF86keysym.h>
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 26;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 26;       /* vert inner gap between windows */
static const unsigned int gappoh    = 26;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 26;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int user_bh            = 28;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "Weather Icons:size=11", "Iosevka Nerd Font:size=11" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#1e1e1e";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#42A5F5";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_cyan,  col_gray1, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	// { "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	// { "Google-chrome", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
    { "st-256color", NULL,       NULL,       0,            0,           1,          0,         -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.49; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	// { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                      0, KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,          0, KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,            0, KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,0, KEY,      toggletag,      {.ui = 1 << TAG} },

#define KEYCODE(MODIFIER, KEY, FUNC, ARG) \
	{ MODIFIER, 0, KEY, FUNC, ARG }

#define KEYSYM(MODIFIER, KEY, FUNC, ARG) \
	{ MODIFIER, KEY, 0, FUNC, ARG }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "alacrittyy", "--working-directory", "$(xcwd)", NULL };
// static const char *termcmd[]  = { "stt", NULL };
static const char *upvol[]   = { "volumectl", "-i", "5", NULL };
static const char *downvol[] = { "volumectl", "-d", "5", NULL };
static const char *mutevol[] = { "volumectl", "-m",  NULL };
static const char *adevctl[] = { "adevctl", "-s",  NULL };
static const char *upbrit[]   = { "brightness", "-i", "10", NULL };
static const char *downbrit[] = { "brightness", "-d", "10", NULL };
// static const char *chrome[]  = { "google-chrome-stable", "--password-store=gnome", NULL };
static const char *chrome[]  = { "firefox", NULL };
// static const char *kswitch[]  = { "kswitch", NULL };
static const char *gnomecontrolcenter[]  = { "gnome-control-center", NULL };
static const char *lf[]  = { "alacritty", "-e", "yazi", NULL };
// static const char *lf[]  = { "stt", "-e", "yazi", NULL };
static const char *flameshot[]  = { "fs", "gui", NULL };
static const char *flameshotfull[]  = { "fs", "full", NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "alacritty", "-o", "window.dimensions.lines=34", "-o", "window.dimensions.columns=128", "-o", "window.title='scratchpad'", NULL };
// static const char *scratchpadcmd[] = { "st", "-g", "116x32", "-t", "scratchpad", NULL };


static const char *kswitchtous[]  = { "kswitchto", "us", NULL };
static const char *kswitchtode[]  = { "kswitchto", "de", NULL };
static const char *kswitchtoru[]  = { "kswitchto", "ru", NULL };
static const char *kswitchtoge[]  = { "kswitchto", "ge", NULL };

static const char *monitor[]  = { "monitor", NULL };
static const char *monitorr[]  = { "monitor", "-r", NULL };

#include "selfrestart.c"

// Find out keycodes using xev
static Key keys[] = {
	/* modifier                     key        function        argument */
	KEYCODE(MODKEY, 40, spawn, {.v = dmenucmd }),  // d
	KEYCODE(MODKEY, 36, spawn, {.v = termcmd }),   // Return
	KEYCODE(MODKEY, 56, togglebar, {0}),               // b
	KEYCODE(MODKEY, 44, focusstack, {.i = +1 }),        // j
	KEYCODE(MODKEY, 45, focusstack, {.i = -1 }),        // k
	// { MODKEY,                       31,        incnmaster,     {.i = +1 } },        // i
	// { MODKEY,                       30,        incnmaster,     {.i = -1 } },        // u
	KEYCODE(MODKEY, 43, setmfact, {.f = -0.05}),      // h
	KEYCODE(MODKEY, 46, setmfact, {.f = +0.05}),      // l
	KEYCODE(MODKEY, 39, zoom, {0}),               // s
    // { MODKEY,                       23,        view,           {0} },               // Tab
	KEYCODE(MODKEY, 24, killclient, {0}),               // q
	KEYCODE(MODKEY, 28, setlayout, {.v = &layouts[0]}),// t
	KEYCODE(MODKEY, 25, setlayout, {.v = &layouts[1]}),// w
	// { MODKEY,                       52,        setlayout,      {.v = &layouts[2]} },// z
	KEYCODE(MODKEY, 65, setlayout, {0}),               // Space
	KEYCODE(MODKEY|ShiftMask, 65, togglefloating, {0}),               // Space
	KEYCODE(MODKEY, 41, togglefullscr, {0}),               // f
	KEYCODE(MODKEY, 19, view, {.ui = ~0 }),       // 0
	KEYCODE(MODKEY|ShiftMask, 19, tag, {.ui = ~0 }),       // 0
	KEYCODE(MODKEY, 59, focusmon, {.i = -1 }),        // comma
	KEYCODE(MODKEY, 60, focusmon, {.i = +1 }),        // period
	KEYCODE(MODKEY, 23, focusmon, {.i = +1 }),        // Tab
	KEYCODE(MODKEY|ShiftMask, 23, tagmon, {.i = +1 }),        // Tab
	KEYCODE(MODKEY|ShiftMask, 59, tagmon, {.i = -1 }),        // comma
	KEYCODE(MODKEY|ShiftMask, 60, tagmon, {.i = +1 }),        // period
    KEYCODE(MODKEY|ShiftMask, 44, movestack, {.i = +1 }),        // j
	KEYCODE(MODKEY|ShiftMask, 45, movestack, {.i = -1 }),        // k
	TAGKEYS(                        10,                      0)                     // 1
	TAGKEYS(                        11,                      1)                     // 2
	TAGKEYS(                        12,                      2)                     // 3
	TAGKEYS(                        13,                      3)                     // 4
	TAGKEYS(                        14,                      4)                     // 5
	TAGKEYS(                        15,                      5)                     // 6
	TAGKEYS(                        16,                      6)                     // 7
	TAGKEYS(                        17,                      7)                     // 8
	TAGKEYS(                        18,                      8)                     // 9
	KEYCODE(MODKEY|ShiftMask, 32, quit, {0}),               // o
    KEYCODE(MODKEY|ShiftMask, 27, self_restart, {0}),               // r
    KEYCODE(MODKEY, 27, resetlayout, {0}),               // r
	KEYSYM(0, XF86XK_AudioLowerVolume, spawn, {.v = downvol }),
	KEYSYM(0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol }),
	KEYSYM(0, XF86XK_AudioMute, spawn, {.v = mutevol }),
    KEYCODE(MODKEY, 20, spawn, {.v = downvol }),   // -
	KEYCODE(MODKEY, 21, spawn, {.v = upvol }),     // =
	KEYCODE(MODKEY, 57, spawn, {.v = mutevol }),   // m
    KEYCODE(MODKEY, 33, spawn, {.v = adevctl }),   // -
    KEYCODE(MODKEY, 34, spawn, {.v = downbrit }),  // [
	KEYCODE(MODKEY, 35, spawn, {.v = upbrit }),    // ]
	KEYCODE(MODKEY, 54, spawn, {.v = chrome }),    // c
    // { Mod1Mask,                     50,        spawn,          {.v = kswitch } },   // Shift L
    // { ShiftMask,                    64,        spawn,          {.v = kswitch } },   // Alt L
    KEYCODE(MODKEY, 52, spawn, {.v = kswitchtous}),// z
    KEYCODE(MODKEY|ShiftMask, 52, spawn, {.v = kswitchtode}),// Shift+z
    KEYCODE(MODKEY, 53, spawn, {.v = kswitchtoru}),// x
    KEYCODE(MODKEY, 55, spawn, {.v = kswitchtoge}),// v
    KEYCODE(MODKEY, 58, spawn, {.v = monitor }),// m
    KEYCODE(MODKEY|ShiftMask, 58, spawn, {.v = monitorr }),// m
	KEYCODE(MODKEY, 42, spawn, {.v = gnomecontrolcenter }), // g
	KEYCODE(MODKEY, 26, spawn, {.v = lf }),        // e
	KEYCODE(0, 112, spawn, {.v = flameshot }), // PgUp
	KEYCODE(ShiftMask, 107, spawn, {.v = flameshotfull }), // PrtSc

    KEYCODE(MODKEY, 49, togglescratch, {.v = scratchpadcmd }), // `

	KEYCODE(MODKEY, 121, incrgaps, {.i = +1 }),		// F1
	KEYCODE(MODKEY, 122, incrgaps, {.i = -1 }),		// F2
	// { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	// { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	// { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	KEYCODE(MODKEY, 123, togglegaps, {0}),				// F3
	// { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	// { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	// { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	// { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	// { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	// { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	// { ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

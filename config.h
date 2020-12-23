/* See LICENSE file for copyright and license details. */

#define XBAR 128
#define YBAR 16

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 16;       /* gap pixel between windows */
static const unsigned int snap      = 8;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int xbar               = XBAR;       /* horizontal offset for statusbar */
static const int ybar               = YBAR;       /* vertical offset for statusbar */
static const int horizpadbar        = 0;        /* horizontal padding for statusbar */
static const int vertpadbar         = 12;        /* vertical padding for statusbar */
static const char *fonts[]          = { "JetBrains Mono:size=10:antialias=true:autohint=false" };
static const char dmenufont[]       = "JetBrains Mono:size=10:antialias=true:autohint=false";
static const char col_bg[]          = "#2b2b2b";
static const char col_fg[]          = "#bbbbbb";
static const char col_hl[]          = "#cc7832";
static const char col_lt[]          = "#323232";
static const char *colors[][3]      = {
	/*               fg        bg      border   */
	[SchemeNorm]   = { col_fg, col_bg, col_bg }, /* 1: normal */
	[SchemeSelTag] = { col_bg, col_hl, col_bg }, /* 2: selected tag*/
	[SchemeSelWin] = { col_hl, col_bg, col_hl }, /* 3: selected window */
	[SchemeLight]  = { col_fg, col_lt, col_bg }, /* 4: light */
	[4]  = { col_hl, col_lt, col_bg }, /* 5: light */
	[5]  = { col_fg, col_lt, col_bg }, /* 6: light */
};

/* tagging */
static const char *tags[] = { "one", "two", "three", "four", "five", "six" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
    { "discord",  NULL,       NULL,       1 << 1,       0,           1 },
    { "Spotify",  NULL,       NULL,       1,            0,           1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont,
                                  "-nb", col_bg, "-nf", col_fg, "-sb", col_hl, "-sf", col_bg,
                                  "-nhb", col_bg, "-nhf", col_hl, "-shb", col_hl, "-shf", col_bg,
                                  "-h", "30", "-x", STR(XBAR), "-y", STR(YBAR), "-w", "824", NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_period, setcfact,       {.f = +0.25} },
	{ MODKEY,                       XK_comma,  setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_f,      zoom,           {0} },
	{ MODKEY,                       XK_grave,  view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_z,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_x,      focusmon,       {.i = +1 } },
	{ MODKEY,                       XK_Tab,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_z,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_x,      tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_Escape, quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


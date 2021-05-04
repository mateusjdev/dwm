/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 3;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 24;       /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "RobotoMono:size=10:antialias=true:autohint=true",
					"JoyPixels:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "RobotoMono:size=10:antialias=true:autohint=true"; 

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_red2[]        = "#cc0022";
static const char col_red[]         = "#a0182f";
static const char col_cyan[]        = "#005577";
static const char col_white[]        = "#ffffff";

static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_white, col_red, col_red2 },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, OPAQUE, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4" };


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      ltile },    /* first entry is default */
	{ "=[]",      rtile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
};

static const int monDefaultLayout[] = {
	0,
	1,
};

/* key definitions */
#define MODKEY Mod1Mask
#define SUPERKEY Mod4Mask

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *cmd_launcher[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *cmd_terminal[]  = { "termite", NULL };
static const char *cmd_printscreen[]  = { "scrot","%Y-%m-%d_%H-%M-%S.jpg","-q100","-z","-e","mkdir --parents ~/images/screenshots","-e","mv $f ~/images/screenshots/", NULL };
static const char *cmd_printedit[]  = { "scrot","%Y-%m-%d_%H-%M-%S.jpg","-q100","-z","-s","-f","-e","mkdir --parents ~/images/screenshots","-e","mv $f ~/images/screenshots/", NULL};

static const char *cmd_fileexplorer[]  = { "termite", "-e", "lf", NULL };
static const char *cmd_htop[]  = { "termite", "-e", "htop", NULL };

static const char *mutecmd[] = { "amixer", "-q", "set", "Master", "toggle", NULL };
static const char *volupcmd[] = { "amixer", "-q", "set", "Master", "5%+", "unmute", NULL };
static const char *voldowncmd[] = { "amixer", "-q", "set", "Master", "5%-", "unmute", NULL };

static Key keys[] = {
        /* modifier                     key                             function        argument */
        { SUPERKEY,                     XK_r,                           spawn,          {.v = cmd_launcher } },
	{ SUPERKEY,                     XK_t,                           spawn,          {.v = cmd_terminal } },
	{ SUPERKEY,                     XK_e,                           spawn,          {.v = cmd_fileexplorer } },
	{ ShiftMask|ControlMask,        XK_Escape,                      spawn,          {.v = cmd_htop } },
	{ 0,                            XF86XK_AudioMute,               spawn,          {.v = mutecmd } },
	{ 0,                            XF86XK_AudioLowerVolume,        spawn,          {.v = voldowncmd } },
	{ 0,                            XF86XK_AudioRaiseVolume,        spawn,          {.v = volupcmd } },
	{ MODKEY,                       XK_b,                           togglebar,      {0} },
	{ MODKEY,                       XK_j,                           focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                           focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                           incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                           incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                           setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                           setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,                      zoom,           {0} },
	{ MODKEY,                       XK_Tab,                         view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                           killclient,     {0} },
	{ MODKEY,                       XK_t,                           setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,                           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,                           setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,                           setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_u,      			setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_o,      			setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,                       setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                       togglefloating, {0} },
	{ MODKEY,                       XK_0,                           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                           tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                       focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                       tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                      tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  			setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  			setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  			setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                                           0)
	TAGKEYS(                        XK_2,                                           1)
	TAGKEYS(                        XK_3,                                           2)
	TAGKEYS(                        XK_4,                                           3)
	{ MODKEY|ShiftMask,             XK_q,                           quit,           {0} },
	{0,                             XK_Print,                       spawn,          {.v = cmd_printscreen } },
	{ControlMask,                   XK_Print,                       spawn,          {.v = cmd_printedit } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = cmd_terminal } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


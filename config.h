/* See LICENSE file for copyright and license details. */

/* appearance */
/* Custom font at beginning for dwmstatus output */
static const char fonts[][150]            = {
    "-*-stlarch-medium-r-*-*-10-*-*-*-*-*-*-*" "," "-*-profont-medium-*-*-*-12-*-*-*-*-*-*-*",
    "-*-stlarch-medium-r-*-*-10-*-*-*-*-*-*-*" "," "-*-terminus-medium-r-*-*-12-*-*-*-*-*-*-*",
    "-*-stlarch-medium-r-*-*-10-*-*-*-*-*-*-*" "," "-*-terminus-medium-r-*-*-13-*-*-*-*-*-*-*",
};
static       char font[sizeof(fonts[0])]  = "";
static const char normbordercolor[]       = "#444444";
static const char normbgcolor[]           = "#222222";
static const char normfgcolor[]           = "#bbbbbb";
static const char selbordercolor[]        = "#005577";
static const char selbgcolor[]            = "#005577";
static const char selfgcolor[]            = "#eeeeee";

static const unsigned int borderpx        = 1;        /* border pixel of windows */
static const unsigned int snap            = 32;       /* snap pixel */
static const Bool showbar                 = True;     /* False means no bar */
static const Bool showbbar                = False;    /* False means no bar */

/* False means using the scroll wheel on a window will not change focus */
static const Bool focusonwheelscroll = False;

static const unsigned int   bar_horizontal_width  = 3;   /* number of characters */
static const unsigned int   bar_horizontal_height = 4;   /* in pixels */
static const unsigned int   bar_vertical_width    = 4;   /* in pixels */
static const          float bar_vertical_height   = 0.8; /* as a percentage of bar height */
static const unsigned int   bar_padding           = 4;   /* in pixels, should be even (left to right padding) */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class          , instance              , title               , tags mask , isfloating , monitor */
	{ "Pithos"        , NULL                  , NULL                , 1 << 8    , False      , 0 } ,
	{ "CvbApp"        , "cvPopupWindow_popup" , NULL                , 0         , True       , 0 } , /* For hdl_designer */
	{ "TopLevelShell" , "hdldesigner"         , "Object Properties" , 0         , True       , 0 } , /* For hdl_designer */
	{ "Firefox"       , "Popup"               , NULL                , 0         , True       , 0 } ,
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = False; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "\uE002",   tile },
	{ "\uE001",   NULL },
	{ "[M]",      monocle },
	{ "\uE003",   bstack },
	{ "\uE004",   bstackhoriz },
	{ NULL ,      NULL },
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

/* commands */
/* dmenu-ish stuff */
static char dmenumon[2]                          = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]                    = { "dmenu_run", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *calccmd[]                     = { "dmenu_calc", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *passcmd[]                     = { "/home/jeffrey/bin/dmenu_pass", "-i", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *srcmd[]                       = { "/home/jeffrey/bin/dmenu_sr", "-i", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *raisecmd[]                    = { "dmenu_raise", "-i", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *firefoxcmd[]                  = { "/home/jeffrey/bin/dmenu_firefox", "-i", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *abducosessioncmd[]            = { "/home/jeffrey/bin/dmenu_abduco", "-i", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *menucmd[]                     = { "dmenu_menu", "-i", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor,"-sb", selbgcolor, "-sf", selfgcolor, NULL };
/* Spawn clients                                 = */
static const char *termcmd[]                     = { "st-dvtm", NULL };
/* Increase/decrease screen resolution           = */
static const char *nextmodecmd[]                 = { "/home/jeffrey/bin/Xzoom_notify", "+", NULL };
static const char *prevmodecmd[]                 = { "/home/jeffrey/bin/Xzoom_notify", "-", NULL };
static const char *defaultmodecmd[]              = { "/home/jeffrey/bin/Xzoom_notify", "0", NULL };
/* Adjust screen atributes                       = */
static const char *redshifttogglecmd[]           = { "/home/jeffrey/bin/redshift-toggle", NULL };
static const char *autobrightnesscmd[]           = { "/home/jeffrey/bin/autobrightness", NULL };

void self_restart(const Arg *arg) {
    char * const argv[] = {"/usr/bin/dwm", NULL};
    execv(argv[0], argv);
}

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                      , key                     , function       , argument */
	{ MODKEY                         , XK_space                , spawn          , {.v = dmenucmd } }           ,
	{ MODKEY                         , XK_a                    , spawn          , {.v = passcmd } }            ,
	{ MODKEY                         , XK_c                    , spawn          , {.v = calccmd } }            ,
	{ MODKEY                         , XK_s                    , spawn          , {.v = srcmd } }              ,
	{ MODKEY                         , XK_w                    , spawn          , {.v = raisecmd } }           ,
	{ MODKEY                         , XK_x                    , spawn          , {.v = firefoxcmd } }         ,
	{ MODKEY                         , XK_z                    , spawn          , {.v = abducosessioncmd } }   ,
	{ MODKEY                         , XK_o                    , spawn          , {.v = menucmd } }            ,

	/* Find keys with xev */
	{ 0                              , XF86XK_Calculator       , spawn          , {.v = autobrightnesscmd } }  , // calculator key
	{ ControlMask                    , XF86XK_Calculator       , spawn          , {.v = redshifttogglecmd } }  , // ctrl+calculator key

	{ Mod4Mask                       , XK_KP_Add               , spawn          , {.v = nextmodecmd } }        ,
	{ Mod4Mask                       , XK_KP_Subtract          , spawn          , {.v = prevmodecmd } }        ,
	{ Mod4Mask                       , XK_KP_Multiply          , spawn          , {.v = defaultmodecmd } }     ,
	{ Mod4Mask|ControlMask           , XK_KP_Add               , font_next      , {.i = +1 } }                 ,
	{ Mod4Mask|ControlMask           , XK_KP_Subtract          , font_next      , {.i = -1 } }                 ,


	{ MODKEY                         , XK_b                    , togglebar      , {.i = 0} }                   ,
	{ MODKEY                         , XK_v                    , togglebar      , {.i = 1} }                   ,
	{ MODKEY                         , XK_j                    , focusstack     , {.i = +1 } }                 ,
	{ Mod1Mask                       , XK_Tab                  , focusstack     , {.i = +1} }                  ,
	{ MODKEY                         , XK_k                    , focusstack     , {.i = -1 } }                 ,
	{ Mod1Mask|ShiftMask             , XK_Tab                  , focusstack     , {.i = -1} }                  ,
	{ MODKEY|ControlMask             , XK_j                    , pushdown       , {0} }                        ,
	{ MODKEY|ControlMask             , XK_k                    , pushup         , {0} }                        ,
	{ MODKEY                         , XK_i                    , incnmaster     , {.i = +1 } }                 ,
	{ MODKEY                         , XK_d                    , incnmaster     , {.i = -1 } }                 ,
	{ MODKEY                         , XK_h                    , setmfact       , {.f = -0.05} }               ,
	{ MODKEY                         , XK_l                    , setmfact       , {.f = +0.05} }               ,
	{ MODKEY|ShiftMask               , XK_h                    , setcfact       , {.f = +0.25} }               ,
	{ MODKEY|ShiftMask               , XK_l                    , setcfact       , {.f = -0.25} }               ,
	{ MODKEY|ShiftMask               , XK_o                    , setcfact       , {.f =  0.00} }               ,
	{ MODKEY                         , XK_Return               , zoom           , {0} }                        ,
	{ MODKEY                         , XK_Tab                  , view           , {0} }                        ,
	{ MODKEY|ShiftMask               , XK_c                    , killclient     , {0} }                        ,
	{ MODKEY                         , XK_t                    , setlayout      , {.v = &layouts[0]} }         ,
	{ MODKEY                         , XK_f                    , setlayout      , {.v = &layouts[1]} }         ,
	{ MODKEY                         , XK_m                    , setlayout      , {.v = &layouts[2]} }         ,
	{ MODKEY                         , XK_r                    , setlayout      , {.v = &layouts[3]} }         ,
	{ MODKEY                         , XK_e                    , setlayout      , {.v = &layouts[4]} }         ,
	{ MODKEY|ShiftMask               , XK_space                , togglefloating , {0} }                        ,
	{ MODKEY                         , XK_0                    , view           , {.ui = ~0 } }                ,
	{ MODKEY|ShiftMask               , XK_0                    , tag            , {.ui = ~0 } }                ,
	{ MODKEY                         , XK_comma                , focusmon       , {.i = -1 } }                 ,
	{ MODKEY                         , XK_period               , focusmon       , {.i = +1 } }                 ,
	{ MODKEY|ShiftMask               , XK_comma                , tagmon         , {.i = -1 } }                 ,
	{ MODKEY|ShiftMask               , XK_period               , tagmon         , {.i = +1 } }                 ,
	{ MODKEY|ControlMask|ShiftMask   , XK_q                    , quit           , {0} }                        ,
	{ MODKEY|ControlMask|ShiftMask   , XK_r                    , self_restart   , {0} }                        ,
	TAGKEYS(  /*                     , */ XK_1                 , 0)
	TAGKEYS(  /*                     , */ XK_2                 , 1)
	TAGKEYS(  /*                     , */ XK_3                 , 2)
	TAGKEYS(  /*                     , */ XK_4                 , 3)
	TAGKEYS(  /*                     , */ XK_5                 , 4)
	TAGKEYS(  /*                     , */ XK_6                 , 5)
	TAGKEYS(  /*                     , */ XK_7                 , 6)
	TAGKEYS(  /*                     , */ XK_8                 , 7)
	TAGKEYS(  /*                     , */ XK_9                 , 8)
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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


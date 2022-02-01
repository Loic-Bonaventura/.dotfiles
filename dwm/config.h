/* Includes */
#include <X11/XF86keysym.h>

/* Contants */
#define TERMINAL "st"
#define TABBEDTERMINAL "tabbed -r 2 st -w ''"
#define EMACS "emacsclient -c -a 'emacs' "

/* appearance */
static const unsigned int borderpx       = 1;   /* border pixel of windows */
static const int corner_radius           = 0;
static const unsigned int snap           = 20;  /* snap pixel */
static const unsigned int gappiv         = 5;  /* vert inner gap between windows */
static const unsigned int gappih         = 5;  /* horiz inner gap between windows */
static const unsigned int gappoh         = 5;  /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 5;  /* vert outer gap between windows and screen edge */
static const int smartgaps_fact          = 1;  /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
static const int swallowfloating         = 1;
static const char autostartblocksh[]     = "autostart_blocking.sh";
static const char autostartsh[]          = "autostart.sh";
static const char dwmdir[]               = "dwm";
static const char localshare[]           = ".local/share";
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
static const int bar_height              = 26;   /* 0 means derive from font, >= 1 explicit height */
/* Status is to be shown on: -1 (all monitors), 0 (a specific monitor by index), 'A' (active monitor) */
static const int statusmon               = -1;

/* Indicators: see patch/bar_indicators.h for options */

static int tagindicatortype              = INDICATOR_TOP_LEFT_SQUARE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;
static const char *fonts[]               = { "MesloLGS Nerd Font:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]            = "MesloLGS Nerd Font:size=10";

static char c000000[]                    = "#000000"; // placeholder value

static char normfgcolor[]                = "#FF6C6B";
static char normbgcolor[]                = "#282C34";
static char normbordercolor[]            = "#1E2127";
static char normfloatcolor[]             = "#1E2127";

static char selfgcolor[]                 = "#eeeeee";
static char selbgcolor[]                 = "#282C34";
static char selbordercolor[]             = "#51AFEF";
static char selfloatcolor[]              = "#51AFEF";

static char titlenormfgcolor[]           = "#51AFEF";
static char titlenormbgcolor[]           = "#282C34";
static char titlenormbordercolor[]       = "#1E2127";
static char titlenormfloatcolor[]        = "#db8fd9";

static char titleselfgcolor[]            = "#ECBE7B";
static char titleselbgcolor[]            = "#282C34";
static char titleselbordercolor[]        = "#005577";
static char titleselfloatcolor[]         = "#005577";

static char tagsnormfgcolor[]            = "#51AFEF";
static char tagsnormbgcolor[]            = "#282C34";
static char tagsnormbordercolor[]        = "#444444";
static char tagsnormfloatcolor[]         = "#db8fd9";

static char tagsselfgcolor[]             = "#ECBE7B";
static char tagsselbgcolor[]             = "#282C34";
static char tagsselbordercolor[]         = "#005577";
static char tagsselfloatcolor[]          = "#005577";

static char hidnormfgcolor[]             = "#005577";
static char hidselfgcolor[]              = "#227799";
static char hidnormbgcolor[]             = "#222222";
static char hidselbgcolor[]              = "#222222";

static char urgfgcolor[]                 = "#FF6C6B";
static char urgbgcolor[]                 = "#282C34";
static char urgbordercolor[]             = "#FF6C6B";
static char urgfloatcolor[]              = "#db8fd9";


static const unsigned int baralpha = 245;
/* static const unsigned int baralpha = 255; */
static const unsigned int borderalpha = 245;
static const unsigned int alphas[][3] = {
	/*                       fg      bg        border     */
	[SchemeNorm]         = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]          = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleNorm]    = { OPAQUE, baralpha, borderalpha },
	[SchemeTitleSel]     = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsNorm]     = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsSel]      = { OPAQUE, baralpha, borderalpha },
	[SchemeHidNorm]      = { OPAQUE, baralpha, borderalpha },
	[SchemeHidSel]       = { OPAQUE, baralpha, borderalpha },
	[SchemeUrg]          = { OPAQUE, baralpha, borderalpha },
};

static char *colors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm]         = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor },
	[SchemeSel]          = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor },
	[SchemeTitleNorm]    = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
	[SchemeTitleSel]     = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor },
	[SchemeTagsNorm]     = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor },
	[SchemeTagsSel]      = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor },
	[SchemeHidNorm]      = { hidnormfgcolor,   hidnormbgcolor,   c000000,              c000000 },
	[SchemeHidSel]       = { hidselfgcolor,    hidselbgcolor,    c000000,              c000000 },
	[SchemeUrg]          = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor },
};




const char *spcmd1[] = {"st", "-n", "spterm", "-g", "90x30", NULL };
static Sp scratchpads[] = {
   /* name          cmd  */
   {"spterm",      spcmd1},
};

/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char *tagicons[][NUMTAGS] = {
	[DEFAULT_TAGS]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9" },
	[ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
	[ALT_TAGS_DECORATION] = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};


/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
				static const Rule rules[] = {
					/* xprop(1):
					 *	WM_CLASS(STRING) = instance, class
					 *	WM_NAME(STRING) = title
					 *	WM_WINDOW_ROLE(STRING) = role
					 *	_NET_WM_WINDOW_TYPE(ATOM) = wintype
					 */
					RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
					RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
					RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
					RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
					/* RULE(.class = "Gimp", .tags = 1 << 4) */
					/* RULE(.class = "Firefox", .tags = 1 << 7) */
					RULE(.class = "1Password", .isfloating = 1)
					RULE(.class = "Pavucontrol", .isfloating = 1)
					RULE(.class = "St", .isfloating = 0, .isterminal = 1, .noswallow = 0)
					RULE(.instance = "spterm", .tags = SPTAG(0), .isfloating = 1)
				};

				/* static const Rule rules[] = { */
				/*  	 /\* WM_CLASS(STRING) = instance, class *\/ */
				/*  	 /\* WM_NAME(STRING) = title *\/ */
				/* 	/\* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor *\/ */
				/* 	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 }, */
				/* 	{ NULL,      "spterm", NULL,           SPTAG(0),  1,          0,           0,        -1 }, /\* xev *\/ */
				/* 	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /\* xev *\/ */
				/*  }; */


/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor   bar    alignment         widthfunc                drawfunc                clickfunc                name */
	{ -1,        0,     BAR_ALIGN_LEFT,   width_tags,              draw_tags,              click_tags,              "tags" },
	{ -1,        0,     BAR_ALIGN_LEFT,   width_ltsymbol,          draw_ltsymbol,          click_ltsymbol,          "layout" },
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_statuscolors,      draw_statuscolors,      click_statuscolors,      "statuscolors" },
	{ -1,        0,     BAR_ALIGN_NONE,   width_wintitle,          draw_wintitle,          click_wintitle,          "wintitle" },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */



static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ "(@)",      spiral },
	{ "HHH",      grid },
	{ NULL,       NULL },
};


/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_s,     ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_w,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_e,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = -1 } },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
/* static char dmenumon[2] = "0"; /\* component of dmenucmd, manipulated in spawn() *\/ */
/* static const char *dmenucmd[] = { */
/* 	"dmenu_run", */
/* 	"-m", dmenumon, */
/* 	"-fn", dmenufont, */
/* 	"-nb", normbgcolor, */
/* 	"-nf", normfgcolor, */
/* 	"-sb", selbgcolor, */
/* 	"-sf", selfgcolor, */
/* 	topbar ? NULL : "-b", */
/* 	NULL */
/* }; */
static const char *termcmd[]        = { TERMINAL, NULL };
static const char *tabbedtermcmd[]  = { "tabbed", "-r", "2", "st", "-w", "''", NULL };
static const char *retrotermcmd[]   = { "cool-retro-term", NULL };
static const char *chromiumcmd[]       = { "chromium", NULL };
static const char *discordcmd[]     = { "discord", NULL };
static const char *slackcmd[]       = { "slack", NULL };
static const char *fmanagercmd[]    = { "pcmanfm", NULL };
static const char *onepasswordcmd[] = { "1password", NULL };
static const char *codecmd[]        = { "codium", NULL };
static const char *signalcmd[]      = { "signal-desktop", NULL };
static const char *brightup[]       = { "/usr/bin/xbacklight", "-inc", "10", NULL };
static const char *brightdown[]     = { "/usr/bin/xbacklight", "-dec", "10", NULL };



static Key keys[] = {
	/* modifier                     key            function                argument */
    { MODKEY,                       XK_r,          spawn,                  SHCMD(TERMINAL " -e ranger") },
	{ MODKEY|ShiftMask,             XK_r,          spawn,                  SHCMD(TERMINAL " -e htop") },
	{ MODKEY,                       XK_z,          spawn,                  SHCMD(TERMINAL " -e alsamixer") },
	{ MODKEY|ShiftMask,             XK_n,          spawn,                  SHCMD(TERMINAL " nmtui") },
    { MODKEY,                       XK_p,          spawn,                  SHCMD("dmenu_run -h 26") },
	{ MODKEY|ControlMask,           XK_d,          spawn,                  SHCMD("chromium drive.protonmail.com") },
	{ MODKEY|ShiftMask,             XK_d,          spawn,                  SHCMD("chromium calendar.protonmail.com") },
	{ MODKEY,                       XK_m,          spawn,                  SHCMD("chromium mail.protonmail.com") },
	{ MODKEY,                       XK_y,          spawn,                  SHCMD("chromium youtube.com") },
    { MODKEY,                       XK_e,          spawn,                  SHCMD(EMACS) },
	{ NULL,                         XK_Print,      spawn,                  SHCMD("flameshot gui") },
	{ MODKEY,                       XK_v,          spawn,                  SHCMD(TERMINAL " -e nvim") },
	{ MODKEY|ShiftMask,             XK_p,          spawn,                  {.v = onepasswordcmd } },
	{ MODKEY|ShiftMask,             XK_g,          spawn,                  {.v = signalcmd } },
	{ MODKEY|ControlMask,           XK_l,          spawn,                  SHCMD("blur-lock") },
	{ Mod1Mask,                     XK_Return,     spawn,                  {.v = tabbedtermcmd } },
	{ MODKEY,                       XK_Return,     spawn,                  {.v = termcmd } },
	{ MODKEY,                       XK_exclam,     spawn,                  {.v = retrotermcmd } },
	{ MODKEY,                       XK_w,          spawn,                  {.v = chromiumcmd } },
	{ MODKEY,                       XK_q,          spawn,                  {.v = discordcmd } },
	{ MODKEY,                       XK_a,          spawn,                  {.v = slackcmd } },
	{ MODKEY,                       XK_n,          spawn,                  {.v = fmanagercmd } },
	{ MODKEY|ShiftMask,             XK_v,          spawn,                  {.v = codecmd } },
    { MODKEY,                       XK_b,          togglebar,              {0} },
	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)
	{ MODKEY,                       XK_Left,       focusdir,               {.i = 0 } }, // left
	{ MODKEY,                       XK_Right,      focusdir,               {.i = 1 } }, // right
	{ MODKEY,                       XK_Up,         focusdir,               {.i = 2 } }, // up
	{ MODKEY,                       XK_Down,       focusdir,               {.i = 3 } }, // down
	{ MODKEY,                       XK_i,          incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,             {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,               {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,               {.f = +0.05} },
	{ MODKEY,                       XK_x,          transfer,               {0} },
	{ MODKEY|ShiftMask,             XK_Return,     zoom,                   {0} },
	{ MODKEY|Mod4Mask,              XK_u,          incrgaps,               {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,          incrgaps,               {.i = -1 } },
	{ MODKEY|ControlMask,           XK_i,          incrigaps,              {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_i,          incrigaps,              {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,          incrogaps,              {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,          incrogaps,              {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_6,          incrihgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_6,          incrihgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_7,          incrivgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_7,          incrivgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_8,          incrohgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_8,          incrohgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_9,          incrovgaps,             {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_9,          incrovgaps,             {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_agrave,     togglegaps,             {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_agrave,     defaultgaps,            {0} },
	{ MODKEY,                       XK_Tab,        view,                   {0} },
	{ MODKEY,                       XK_c,          killclient,             {0} },
	{ MODKEY|ShiftMask,             XK_q,          quit,                   {0} },
	{ MODKEY,                       XK_t,          setlayout,              {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,          setlayout,              {.v = &layouts[1]} },
	{ MODKEY,                       XK_ugrave,     setlayout,              {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_ugrave,     setlayout,              {.v = &layouts[3]} },
	{ MODKEY,                       XK_s,          setlayout,              {.v = &layouts[4]} },
	{ MODKEY,                       XK_g,          setlayout,              {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,      setlayout,              {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating,         {0} },
	{ MODKEY|ShiftMask,             XK_f,          fullscreen,             {0} },
	{ MODKEY|ControlMask,           XK_agrave,     view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_parenright, tag,                    {.ui = ~0 } },
	{ MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_semicolon,  focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_semicolon,  tagmon,                 {.i = +1 } },
    { NULL,                 XF86XK_MonBrightnessUp,    spawn,            {.v = brightup } },
    { NULL,                 XF86XK_MonBrightnessDown,  spawn,            {.v = brightdown } },
	{ MODKEY,                       XK_colon,      togglescratch,          {.ui = 0 } },
	{ MODKEY|ControlMask,           XK_colon,      setscratch,             {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_colon,      removescratch,          {.ui = 0 } },
	{ MODKEY|ShiftMask,             XK_s,          togglesticky,           {0} },
	{ MODKEY,                       XK_0,          view,                   {.ui = ~SPTAGMASK } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~SPTAGMASK } },
	{ MODKEY|ControlMask,           XK_comma,      cyclelayout,            {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,     cyclelayout,            {.i = +1 } },
	TAGKEYS(                        XK_ampersand,              0)
    TAGKEYS(                        XK_eacute,                 1)
    TAGKEYS(                        XK_quotedbl,               2)
    TAGKEYS(                        XK_apostrophe,             3)
    TAGKEYS(                        XK_parenleft,              4)
    TAGKEYS(                        XK_minus,                  5)
    TAGKEYS(                        XK_egrave,                 6)
    TAGKEYS(                        XK_underscore,             7)
    TAGKEYS(                        XK_ccedilla,               8)
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};
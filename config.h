#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass = "Polybar";     /* Alternate bar class name */
static const char *altbarcmd  = "$HOME/bar.sh"; /* Alternate bar launch command */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "SauceCodePro Nerd Font Mono:size=12" };
static const char dmenufont[]       = "SauceCodePro Nerd Font Mono:size=15";
static const char col_gray1[]       = "#2E3440";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#98c9cf";
static const char col_gray4[]       = "#aeeef5";
static const char col_cyan[]        = "#495263";
static const char col_border[]      = "#eb172a";
static const unsigned int baralpha = 0xd0;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};

/* tagging */
// static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };
static const char *tags[] = { "₁", "₂", "₃", "₄", "💕₅", "₆", "💬₇", "₈", "₉" };
// static const char *tags[] = { "\uf120", "\uf7ae", "\uf121", "\uf04b", "\ue62e", "\uf251", "\ue727", "\uf537", "\uf684" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                    instance    title       tags mask     isfloating   monitor */
	// { "firefox",                NULL,       NULL,       1 << 8,       0,           -1 },
	// { "chrome",                 NULL,       NULL,       1 << 1,       0,           -1 },
	{ "uTools",                 NULL,       NULL,       0,            1,           -1 },
	{ "HevoNext.B2CApp",                 NULL,       NULL,       0,            1,           -1 },
	{ "fcitx",                  NULL,       NULL,       0,            1,           -1 },
	{ "idea",                   NULL,       NULL,       1 << 7,       0,           -1 },
	{ "netease-cloud-music",    NULL,       NULL,       1 << 5,       0,           -1 },
	{ "wemeetapp.exe",          NULL,       NULL,       1 << 6,       1,           -1 },
	{ "Typora",                 NULL,       NULL,       1 << 3,       0,           -1 },
	// { "icalingua",              NULL,       NULL,       1 << 6,       0,           -1 },
	// { "wechat.exe",             NULL,       NULL,       1 << 6,       0,           -1 },
	// { "tim.exe",                NULL,       NULL,       1 << 6,       0,           -1 },
	// { "Wine",                   NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
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
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *trayercmd[]  = { "/home/bzm/scripts/trayer-change.sh", NULL };
static const char *conkycmd[]  = { "/home/bzm/scripts/conky-change.sh", NULL };
static const char *openwechatcmd[]  = { "/home/bzm/scripts/open-wechat.sh", NULL };
static const char *chromecmd[]  = { "/home/bzm/scripts/open-chrome.sh",NULL };
static const char *gitkrakencmd[]  = { "gitkraken","--force-device-scale-factor=1.15",NULL };
static const char *ideacmd[]  = { "/usr/local/bin/idea", NULL };
static const char *typoracmd[]  = { "typora", NULL };
static const char *utoolscmd[]  = { "utools", NULL };
static const char *musiccmd[]  = { "netease-cloud-music", NULL };
static const char *timcmd[]  = { "/opt/deepinwine/tools/sendkeys.sh", "z" , "tim" , "4" };

static const char *upvol[]   = { "/home/bzm/scripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/bzm/scripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/bzm/scripts/vol-toggle.sh",  NULL };
static const char *picomcmd[] = { "/home/bzm/scripts/picom-up.sh",  NULL };
static const char *fcitxcmd[] = { "/home/bzm/scripts/fcitx-up.sh",  NULL };


static const char *wpcmd[]  = { "/home/bzm/scripts/wp-change.sh", NULL };
static const char *lockcmd[]  = { "/home/bzm/scripts/lock_screen.sh", NULL };
static const char *sktogglecmd[]  = { "/home/bzm/scripts/sck-tog.sh", NULL };
static const char *nautilustogcmd[]  = { "/home/bzm/scripts/nautilus-tog.sh", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };

static const char *setqwertycmd[]  = { "/home/bzm/scripts/setxmodmap-qwerty.sh", NULL };

static const char *screenlockcmd[]  = { "/home/bzm/scripts/screenlock.sh", NULL};

static const char *incbacklightcmd[]  = { "/home/bzm/scripts/inc-backlight.sh", NULL};
static const char *decbacklightcmd[]  = { "/home/bzm/scripts/dec-backlight.sh", NULL};

static const char *screenshotcmd[] = { "flameshot", "gui", NULL };
static const char *screenshotfullcmd[] = { "flameshot", "full", "-c",NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	/*--------------------------------------------------------------------------------------------------------------*/
	/*打开软件*/
	/*--------------------------------------------------------------------------------------------------------------*/
	{ MODKEY,              XK_s,                    spawn,          {.v = dmenucmd } },		/*打开dmenu */
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },		/* 打开终端 */
	{ MODKEY,              XK_w,                    spawn,          {.v = trayercmd } },		/*打开托盘*/
	{ MODKEY,              XK_c,                    spawn,          {.v = conkycmd } },		/* 打开系统监控 */

	{ MODKEY|ControlMask,  XK_c,                    spawn,          {.v = chromecmd } },	/* 打开chrome*/
	{ MODKEY|ControlMask,  XK_k,                    spawn,          {.v = gitkrakencmd } },	/* 打开chrome*/
	{ MODKEY|ControlMask,  XK_i,                    spawn,          {.v = ideacmd } },	/* 打开idea*/
	{ MODKEY|ControlMask,  XK_t,                    spawn,          {.v = typoracmd } },	/* 打开typora*/
	{ MODKEY|ControlMask,  XK_u,                    spawn,          {.v = utoolscmd } },	/* 打开utools*/
	// { MODKEY|ControlMask,  XK_m,                    spawn,          {.v = musiccmd } },	  [> 打开netease-cloud-music<]
	{ MODKEY|ControlMask,  XK_m,                    spawn,          {.v = timcmd } },	  /* 打开tim*/
	{ MODKEY|ControlMask,  XK_n,                    spawn,          {.v = openwechatcmd } },	  /* 打开微信*/
	{ MODKEY,              XK_Print,                spawn,          {.v = screenshotcmd } },	/*截屏(gui)*/
	{ ControlMask,         XK_Print,                spawn,          {.v = screenshotfullcmd } },	/*截屏(全屏)*/
	{ MODKEY|ControlMask,  XK_s,                    spawn,          {.v = sktogglecmd } },	/* 键盘回显 */
	{ MODKEY|ControlMask,  XK_f,                    spawn,          {.v = nautilustogcmd } },	/*打开文件管理器*/
	{ MODKEY|ControlMask,  XK_l,                    spawn,          {.v = fcitxcmd } },	/*输入法*/

	{ MODKEY|ShiftMask,    XK_w,                    spawn,          {.v = setqwertycmd } },
	// { MODKEY|ControlMask,  XK_l,                    spawn,          {.v = screenlockcmd } },
	/*--------------------------------------------------------------------------------------------------------------*/
	/*系统设置*/
	/*--------------------------------------------------------------------------------------------------------------*/
	{ 0,                   XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
	{ 0,                   XF86XK_AudioMute,        spawn,          {.v = mutevol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
	{ MODKEY,              XK_bracketleft,          spawn,          {.v = downvol } },	/*减小声音*/
	{ MODKEY,              XK_backslash,            spawn,          {.v = mutevol } },	/*开关声音*/
	{ MODKEY,              XK_bracketright,         spawn,          {.v = upvol   } },	/*增加声音*/

	{ MODKEY,              XK_b,                    spawn,          {.v = wpcmd } },	/* 切换壁纸 */
	{ MODKEY,              XK_l,                    spawn,          {.v = lockcmd } },	/* 锁屏 */

	{ MODKEY,              XK_comma,/*,*/      		spawn,   		{.v = decbacklightcmd } }, /*降低亮度*/
	{ MODKEY,              XK_period,/*.*/     		spawn,   		{.v = incbacklightcmd } },	/*增加亮度*/
	{ MODKEY,              XK_p,               		spawn,   		{.v = picomcmd } },

	// { MODKEY|ShiftMask,    XK_e,                    rotatestack,    {.i = +1 } },
	// { MODKEY|ShiftMask,    XK_u,                    rotatestack,    {.i = -1 } },


	/*--------------------------------------------------------------------------------------------------------------*/
	/*窗口操作*/
	/*--------------------------------------------------------------------------------------------------------------*/
	{ MODKEY,              XK_k,                    focusstack,     {.i = +1 } },	/*跳转窗口*/
	{ MODKEY,              XK_j,                    focusstack,     {.i = -1 } },

	{ MODKEY,              XK_n,                    viewtoleft,     {0} },	/*切换显示标签*/
	{ MODKEY,              XK_i,                    viewtoright,    {0} },

	{ MODKEY|ShiftMask,    XK_n,                    tagtoleft,      {0} },	/*将当前窗口移到其他标签*/
	{ MODKEY|ShiftMask,    XK_i,                    tagtoright,     {0} },

	{ MODKEY,              XK_Tab,                  view,           {0} },      /*历史标签*/

	{ MODKEY,              XK_g,                    incnmaster,     {.i = +1 } },	/*切换布局方式*/
	{ MODKEY|ShiftMask,    XK_g,                    incnmaster,     {.i = -1 } },

	{ MODKEY,              XK_Left,                 setmfact,       {.f = -0.05} },	/*修改窗口大大小*/
	{ MODKEY,              XK_Right,                setmfact,       {.f = +0.05} },

	{ MODKEY,              XK_o,                    hidewin,        {0} },		/*最小化框口*/
	{ MODKEY|ShiftMask,    XK_o,                    restorewin,     {0} },		/*恢复最近一次隐藏的窗口*/
	{ MODKEY,              XK_u,                    hideotherwins,  {0}},		/*隐藏除当前窗口以外的所有窗口*/
	{ MODKEY|ShiftMask,    XK_u,                    restoreotherwins, {0}},		/*恢复所有在当前标签下的窗口*/

	{ MODKEY|ShiftMask,    XK_b,                    togglebar,      {0} },		/*隐藏侧边栏*/
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },		/*将当前窗口调为主窗口*/
	{ ControlMask|ShiftMask,    XK_q,                    killclient,     {0} },		/*关闭当前窗口*/


	/*  --------------------------设置布局模式-------------------------------------------- */
	{ MODKEY,              XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_m,                    setlayout,      {.v = &layouts[1]} },
	{ MODKEY,              XK_f,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_space,                setlayout,      {0} },
	{ MODKEY|ShiftMask,    XK_f,                    fullscreen,     {0} },		/*全屏模式*/

	{ MODKEY|ShiftMask,    XK_space,                togglefloating, {0} },
	{ MODKEY,              XK_apostrophe,           togglescratch,  {.v = scratchpadcmd } },
	// { MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	// { MODKEY|ShiftMask,    XK_0,                    tag,            {.ui = ~0 } },
	//	{ MODKEY,              XK_comma,                focusmon,       {.i = -1 } },
	//	{ MODKEY,              XK_period,               focusmon,       {.i = +1 } },
	//	{ MODKEY|ShiftMask,    XK_comma,                tagmon,         {.i = -1 } },
	//	{ MODKEY|ShiftMask,    XK_period,               tagmon,         {.i = +1 } },
	 	TAGKEYS(               XK_1,                      0)
	 	TAGKEYS(               XK_2,                      1)
	 	TAGKEYS(               XK_3,                      2)
	 	TAGKEYS(               XK_4,                      3)
	 	TAGKEYS(               XK_5,                      4)
	 	TAGKEYS(               XK_6,                      5)
	 	TAGKEYS(               XK_7,                      6)
	 	TAGKEYS(               XK_8,                      7)
	 	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask|ShiftMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
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


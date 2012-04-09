#ifndef FONT_H_
#define FONT_H_

static int symbol_w = 14;
static int symbol_h = 22;
static wchar_t* encoding = L" "
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789.,:;!?@#$%^&*()[]{}_-+<>=~\"'`/\\|";
static wchar_t* cursor_encoding = L"0123456789";

static Uint8 cursor[] =

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"##############"
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"##############"
		"##############"
		"##############"
		"##############";

static Uint8 font[] =

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"              "

		"     ####     "
		"    ######    "
		"   ###  ###   "
		"   ###  ###   "
		"  ###    ###  "
		"  ###    ###  "
		" ###      ### "
		" ###      ### "
		"###        ###"
		"###        ###"
		"##############"
		"##############"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"              "
		"              "
		"              "
		"              "

		" ########     "
		"###########   "
		"###     ####  "
		"###      #### "
		"###      #### "
		"###      #### "
		"###      #### "
		"###      ###  "
		"###########   "
		"##########    "
		"###     ####  "
		"###      #### "
		"###       ####"
		"###       ####"
		"###       ####"
		"###      #### "
		"############  "
		"##########    "
		"              "
		"              "
		"              "
		"              "

		"    #######   "
		"  ########### "
		" ####     ####"
		"####          "
		"###           "
		"###           "
		"###           "
		"###           "
		"###           "
		"###           "
		"###           "
		"###           "
		"###           "
		"###           "
		"####          "
		" ####     ####"
		"  ########### "
		"    #######   "
		"              "
		"              "
		"              "
		"              "

		"   #######    "
		"  ##########  "
		" ####    #### "
		" ###      ####"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###      ####"
		" ###     #### "
		" ###########  "
		" #########    "
		"              "
		"              "
		"              "
		"              "

		"  ############"
		" #############"
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ##########   "
		" ##########   "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" #############"
		"  ############"
		"              "
		"              "
		"              "
		"              "

		"  ############"
		" #############"
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ##########   "
		" ##########   "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		"              "
		"              "
		"              "
		"              "

		"    ######    "
		"  ##########  "
		" ###      ### "
		"###        ###"
		"###        ###"
		"###           "
		"###           "
		"###           "
		"###    #######"
		"###    #######"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		"  ##########  "
		"    ######    "
		"              "
		"              "
		"              "
		"              "

		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" #############"
		" #############"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		"              "
		"              "
		"              "
		"              "

		"  ########### "
		"  ########### "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"  ########### "
		"  ########### "
		"              "
		"              "
		"              "
		"              "

		"     ######## "
		"     ######## "
		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"          ### "
 		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"###       ### "
		"###       ### "
		" ####   ####  "
		"  #########   "
		"    #####     "
		"              "
		"              "
		"              "
		"              "

		"###       ### "
		"###       ### "
		"###       ### "
		"###       ### "
		"###      ###  "
		"###     ###   "
		"###    ###    "
		"#########     "
		"########      "
		"###   ###     "
		"###    ###    "
		"###     ###   "
		"###      ###  "
		"###       ### "
		"###       ### "
		"###       ### "
		"###       ### "
		"###       ### "
		"              "
		"              "
		"              "
		"              "

		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###########  "
		" ###########  "
		"              "
		"              "
		"              "
		"              "

		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"####      ####"
		"#####    #####"
		"######  ######"
		"### ###### ###"
		"###  ####  ###"
		"###   ##   ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"              "
		"              "
		"              "
		"              "

		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"####       ###"
		"#####      ###"
		"######     ###"
		"### ###    ###"
		"###  ###   ###"
		"###   ###  ###"
		"###    ### ###"
		"###     ######"
		"###      #####"
		"###       ####"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"              "
		"              "
		"              "
		"              "

		"    ######    "
		"  ##########  "
		" ###      ### "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		"  ##########  "
		"    ######    "
		"              "
		"              "
		"              "
		"              "

		"  #########   "
		" ############ "
		" ###      ####"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###      ####"
		" ############ "
		" ##########   "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		"              "
		"              "
		"              "
		"              "

		"    ######    "
		"  ##########  "
		" ###      ### "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		"  ##########  "
		"    ######    "
		"        ###   "
		"         ###  "
		"          ### "
		"              "


		"  #########   "
		" ############ "
		" ###      ####"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###      ####"
		" ############ "
		" ##########   "
		" ###     ###  "
		" ###      ### "
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		"              "
		"              "
		"              "
		"              "

		"  ##########  "
		" ############ "
		"####      ####"
		"###           "
		"###           "
		"###           "
		"####          "
		" #########    "
		"  ##########  "
		"         #### "
		"           ###"
		"           ###"
		"           ###"
		"           ###"
		"           ###"
		"####      ####"
		" ############ "
		"  ##########  "
		"              "
		"              "
		"              "
		"              "

		" #############"
		" #############"
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"              "
		"              "
		"              "
		"              "

		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		"  ##########  "
		"    ######    "
		"              "
		"              "
		"              "
		"              "

		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		" ###      ### "
		"  ###    ###  "
		"   ###  ###   "
		"    ######    "
		"     ####     "
		"      ##      "
		"              "
		"              "
		"              "
		"              "

		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###   ##   ###"
		"###  ####  ###"
		"### ###### ###"
		"######  ######"
		"#####    #####"
		"####      ####"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"              "
		"              "
		"              "
		"              "

		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		"  ###    ###  "
		"   ###  ###   "
		"    ######    "
		"     ####     "
		"      ##      "
		"     ####     "
		"    ######    "
		"   ###  ###   "
		"  ###    ###  "
		" ###      ### "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"              "
		"              "
		"              "
		"              "

		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		"  ###     ### "
		"   ###   ###  "
		"    #######   "
		"     #####    "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"              "
		"              "
		"              "
		"              "

		"##############"
		"##############"
		"           ###"
		"           ###"
		"          ### "
		"         ###  "
		"        ###   "
		"       ###    "
		"      ###     "
		"     ###      "
		"    ###       "
		"   ###        "
		"  ###         "
		" ###          "
		"###           "
		"###           "
		"##############"
		"##############"
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"     #######  "
		"   ########## "
		"  ####    ### "
		" ####     ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ####     ### "
		"  ####    ### "
		"   ########## "
		"    ######### "
		"              "
		"              "
		"              "
		"              "

		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ########     "
		" ##########   "
		" ###    ####  "
		" ###     #### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###     #### "
		" ###    ####  "
		" ##########   "
		" ########     "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"    ######    "
		"  ##########  "
		" ####    #### "
		"####          "
		"###           "
		"###           "
		"###           "
		"###           "
		"####          "
		" ####    #### "
		"  ##########  "
		"    ######    "
		"              "
		"              "
		"              "
		"              "

		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"          ### "
		"     ######## "
		"   ########## "
		"  ####    ### "
		" ####     ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ####     ### "
		"  ####    ### "
		"   ########## "
		"    ######### "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"    ######    "
		"  ##########  "
		" ####    #### "
		"####       ###"
		"###        ###"
		"###       ### "
		"############  "
		"###           "
		"####          "
		" ####     ####"
		"  ########### "
		"    #######   "
		"              "
		"              "
		"              "
		"              "

		"         #####"
		"       #######"
		"      ####    "
		"      ###     "
		"      ###     "
		"      ###     "
		" #############"
		" #############"
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"      ###     "
		"     ####     "
		"########      "
		"#######       "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"    ######    "
		"  ##########  "
		" ####    #### "
		"####       ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ####    #### "
		"  #########   "
		"   #########  "
		"          ### "
		"           ###"
		"###        ###"
		"####      ### "
		" ###########  "
		"   #######    "


		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" #########    "
		" ###########  "
		" ###     #### "
		" ###      ####"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		"              "
		"              "
		"              "
		"              "

		"              "
		"    ####      "
		"    ####      "
		"    ####      "
		"              "
		"              "
		"########      "
		"########      "
		"     ###      "
		"     ###      "
		"     ###      "
		"     ###      "
		"     ###      "
		"     ###      "
		"     ###   ###"
		"     #### ####"
		"      ####### "
		"       #####  "
		"              "
		"              "
		"              "
		"              "


		"              "
		"      ####    "
		"      ####    "
		"      ####    "
		"              "
		"              "
		"  ########    "
		"  ########    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"       ###    "
		"###    ###    "
		" ###  ####    "
		"  #######     "
		"   #####      "

		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###     ###  "
		" ###    ###   "
		" ###   ###    "
		" ###  ###     "
		" #######      "
		" ########     "
		" ###   ###    "
		" ###    ###   "
		" ###     ###  "
		" ###      ### "
		" ###       ###"
		" ###       ###"
		"              "
		"              "
		"              "
		"              "

		"######        "
		"######        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###        "
		"   ###     ###"
		"   ####   ####"
		"    ######### "
		"     #######  "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"#####   ####  "
		"############# "
		"###  ###  ####"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		" ########     "
		" ##########   "
		" ###    ####  "
		" ###     #### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		" ###      ### "
		"              "
		"              "
		"              "
		"              "


		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"    ######    "
		"  ##########  "
		" ####    #### "
		"####      ####"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"####      ####"
		" ####    #### "
		"  ##########  "
		"    ######    "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		" #########    "
		" ###########  "
		" ###     #### "
		" ###      ####"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###       ###"
		" ###      ####"
		" ###     #### "
		" ###########  "
		" #########    "
		" ###          "
		" ###          "
		" ###          "
		" ###          "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"    ######### "
		"  ########### "
		" ####     ### "
		"####      ### "
		"###       ### "
		"###       ### "
		"###       ### "
		"###       ### "
		"####      ### "
		" ####     ### "
		"  ########### "
		"   ########## "
		"          ### "
		"          ### "
		"          ### "
		"          ### "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		" ########     "
		" ##########   "
		" ###    ####  "
		" ###     #### "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		" ###          "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"  #########   "
		" ###########  "
		"####     #### "
		"####          "
		" ########     "
		"  #########   "
		"        ####  "
		"          ### "
		"          ### "
		"####     #### "
		" ###########  "
		"  #########   "
		"              "
		"              "
		"              "
		"              "

		"    ###       "
		"    ###       "
		"    ###       "
		"    ###       "
		"    ###       "
		"    ###       "
		" ############ "
		" ############ "
		"    ###       "
		"    ###       "
		"    ###       "
		"    ###       "
		"    ###       "
		"    ###       "
		"    ###    ###"
		"    ####  ####"
		"     ######## "
		"      ######  "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"####      ####"
		" ####    #### "
		"  ##########  "
		"    ######    "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		" ###      ### "
		" ###      ### "
		"  ###    ###  "
		"   ###  ###   "
		"    ######    "
		"     ####     "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###   ##   ###"
		"###  ####  ###"
		" ############ "
		"  ##########  "
		"   ###  ###   "
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"###        ###"
		" ###      ### "
		"  ###    ###  "
		"   ###  ###   "
		"    ######    "
		"     ####     "
		"    ######    "
		"   ###  ###   "
		"  ###    ###  "
		" ###      ### "
		"###        ###"
		"###        ###"
		"              "
		"              "
		"              "
		"              "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"###        ###"
		"####       ###"
		" ####      ###"
		"  ############"
		"   ###########"
		"           ###"
		"           ###"
		"###        ###"
		"####      ### "
		" ###########  "
		"   #######    "

		"              "
		"              "
		"              "
		"              "
		"              "
		"              "
		"############# "
		"############  "
		"        ###   "
		"       ###    "
		"      ###     "
		"     ###      "
		"    ###       "
		"   ###        "
		"  ###         "
		" ###          "
		"############# "
		"############# "
		"              "
		"              "
		"              "
		"              "


		"          "
		" #######  "
		"##     ## "
		"##     ## "
		"##  #  ## "
		"##  #  ## "
		"##  #  ## "
		"##  #  ## "
		"##  #  ## "
		"##  #  ## "
		"##     ## "
		"##     ## "
		" #######  "
		"          "
		"          "
		"          "

		"          "
		" #####    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		" ######## "
		"          "
		"          "
		"          "


		"          "
		" #######  "
		"##     ## "
		"       ## "
		"       ## "
		"      ##  "
		"     ##   "
		"   ##     "
		"  ##      "
		" ##       "
		"##        "
		"##        "
		"######### "
		"          "
		"          "
		"          "

		"          "
		" #######  "
		"##     ## "
		"       ## "
		"       ## "
		"      ##  "
		"   ####   "
		"      ##  "
		"       ## "
		"       ## "
		"       ## "
		"##     ## "
		" #######  "
		"          "
		"          "
		"          "

		"          "
		"     ###  "
		"    ####  "
		"   ## ##  "
		"  ##  ##  "
		" ##   ##  "
		"##    ##  "
		"##    ##  "
		"######### "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"          "
		"          "
		"          "

		"          "
		"########  "
		"##        "
		"##        "
		"##        "
		"##        "
		" ######   "
		"      ##  "
		"       ## "
		"       ## "
		"       ## "
		"##    ##  "
		" ######   "
		"          "
		"          "
		"          "

		"          "
		" #######  "
		"##     ## "
		"##        "
		"##        "
		"##        "
		"########  "
		"##     ## "
		"##     ## "
		"##     ## "
		"##     ## "
		"##     ## "
		" #######  "
		"          "
		"          "
		"          "

		"          "
		"##########"
		"       ## "
		"      ##  "
		"     ##   "
		"     ##   "
		"    ##    "
		"    ##    "
		"    ##    "
		"   ##     "
		"   ##     "
		"   ##     "
		"   ##     "
		"          "
		"          "
		"          "

		"          "
		" #######  "
		"##     ## "
		"##     ## "
		"##     ## "
		"##     ## "
		" #######  "
		"##     ## "
		"##     ## "
		"##     ## "
		"##     ## "
		"##     ## "
		" #######  "
		"          "
		"          "
		"          "

		"          "
		" #######  "
		"##     ## "
		"##     ## "
		"##     ## "
		"##     ## "
		" ######## "
		"       ## "
		"       ## "
		"       ## "
		"       ## "
		"##     ## "
		" #######  "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"   ###    "
		"   ###    "
		"   ###    "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"   ###    "
		"   ###    "
		"   ###    "
		"   ##     "
		"  ##      "
		"          "

		"          "
		"          "
		"          "
		"   ###    "
		"   ###    "
		"   ###    "
		"          "
		"          "
		"          "
		"          "
		"   ###    "
		"   ###    "
		"   ###    "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"   ###    "
		"   ###    "
		"   ###    "
		"          "
		"          "
		"          "
		"          "
		"   ###    "
		"   ###    "
		"   ###    "
		"   ##     "
		"  ##      "
		"          "


		"    #     "
		"   ###    "
		"   ###    "
		"   ###    "
		"   ###    "
		"   ###    "
		"    #     "
		"    #     "
		"    #     "
		"          "
		"    #     "
		"   ###    "
		"   ###    "
		"          "
		"          "
		"          "

		"   ####   "
		"  ##  ##  "
		" ##    ## "
		" ##    ## "
		"      ##  "
		"     ##   "
		"    ##    "
		"   ##     "
		"    ##    "
		"          "
		"    #     "
		"   ###    "
		"   ###    "
		"          "
		"          "
		"          "

		"          "
		"  ######  "
		" ##    ## "
		"##      ##"
		"##      ##"
		"##  ##  ##"
		"## # ## ##"
		"## # ## ##"
		"## # ## ##"
		"##  ##### "
		"##        "
		"##      ##"
		" ##    ## "
		"  ######  "
		"          "
		"          "

		"          "
		"   ##  ## "
		"   ##  ## "
		"   ##  ## "
		"   ##  ## "
		" #########"
		"  ##  ##  "
		"  ##  ##  "
		"  ##  ##  "
		"######### "
		" ##  ##   "
		" ##  ##   "
		" ##  ##   "
		" ##  ##   "
		"          "
		"          "

		"          "
		"    ##    "
		" ######## "
		"##  ##  ##"
		"##  ##    "
		"##  ##    "
		" #####    "
		"    ##    "
		"    ##### "
		"    ##  ##"
		"    ##  ##"
		"##  ##  ##"
		" ######## "
		"    ##    "
		"          "
		"          "

		"          "
		" ####   ##"
		"##  ## ## "
		"##  ## ## "
		" #### ##  "
		"     ##   "
		"    ##    "
		"   ##     "
		"   ##     "
		"  ## #### "
		" ## ##  ##"
		" ## ##  ##"
		"##   #### "
		"          "
		"          "
		"          "

		"          "
		"    ##    "
		"   ####   "
		"  ##  ##  "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "

		"          "
		"  ###     "
		" ## ##    "
		"##   ##   "
		"##   ##   "
		" ##  #    "
		"  ###     "
		" ## ##    "
		"##   #####"
		"##    ##  "
		"##    ##  "
		" ##  #### "
		"  ####  ##"
		"          "
		"          "
		"          "

		"          "
		"          "
		"  ##  ##  "
		"   ####   "
		" ######## "
		"   ####   "
		"  ##  ##  "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "

		"          "
		"       #  "
		"     ##   "
		"    ##    "
		"   ##     "
		"   ##     "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"   ##     "
		"   ##     "
		"    ##    "
		"     ##   "
		"       #  "

		"          "
		"  #       "
		"   ##     "
		"    ##    "
		"     ##   "
		"     ##   "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"     ##   "
		"     ##   "
		"    ##    "
		"   ##     "
		"  #       "

		"          "
		"  #####   "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  ##      "
		"  #####   "

		"          "
		"   #####  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"      ##  "
		"   #####  "


		"          "
		"     ###  "
		"    ##    "
		"   ##     "
		"   ##     "
		"   ##     "
		"   ##     "
		"  ##      "
		"###       "
		"  ##      "
		"   ##     "
		"   ##     "
		"   ##     "
		"   ##     "
		"    ##    "
		"     ###  "

		"          "
		"  ###     "
		"    ##    "
		"     ##   "
		"     ##   "
		"     ##   "
		"     ##   "
		"      ##  "
		"       ###"
		"      ##  "
		"     ##   "
		"     ##   "
		"     ##   "
		"     ##   "
		"    ##    "
		"  ###     "

		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"##########"
		"          "

		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"  ######  "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"          "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"##########"
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"       ###"
		"     ###  "
		"   ###    "
		" ###      "
		"##        "
		" ###      "
		"   ###    "
		"     ###  "
		"       ###"
		"          "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"###       "
		"  ###     "
		"    ###   "
		"      ### "
		"        ##"
		"      ### "
		"    ###   "
		"  ###     "
		"###       "
		"          "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		" ######## "
		"          "
		"          "
		" ######## "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "

		"          "
		"          "
		"          "
		"      ### "
		"#    ## ##"
		"## ##    #"
		" ###      "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "

		"          "
		"  ##  ##  "
		"  ##  ##  "
		"  ##  ##  "
		"  ##  ##  "
		"   #  #   "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "


		"          "
		"    ###   "
		"    ###   "
		"    ###   "
		"    ##    "
		"   ##     "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "


		"          "
		"  ###     "
		"  ###     "
		"  ###     "
		"   ##     "
		"    ##    "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "
		"          "

		"       ## "
		"       ## "
		"      ##  "
		"      ##  "
		"     ##   "
		"     ##   "
		"    ##    "
		"    ##    "
		"   ##     "
		"   ##     "
		"  ##      "
		"  ##      "
		" ##       "
		" ##       "
		"          "
		"          "

		"##        "
		"##        "
		" ##       "
		" ##       "
		"  ##      "
		"  ##      "
		"   ##     "
		"   ##     "
		"    ##    "
		"    ##    "
		"     ##   "
		"     ##   "
		"      ##  "
		"      ##  "
		"          "
		"          "

		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"    ##    "
		"          "
		;


#endif /* FONT_H_ */

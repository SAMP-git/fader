
// fadeTest Script por Stewie`
/* Comandos:
*       /fadein [tempo]
*       /fadeout [tempo]
*       /fade [tempoIn] [tempoWait] [tempoOut]
*/
 
#define FILTERSCRIPT
#include <a_samp>

#tryinclude <sscanf>
#if !defined sscanf
	#tryinclude <sscanf2>
	#if !defined sscanf
		#error "requires sscanf"
	#endif
#endif

#include <fader>
#include <zcmd>
 
public OnFilterScriptInit() {
	print("* fadeTest Script por Stewie` carregado.");
	return 1;
}
 
CMD:fadein(playerid, params[]) {
	new time;
	if(!strlen(params))
		return SendClientMessage(playerid, -1, "CMD: /fadein [tempo]");

	time = strval(params);
	fadeIn(playerid, time, 0x000000FF);
	return 1;
}

CMD:fadeout(playerid, params[]) {
	new time;
	if(!strlen(params))
		return SendClientMessage(playerid, -1, "CMD: /fadeout [tempo]");

	time = strval(params);
	fadeOut(playerid, time, 0x000000FF);
	return 1;
}
CMD:fade(playerid, params[]) {
	new timeIn, timeWait, timeOut;
	if(sscanf(params, "ddd", timeIn, timeWait, timeOut))
		return SendClientMessage(playerid, -1, "CMD: /fade [tempo] [tempoWait] [tempoOut]");

	fade(playerid, timeIn, timeWait, timeOut);
	return 1;
}
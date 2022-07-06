#include <a_samp>

forward Fader_PlayerProcess(playerid);
forward Fader_PlayerProcessOut(playerid);
forward OnPlayerFadeIn(playerid);
forward OnPlayerFadeOut(playerid);
stock fadeIn(playerid, ms, color = 0x000000FF);
stock fadeOut(playerid, ms, bool:hide = true, color = 0x000000FF);

static const
	MAX_TRANSPARENCY = 0xFF,
	TIMER_FRAME = 25;
	
enum {
	FADE_IN = 1,
	FADE_OUT
}

enum E_PLAYER_FADE_INFO {
	PlayerText:fadeText,
	fadeCurrentAlpha,
	
	fadeTotalFrames,
	fadeTransparencyPerFrame,
	fadeColor,
	
	fadeTimer,
	fadeType,
	bool:fadeHide,
	bool:fadeAll,
	fadeWaitTime,
	fadeOutTime,
	fadeOutColor
}
static gPlayerFadeInfo[MAX_PLAYERS][E_PLAYER_FADE_INFO];

#define CAP_MAX(%0) ((%0 > MAX_TRANSPARENCY) ? (MAX_TRANSPARENCY) : %0)
#define CAP_MIN(%0) ((%0 < 0) ? (0) : %0)

/* Natives:
*	native fadeIn(playerid, ms, color = 0x000000FF);
*	native fadeOut(playerid, ms, color = 0x000000FF);
*
* 	por Stewie`
*/
stock fade(playerid, msIn, msWait, msOut, bool:hide = true, colorIn = 0x000000FF, colorOut = 0x000000FF) {
	if(!IsPlayerConnected(playerid)) 
		return 0;
	if(msIn < 0 || msWait < 0 || msOut < 0)
		return 0;

	gPlayerFadeInfo[playerid][fadeAll] = true;
	gPlayerFadeInfo[playerid][fadeWaitTime]= msWait;
	gPlayerFadeInfo[playerid][fadeOutTime] = msOut;
	gPlayerFadeInfo[playerid][fadeOutColor] = colorOut;
	gPlayerFadeInfo[playerid][fadeHide] = hide;

	return fadeIn(playerid, msIn, colorIn);
}
stock fadeIn(playerid, ms, color = 0x000000FF) {
	if(!IsPlayerConnected(playerid)) 
		return 0;
		
	// pegar a transparência atual da textdraw
	new alpha = gPlayerFadeInfo[playerid][fadeCurrentAlpha];
	// no caso da textdraw tiver a transparência cheia, ou mais do que o permitido, o nosso trabalho está feito
	if((MAX_TRANSPARENCY - alpha) <= 0) {
		gPlayerFadeInfo[playerid][fadeCurrentAlpha] = MAX_TRANSPARENCY;
		gPlayerFadeInfo[playerid][fadeType] = 0;

		PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | alpha);
		PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
		return 1;
	}
	// configurar o tipo de fade que estamos aplicando
	gPlayerFadeInfo[playerid][fadeType] = FADE_IN;
	
	// mover um byte para a direita para 'apagar' a transparência
	// e '>>' para não voltar para o começo os primeiros bits
	gPlayerFadeInfo[playerid][fadeColor] = color >> 8;

	// a quantidade de frames que o PlayerProcess irá processar
	// você pode perceber que a duração é dividida pelos frames por processamento e, se existe restante, é adicinado mais um processamento
	gPlayerFadeInfo[playerid][fadeTotalFrames] = ((ms / TIMER_FRAME) + _:((ms % TIMER_FRAME) > 0));
	// o tanto de transparência que será adicionado por cada processamento
	// no fim, deve ser checado a quantidade de transarência que a textdraw possui
	// a transparência que será adicionada até o final é dividida pela quantidade de frames que serão rodados
	gPlayerFadeInfo[playerid][fadeTransparencyPerFrame] =  (MAX_TRANSPARENCY - alpha) / gPlayerFadeInfo[playerid][fadeTotalFrames];
	
	if(gPlayerFadeInfo[playerid][fadeTransparencyPerFrame] <= 0) {
		gPlayerFadeInfo[playerid][fadeTransparencyPerFrame] = 10;
	}
	
	PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | alpha);
	PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
	// se não existir o timer, criá-lo!
	if(!gPlayerFadeInfo[playerid][fadeTimer]) {
		gPlayerFadeInfo[playerid][fadeTimer] = SetTimerEx("Fader_PlayerProcess", TIMER_FRAME, 1, "i", playerid);
	}
	return 1;
}

stock fadeOut(playerid, ms, bool:hide = true, color = 0x000000FF) {
	if(!IsPlayerConnected(playerid)) 
		return 0;
		
	// pegar a transparência atual da textdraw
	new alpha = gPlayerFadeInfo[playerid][fadeCurrentAlpha];
	// no caso da textdraw não tiver nenhuma transparênca, sair
	if(alpha <= 0) {
		gPlayerFadeInfo[playerid][fadeCurrentAlpha] = MAX_TRANSPARENCY;
		gPlayerFadeInfo[playerid][fadeType] = 0;

		PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | alpha);
		PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
		return 1;
	}
	// ao finalizar esconder a textdraw
	gPlayerFadeInfo[playerid][fadeHide] = hide;
	
	// configurar o tipo de fade que estamos aplicando
	gPlayerFadeInfo[playerid][fadeType] = FADE_OUT;
	
	// mover um byte para a direita para 'apagar' a transparência
	// e '>>' para não voltar para o começo os primeiros bits
	gPlayerFadeInfo[playerid][fadeColor] = color >> 8;

	// a quantidade de frames que o PlayerProcess irá processar
	// você pode perceber que a duração é dividida pelos frames por processamento e, se existe restante, é adicinado mais um processamento
	gPlayerFadeInfo[playerid][fadeTotalFrames] = ((ms / TIMER_FRAME) + _:((ms % TIMER_FRAME) > 0));
	// o tanto de transparência que será subtraída por cada processamento
	// no fim, deve ser checado a quantidade de transarência que a textdraw possui
	// a transparência que será subtraída até o final é dividida pela quantidade de frames que serão rodados
	gPlayerFadeInfo[playerid][fadeTransparencyPerFrame] =  (alpha) / gPlayerFadeInfo[playerid][fadeTotalFrames];
	
	if(gPlayerFadeInfo[playerid][fadeTransparencyPerFrame] <= 0) {
		gPlayerFadeInfo[playerid][fadeTransparencyPerFrame] = 10;
	}
	
	PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | alpha);
	PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
	// se não existir o timer, criá-lo!
	if(!gPlayerFadeInfo[playerid][fadeTimer]) {
		gPlayerFadeInfo[playerid][fadeTimer] = SetTimerEx("Fader_PlayerProcess", TIMER_FRAME, 1, "i", playerid);
	}
	return 1;
}

public Fader_PlayerProcess(playerid) {
	// se o fade foi desligado para o player, mas o timer ainda está ativo
	if(gPlayerFadeInfo[playerid][fadeType] == 0) {
		// se o timer existe
		if(gPlayerFadeInfo[playerid][fadeTimer] > 0) {
			KillTimer(gPlayerFadeInfo[playerid][fadeTimer]);
			gPlayerFadeInfo[playerid][fadeTimer] = 0;

			PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | gPlayerFadeInfo[playerid][fadeCurrentAlpha]);
			PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
		}
	}
	
	switch(gPlayerFadeInfo[playerid][fadeType]) {
		case FADE_IN: {
			gPlayerFadeInfo[playerid][fadeCurrentAlpha] += gPlayerFadeInfo[playerid][fadeTransparencyPerFrame];
			gPlayerFadeInfo[playerid][fadeCurrentAlpha] = CAP_MAX(gPlayerFadeInfo[playerid][fadeCurrentAlpha]);
			
			PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | gPlayerFadeInfo[playerid][fadeCurrentAlpha]);
			PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
			
			if(gPlayerFadeInfo[playerid][fadeCurrentAlpha] == MAX_TRANSPARENCY) {
				gPlayerFadeInfo[playerid][fadeType] = 0;
				KillTimer(gPlayerFadeInfo[playerid][fadeTimer]);
				gPlayerFadeInfo[playerid][fadeTimer] = 0;

				CallLocalFunction("OnPlayerFadeIn", "i", playerid);
				if(gPlayerFadeInfo[playerid][fadeAll]) {
					SetTimerEx("Fader_PlayerProcessOut", gPlayerFadeInfo[playerid][fadeWaitTime], 0, "i", playerid);
				}
			}
		}
		case FADE_OUT: {
			gPlayerFadeInfo[playerid][fadeCurrentAlpha] -= gPlayerFadeInfo[playerid][fadeTransparencyPerFrame];
			gPlayerFadeInfo[playerid][fadeCurrentAlpha] = CAP_MIN(gPlayerFadeInfo[playerid][fadeCurrentAlpha]);
			
			PlayerTextDrawBoxColor(playerid, gPlayerFadeInfo[playerid][fadeText], (gPlayerFadeInfo[playerid][fadeColor] << 8) | gPlayerFadeInfo[playerid][fadeCurrentAlpha]);
			PlayerTextDrawShow(playerid, gPlayerFadeInfo[playerid][fadeText]);
			
			if(gPlayerFadeInfo[playerid][fadeCurrentAlpha] == 0) {
				gPlayerFadeInfo[playerid][fadeType] = 0;
				KillTimer(gPlayerFadeInfo[playerid][fadeTimer]);
				gPlayerFadeInfo[playerid][fadeTimer] = 0;

				CallLocalFunction("OnPlayerFadeOut", "i", playerid);

				if(gPlayerFadeInfo[playerid][fadeHide])
					PlayerTextDrawHide(playerid, gPlayerFadeInfo[playerid][fadeText]);

				gPlayerFadeInfo[playerid][fadeAll] = false;
			}
		}
	}
	return 1;
}

public Fader_PlayerProcessOut(playerid) {
	fadeOut(playerid, gPlayerFadeInfo[playerid][fadeOutTime], gPlayerFadeInfo[playerid][fadeHide], gPlayerFadeInfo[playerid][fadeOutColor]);
}

public OnPlayerConnect(playerid) {
	new tmp[E_PLAYER_FADE_INFO];
	gPlayerFadeInfo[playerid] = tmp;

	new PlayerText:text = CreatePlayerTextDraw(playerid, -20.0, -20.0, "_");
	gPlayerFadeInfo[playerid][fadeText] = text;
	// detalhes
	PlayerTextDrawUseBox(playerid, text, 1);
	PlayerTextDrawBoxColor(playerid, text, 0xFFFFFF00);
	PlayerTextDrawLetterSize(playerid, text, 680.0, 500.0);
	PlayerTextDrawTextSize(playerid, text, 680.0, 500.0);

	if (funcidx("Fader_OnPlayerConnect") != -1)
	{
		return CallLocalFunction("Fader_OnPlayerConnect", "i", playerid);
	}	
    return 1;
}

public OnPlayerDisconnect(playerid,reason) {
	gPlayerFadeInfo[playerid][fadeTimer] = 0;

	if (funcidx("Fader_OnPlayerDisconnect") != -1)
	{
		return CallLocalFunction("Fader_OnPlayerDisconnect", "ii", playerid, reason);
	}	
    return 1;
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect Fader_OnPlayerConnect
forward Fader_OnPlayerConnect(playerid);

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect Fader_OnPlayerDisconnect
forward OnPlayerDisconnect(playerid, reason);

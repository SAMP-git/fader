# fader

### Original por [Stewie`](http://forum.sa-mp.com/member.php?u=108849) [topico](http://forum.sa-mp.com/showthread.php?t=422245)

#### fade
> Mostra e esconde uma textdraw para o player em três passos
> - Aumenta de acordo com o tempo(msIn) sua opacidade
> - Um tempo(msWait) de hiato
> - Diminui de acordo com o tempo sua opacidade
>
>* **Parameters:**
>   * `playerid`: ID do player 
>   * `msIn`: tempo para mostrar totalmente
>   * `msWait`: tempo para esperar e começar o out
>   * `msOut`: tempo para esconder totalmente
>   * `hide`: esconder textdraw após o fade? 
>   * `color`: cor do fade
>* **Returns:**
>   * verdadeiro sucesso, falso caso contrario
>* **Remarks:**
>   - tempo em ms
 
***

#### fadeIn
> Mostra uma textdraw para o player aumentando de acordo com o tempo sua opacidade
> 
>* **Parameters:**
>   * `playerid`: ID do player 
>   * `ms`: tempo para mostrar totalmente
>   * `color`: cor do fade
>* **Returns:**
>   * verdadeiro sucesso, falso caso contrario
>* **Remarks:**
>   - tempo em ms
 
***

#### fadeOut
> Esconde uma textdraw para o player diminuindo de acordo com o tempo sua opacidade
> 
>* **Parameters:**
>   * `playerid`: ID do player 
>   * `ms`: tempo para esconder totalmente
>   * `color`: cor do fade
>* **Returns:**
>   * verdadeiro sucesso, falso caso contrario
>* **Remarks:**
>   - tempo em ms
 
***

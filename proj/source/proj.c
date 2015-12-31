#include <minix/drivers.h>
#include "timer.h"
#include "pixmap.h"
#include "interface.h"
#include "Bitmap.h"
#include "Player.h"
#include "Game.h"
#include "GameState.h"
#include "Collisions.h"

int main(int argc, char **argv) {

	sef_startup();
	initGame(0x114);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------

	int r, ipc_status;
	message msg;
	unsigned char codigo;
	char irq_keyboard, irq_timer;

	//Iniciar jogo
	GameState* gameSt=newGameState();


	if ((irq_keyboard = kbd_subscribe(&hook_kbd)) == -1) {
		printf("Failed keyboard subscribe.\n");
		return;
	}
	if ((irq_timer = timer_subscribe_int()) == 1) {
		printf("Failed timer subscribe.\n");
		return;
	}
	while (codigo != VAL_ESC) { //enquanto nao for pressionado o ESC continua
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_keyboard) {
					kbd_code_scan(&codigo);
				} else if (msg.NOTIFY_ARG & irq_timer) {
					updateGameState(gameSt,&codigo);
					drawGameState(gameSt);
					refresh();
				}
				break;
			default:
				break;
			}
		}
	}
	deleteGameState(gameSt);
	if (kbd_unsubscribe(&hook_kbd) == -1) {
		printf("Failed keyboard unsubscribe.\n");
		return;
	}

	if (timer_unsubscribe_int() == 1) {
		printf("Failed timer unsubscribe.\n");
		return;
	}

	exitGame();
	return 0;
}

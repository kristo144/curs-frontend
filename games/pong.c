#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <time.h>

#define WINDOW_HEIGHT 400
#define WINDOW_WIDTH 600

#define BORDER 10
#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 70
#define PADDLE_SIZE (Vector2) {PADDLE_WIDTH, PADDLE_HEIGHT}
#define PADDLE_SPEED 5 /* pixels per second */

#define BALL_RADIUS 5
#define BALL_DIAMETER ( 2 * BALL_RADIUS )
#define BALL_SIZE (Vector2) {BALL_DIAMETER, BALL_DIAMETER}
#define BALL_SPEED 4
#define BALL_SPEEDUP 1
#define BALL_SPEEDUP_VERT 2.0f
#define BALL_MAX_SPEED 20

#define DIVIDER_COUNT 10
#define DIVIDER_LEN (((WINDOW_HEIGHT -  BORDER) / DIVIDER_COUNT) - BORDER)
#define DIVIDER_WIDTH 2

#define WIN_SCORE 6
#define SCORE_SIZE 30
#define SCORE_MARGIN 15

enum {
	TITLE,
	PLAYING,
	WIN,
	LOSE,
};

void resetBall(Rectangle *ball, Vector2 *vel) {
	*vel = Vector2Scale(CLITERAL(Vector2) {((GetRandomValue(0,1)<<1)-1), GetRandomValue(-1, 1)}, BALL_SPEED);
	ball->x = (WINDOW_WIDTH  - BALL_DIAMETER) / 2;
	ball->y = (WINDOW_HEIGHT - BALL_DIAMETER) / 2;
}

int main(void) {

	/* Init random */
	SetRandomSeed(time(NULL));

	/* Initialize paddle position */
	Rectangle plr = {BORDER, BORDER, PADDLE_WIDTH, PADDLE_HEIGHT};
	Rectangle nmy = {WINDOW_WIDTH - PADDLE_WIDTH - BORDER, WINDOW_HEIGHT - PADDLE_HEIGHT - BORDER, PADDLE_WIDTH, PADDLE_HEIGHT};
	Rectangle bal = { .width = BALL_DIAMETER, .height = BALL_DIAMETER };
	Vector2 vel;
	resetBall(&bal, &vel);
	unsigned char score[2] = {0, 0};
	int state = TITLE;
	char scrp[4], scre[4];
	int tw;

	/* Calculate dividing line */
	Rectangle divider[DIVIDER_COUNT];
	for (char i = 0; i < DIVIDER_COUNT; i++) {
		divider[i] = CLITERAL(Rectangle) {
			.x = (WINDOW_WIDTH - DIVIDER_WIDTH) / 2,
				.y = BORDER + ((BORDER+DIVIDER_LEN) * i),
				.width = DIVIDER_WIDTH,
				.height = DIVIDER_LEN,
		};
	}

	/* Raylib */
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "pong");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		switch (state) {
			case PLAYING:
				/* Calculate new ball position */
				bal.x += vel.x;
				bal.y += vel.y;
				if (bal.y <= 0) {
					bal.y = 0;
					vel.y = -vel.y;
				}
				else if (bal.y + BALL_DIAMETER >= WINDOW_HEIGHT) {
					bal.y = WINDOW_HEIGHT - BALL_DIAMETER;
					vel.y = -vel.y;
				}

				/* Check for collision with paddles */
				if (CheckCollisionRecs(bal, nmy)) {
					bal.x = nmy.x - BALL_DIAMETER;
					vel.x = -vel.x;
					if (-vel.x < BALL_MAX_SPEED) vel.x -= BALL_SPEEDUP;
					vel.y += BALL_SPEEDUP_VERT * (((bal.y + BALL_RADIUS) - nmy.y) / PADDLE_HEIGHT - 0.5f);
				}
				else if (CheckCollisionRecs(bal, plr)) {
					bal.x = plr.x + PADDLE_WIDTH;
					vel.x = -vel.x;
					if (vel.x < BALL_MAX_SPEED) vel.x += BALL_SPEEDUP;

					vel.y += BALL_SPEEDUP_VERT * (((bal.y + BALL_RADIUS) - plr.y) / PADDLE_HEIGHT - 0.5f);
				}

				/* Move player paddle */
				if (IsKeyDown(KEY_W) && plr.y > 0) {
					plr.y -= PADDLE_SPEED;
				}
				else if (IsKeyDown(KEY_S) && plr.y + PADDLE_HEIGHT < WINDOW_HEIGHT) {
					plr.y += PADDLE_SPEED;
				}

				/* Move enemy paddle */
				if (nmy.y + PADDLE_HEIGHT/2 < bal.y + BALL_RADIUS - 3*BORDER && nmy.y + PADDLE_HEIGHT < WINDOW_HEIGHT) nmy.y += PADDLE_SPEED;
				else if (nmy.y + PADDLE_HEIGHT/2 > bal.y - BALL_RADIUS + 3*BORDER && nmy.y > 0) nmy.y -= PADDLE_SPEED;

				/* Manually reset ball position */
				if (IsKeyPressed(KEY_SPACE)) resetBall(&bal, &vel);

				/* Check scoring */
				if (bal.x > WINDOW_WIDTH) {
					score[0]++;
					if (score[0] >= WIN_SCORE) { state = WIN; }
					else resetBall(&bal, &vel);
				}
				else if (bal.x < 0) {
					score[1]++;
					if (score[1] >= WIN_SCORE) { state = LOSE; }
					resetBall(&bal, &vel);
				}

				/* Calculate score */
				snprintf(scrp, sizeof(scrp), "%i", score[0]);
				snprintf(scre, sizeof(scre), "%i", score[1]);
				tw = MeasureText(scrp, SCORE_SIZE);
				break;
			default:
				if (IsKeyPressed(KEY_SPACE)) {
					state = PLAYING;
					resetBall(&bal, &vel);
					score[0] = 0;
					score[1] = 0;
				}
				break;
		}

		/* Draw scene */
		BeginDrawing();
		ClearBackground(DARKGRAY);

		switch (state) {
			case PLAYING:
				DrawRectangleRec(plr, RAYWHITE);
				DrawRectangleRec(nmy, RAYWHITE);
				DrawRectangleRec(bal, RAYWHITE);
				DrawText(scrp, (WINDOW_WIDTH / 2) - SCORE_MARGIN - tw, SCORE_MARGIN, SCORE_SIZE, RAYWHITE);
				DrawText(scre, (WINDOW_WIDTH / 2) + SCORE_MARGIN, SCORE_MARGIN, SCORE_SIZE, RAYWHITE);
				for (char i = 0; i < DIVIDER_COUNT; i++)
					DrawRectangleRec(divider[i], RAYWHITE);
				break;
			case WIN:
				tw = MeasureText("YOU", 80);
				DrawText("YOU",  ((WINDOW_WIDTH - tw)/ 2), WINDOW_HEIGHT/2 - 90, 80, RAYWHITE);
				tw = MeasureText("WIN", 120);
				DrawText("WIN", ((WINDOW_WIDTH - tw)/ 2), WINDOW_HEIGHT/2, 120, RAYWHITE);
				break;
			case LOSE:
				tw = MeasureText("YOU", 80);
				DrawText("YOU",  ((WINDOW_WIDTH - tw)/ 2), WINDOW_HEIGHT/2 - 90, 80, RAYWHITE);
				tw = MeasureText("LOSE", 120);
				DrawText("LOSE", ((WINDOW_WIDTH - tw)/ 2), WINDOW_HEIGHT/2, 120, RAYWHITE);
				break;
			case TITLE:
				tw = MeasureText("PONG", 140);
				DrawText("PONG", ((WINDOW_WIDTH - tw)/ 2), WINDOW_HEIGHT/2 - 90, 140, RAYWHITE);
				tw = MeasureText("Press SPACE to start", 40);
				DrawText("Press SPACE to start", ((WINDOW_WIDTH - tw)/ 2), WINDOW_HEIGHT/2 + 80, 40, RAYWHITE);
		}
		EndDrawing();
	}
	CloseWindow();

	return 0;
}

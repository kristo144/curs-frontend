#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define WW 1200
#define WH 900

#define RAD 150.0f
#define SPD 3

#define TV 40

int main(void) {
	Vector2 pos = { RAD + 20, WH/4 };
	Vector2 vel = { SPD,  0 };
	const float grav = 1;
	float hs = -1.0f;
	float time = 0.0f;
	_Bool clicked = 0, ticking = 0;

	InitWindow(WW, WH, "bounce");
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		vel.y += grav;
		vel.x *= 0.995f;
		pos = Vector2Add(pos, vel);
		time += ticking * GetFrameTime();

		if (pos.y + RAD > WH) {
			pos.y = WH - RAD;
			vel.y = -vel.y + 0.1;
			ticking = 0;
			if (time > hs) hs = time;
		}
		if (pos.x + RAD > WW) {
			pos.x = WW - RAD;
			vel.x = -vel.x;
		}
		else if (pos.x - RAD < 0) {
			pos.x = RAD;
			vel.x = -vel.x;
		}

		Vector2 mouse = GetMousePosition();
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointCircle(mouse, pos, RAD)) {
			clicked = 1;
			time *= ticking;
			ticking = 1;

			vel.x += pos.x - mouse.x;
			if (vel.x < -TV) vel.x = -TV;
			else if (vel.x > TV) vel.x = TV;
			vel.y = -25.0f;
		}
		int tw = MeasureText("Click the ball!", 40);

		BeginDrawing();
		ClearBackground(DARKGRAY);
		DrawCircleV(pos, RAD, RAYWHITE);
		if (!clicked) DrawText("Click the ball!", (WW - tw) / 2, 40, 40, RAYWHITE);
		else {
			char score[32] = {0};
			snprintf(score, sizeof(score), "Score: %.3f", time);
			DrawText(score, 20, 20, 40, RAYWHITE);
			snprintf(score, sizeof(score), "Hi: %.3f", hs);
			DrawText(score, 20, 80, 40, BLUE);
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

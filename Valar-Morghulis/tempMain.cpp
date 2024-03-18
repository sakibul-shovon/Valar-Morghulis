#include "iGraphics.h"
#include <iostream>
#include <string.h>
#include "Library\screen.h"
#include <utility>
#include <vector>

using namespace std;

#define screen_width 1280
#define screen_height 720

#define and &&
#define or ||

string s;
int image;
float speed = 20;
bool check_enemy = true;

// Screen
bool start_screen = true;
bool home_menu = false;
bool music_on = true;
bool about_us = false;
bool play_Button = false;
bool map1 = false;
bool map2 = false;

int map_2_character_x = 327;
int map_2_character_y = 268;
int enemy_count = 5;
bool dead = true;

// Enemy structure
struct Enemy {
	int x;
	int y;

	Enemy() {
		x = rand() % 800;
		y = rand() % 600;
	}

	void render() {
		iShowBMP2(x, y, "images/enemy.bmp", 255);
	}

	void MoveTowards() {
		float speed = 2;
		float deltaX = map_2_character_x - x;
		float deltaY = map_2_character_y - y;
		float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

		if (distance <= speed || distance == 0) {
			x = map_2_character_x;
			y = map_2_character_y;
		}
		else {
			x += (deltaX / distance) * speed;
			y += (deltaY / distance) * speed;
		}
	}
};

int enemy_count_max = 5;
vector<Enemy> vEnemies(5);

void spawnEnemy() {
	if (vEnemies.size() < enemy_count_max)
		vEnemies.push_back(Enemy());
}

void drawEnemy() {
	cout << "draw";
	int temp_x = rand() % 800;
	int temp_y = rand() % 600;
	iShowImage(temp_x, temp_y, 100, 200, image);
	Enemy e();
}

void iDraw() {
	iClear();

	if (start_screen) {
		iShowBMP2(0, 0, "images/start.bmp", 0);
	}
	else if (home_menu) {
		iShowBMP(0, 0, "images/main.bmp");
	}
	else if (about_us) {
		iShowBMP(0, 0, "images/abc.bmp");
	}
	else if (play_Button && map1) {
		iShowBMP(0, 0, "images/ddd.bmp");
		iShowBMP2(map_2_character_x, map_2_character_y, "images/mainChar.bmp", 255);
		for (int i = 0; i < vEnemies.size(); i++) {
			vEnemies[i].render();
			vEnemies[i].MoveTowards();
			if (dead) {
				vEnemies.pop_back();
				dead = false;
			}
		}
	}
	else if (play_Button && map2) {
		iShowBMP(0, 0, "images/Map1/2.bmp");
		cout << 1;
	}
}

void iMouseMove(int mx, int my) {}

void iPassiveMouseMove(int mx, int my) {}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		dead = true;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (home_menu) {
			// Code for home menu button clicks
		}
		else if (about_us) {
			if (mx >= 1201 && mx <= 1260 && my >= 653 && my <= 707) {
				about_us = false;
				home_menu = true;
			}
		}
		else if (play_Button && mx >= 0 && mx <= 1280 && my >= 715 && my <= 720 && map1) {
			map1 = false;
			map2 = true;
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		cout << mx << " " << my << endl;
	}
}

void iKeyboard(unsigned char key) {
	if (key == '\r') {
		start_screen = false;
		home_menu = true;
	}

	if (key == 'w' && play_Button && map2 && !home_menu) {
		map_2_character_x += 5;
	}

	// Other key movements...

	if (key == 'm') {
		music_on = !music_on;
		cout << music_on;
		if (music_on) {
			PlaySound("sounds/bgm.wav", NULL, SND_LOOP | SND_ASYNC);
		}
		else {
			PlaySound(0, 0, 0);
		}
	}
}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_RIGHT) {
		// Handle right arrow key
	}
	if (key == GLUT_KEY_LEFT) {
		// Handle left arrow key
	}
	if (key == GLUT_KEY_HOME) {
		// Handle home key
	}
}

void music_fun() {
	if (music_on) {
		PlaySound("sounds/bgm.wav", NULL, SND_LOOP | SND_ASYNC);
	}
	else {
		PlaySound(0, 0, 0);
	}
}

int main() {
	iInitialize(screen_width, screen_height, "Valar Morghulis");
	sayHello();
	iSetTimer(2000, drawEnemy);
	PlaySound("sounds/bgm.wav", NULL, SND_LOOP | SND_ASYNC);

	iStart();
	return 0;
}

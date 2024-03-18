#include "iGraphics.h"
#include <iostream>
#include <string.h>
#include <string>
#include "Library\screen.h"
#include <utility>
#include <vector>
#include <math.h>
#include <Windows.h> 
#define and &&
using namespace std;

#define screen_width 1280
#define screen_height 720

string s;
int image;
bool pause = false;
float speed = 20;
float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}
vector<char> name(20, '0');
// screen
bool start_screen = true;
bool home_menu = false;
bool music_on = true;
bool about_us = false;
bool instruction = false;
bool high_score = false;

bool play_Button = false;
bool map1 = false;
bool map2 = false;

int character_x = 327;
int character_y = 268;
int level = 1;
int enemy_count_max = 5;
bool dead = true;

// Counter variable
int hitCounter = 0;
char charImg[4][40] = {"images/mainChar Front.bmp", "images/mainCharBack.bmp", "images/mainCharLeft.bmp", "images/mainCharRight.bmp"};
int index = 3;
int enemy_killed = 0;

int health = 0;
int maxHealth;
int healthBar = 0;
int kill_count = 0;
char input_name[100];
int index_name = 0;
bool map_page = false;
bool gameover = false;
char names[6][20];
int scores[6];
char scoreChar[4];
char character_name[20];
int healthBarCalculation(int health)
{
	if(maxHealth != 0)
		healthBar = 300 * health / maxHealth;
	return healthBar;
}
void scoreCalculator(int kill_count, char* scoreChar, size_t size)
{
	sprintf_s(scoreChar, size, "%d", kill_count);
}
double calculateDistance(double x1, double y1, double x2, double y2)
{
	double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
	return distance;
}

void playerName()
{

	iText(605, 177, input_name, GLUT_BITMAP_TIMES_ROMAN_24);
}

void sortScores()
{
	for (int i = 0; i < 6 - 1; i++)
	{
		for (int j = i + 1; j < 6; j++)
		{
			if (scores[i] < scores[j])
			{
				swap(scores[i], scores[j]);
				swap(names[i], names[j]);
			}
		}
	}
}

void writeDefaultScores()
{
	FILE *fp;
	fopen_s(&fp, "HighScore.txt", "r+");

	if (fp == NULL)
	{
		cout << "HighScore.txt does not exist. creating default." << endl;
		fopen_s(&fp, "HighScore.txt", "w+");
		fprintf_s(fp, "Shovon 7\nZawad 6\nAriful 4\nDreamGirl 4\nMonkey 1\n");
	}
	fclose(fp);
	// cout << "Highcsore" << endl;
}
void readScoreFile()
{
	FILE *fp;
	fopen_s(&fp, "HighScore.txt", "r+");

	if (fp != NULL)
	{
		for (int i = 0; i < 5; i++)
		{
			fscanf_s(fp, "%s %d", names[i], 20, &scores[i]);
		}

		// sortScores();
		for (int i = 0; i < 5; i++)
		{ // show text in highscore
			//    printf_s("%s %d\n", names[i], scores[i]);
			char pointsS[20];
			sprintf_s(pointsS, "%d", scores[i]);

			iSetColor(255, 255, 255);
			iText(325, 455 - i * 90, names[i], GLUT_BITMAP_TIMES_ROMAN_24);
			iText(890, 455 - i * 90, pointsS, GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(0, 0, 0);
		}

		fclose(fp);
	}
}
void writeScoreFile()
{
	cout << "called" << endl;
	FILE *fp;
	fopen_s(&fp, "HighScore.txt", "r+");

	if (fp != NULL)
	{
		for (int i = 0; i < 5; i++)
		{
			fscanf_s(fp, "%s %d", names[i], 20, &scores[i]);
			cout << names[i] << scores[i];
			if (strcmp(names[i], character_name) == 0 && scores[i] == kill_count)
			{
				cout << "ruh";
				return;
			}
		}

		strcpy_s(names[5], character_name);
		scores[5] = kill_count;

		sortScores();

		fclose(fp);
		fopen_s(&fp, "HighScore.txt", "w");

		if (fp != NULL)
		{
			for (int i = 0; i < 6; i++)
				// cout << names[i] << scores[i];

				for (int i = 0; i < 5; i++)
				{ // show text in highscore
					fprintf_s(fp, "%s %d\n", names[i], scores[i]);
				}

			fclose(fp);
		}
		kill_count = 0;
	}
}
// structures
struct Bullet
{
	float x;
	float y;
	float speed;
	float direction;
	float mx, my;

	// Default constructor
	Bullet() : x(0), y(0), speed(0), direction(0), mx(0), my(0) {}

	Bullet(float startX, float startY, float bulletSpeed, float mxx, float myy)
		: x(startX), y(startY), speed(bulletSpeed), mx(mxx), my(myy) {}

	void bulletShoot()
	{
		int var = 1;
		if (mx > x && x <= mx)
			x += speed * var;
		else if (mx < x && x >= mx)
			x -= speed * var;
		if (my > y && y <= my)
			y += speed * var;
		else if (my < y && y >= my)
			y -= speed * var;
	}
};

struct Enemy
{
	int x;
	int y;
	bool isShooting;
	Bullet enemyBullet;

	Enemy()
	{
		x = rand() % 1280;
		y = rand() % 720;
		isShooting = false;
	}

	void render()
	{
		iShowBMP2(x, y, "images/enemy.bmp", 16777215);
	}

	void MoveTowards(int targetX, int targetY, vector<Enemy> &vEnemies)
	{

		float deltaX = targetX - x;
		float deltaY = targetY - y;
		float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

		float smoothingFactor = 0.001;
		float speed = min(max(1.0, distance / 50.0), 5.0);

		float angle = atan2(targetY - y, targetX - x);

		float newX = lerp(x, targetX, smoothingFactor);
		float newY = lerp(y, targetY, smoothingFactor);

		float reducedSpeedFactor = 0.5;
		newX += cos(angle) * (speed * reducedSpeedFactor);
		newY += sin(angle) * (speed * reducedSpeedFactor);

		// Check if the new position is too close to any other enemy
		bool tooClose = false;
		for (size_t i = 0; i < vEnemies.size(); i++)
		{
			if (this != &vEnemies[i])
			{ // Skip comparing with itself
				float distanceToOtherEnemy = sqrt(pow(newX - vEnemies[i].x, 2) + pow(newY - vEnemies[i].y, 2));
				if (distanceToOtherEnemy < 30)
				{
					tooClose = true;
					break;
				}
			}
		}

		if (!tooClose)
		{

			if (pause == true)
			{
				x = x;
				y = y;
			}
			else
			{
				x = newX;
				y = newY;
			}
		}
		if (abs(calculateDistance(x, y, character_x, character_y) <= 100))
		{
			health--;
		}
		// Check if the enemy is close to the main character
	}
};

// global variables
vector<Enemy> vEnemies(10);
vector<Bullet> bullets;

// functions
void iDraw()
{
	// cout << map1 << endl;
	iClear();
	// cout << health << endl;
	// cout << input_name << endl;
	// cout << character_name << endl;
	// cout << hitCounter << endl;
	if (start_screen)
	{
		iShowBMP2(0, 0, "images/start.bmp", 0);
		playerName();
	}
	else if (home_menu)
	{
		iShowBMP(0, 0, "images/main.bmp");
	}
	else if (about_us)
	{
		iShowBMP(0, 0, "images/abc.bmp");
	}

	else if (instruction)
	{
		iShowBMP(0, 0, "images/ins.bmp");
	}

	else if (play_Button)
	{

		iShowBMP(0, 0, "images/mapSelect.bmp");
	}
	else if (high_score)
	{
		iShowBMP(0, 0, "images/HighScore.bmp");
		readScoreFile();
	}
	else if (map1)
	{

		iShowBMP(0, 0, "images/firstMap.bmp");
		if (health > 0)
		{
			iText(50, 560, "Player's Health", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(255, 0, 0);
			iRectangle(50, 530, 300, 20);
			iFilledRectangle(50, 530, healthBarCalculation(health), 20);
			iText(1000, 560, "Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
			scoreCalculator(kill_count, scoreChar, 4);
			iText(1080, 560, scoreChar , GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (pause == true)
		{
			iShowBMP(400, 213, "images/pause.bmp");
		}
		iShowBMP2(character_x, character_y, charImg[index], 16777215);

		// Draw enemies
		for (size_t i = 0; i < vEnemies.size(); i++)
		{
			vEnemies[i].render();
			vEnemies[i].MoveTowards(character_x, character_y, vEnemies); // Pass vEnemies as an argument
		}

		// Draw bullets shot by the player
		for (size_t i = 0; i < bullets.size(); i++)
		{
			iShowBMP2(bullets[i].x, bullets[i].y, "images/bullet.bmp", 16777215);
		}

		// Draw bullets shot by the enemy
		for (size_t i = 0; i < vEnemies.size(); i++)
		{
			if (vEnemies[i].isShooting)
			{
				iShowBMP2(vEnemies[i].enemyBullet.x, vEnemies[i].enemyBullet.y, "images/bullet.bmp", 16777215);
			}
		}

		if (health <= 0)
		{
			gameover = true;
			writeScoreFile();
			map1 = false;
		}
	}
	else if (map2)
	{
		iShowBMP(0, 0, "images/secondMap.bmp");
		if (health > 0)
		{
			iText(50, 560, "Player's Health", GLUT_BITMAP_TIMES_ROMAN_24);
			iSetColor(255, 0, 0);
			iRectangle(50, 530, 300, 20);
			iFilledRectangle(50, 530, healthBarCalculation(health), 20);
			iText(1000, 560, "Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
			scoreCalculator(kill_count, scoreChar, 4);
			iText(1080, 560, scoreChar, GLUT_BITMAP_TIMES_ROMAN_24);
		}
		if (pause == true)
		{
			iShowBMP(400, 213, "images/pause.bmp");
		}
		iShowBMP2(character_x, character_y, charImg[index], 16777215);

		// Draw enemies
		for (size_t i = 0; i < vEnemies.size(); i++)
		{
			vEnemies[i].render();
			vEnemies[i].MoveTowards(character_x, character_y, vEnemies); // Pass vEnemies as an argument
		}

		// Draw bullets shot by the player
		for (size_t i = 0; i < bullets.size(); i++)
		{
			iShowBMP2(bullets[i].x, bullets[i].y, "images/bullet.bmp", 16777215);
		}

		// Draw bullets shot by the enemy
		for (size_t i = 0; i < vEnemies.size(); i++)
		{
			if (vEnemies[i].isShooting)
			{
				iShowBMP2(vEnemies[i].enemyBullet.x, vEnemies[i].enemyBullet.y, "images/bullet.bmp", 16777215);
			}
		}

		if (health <= 0)
		{
			gameover = true;
			writeScoreFile();
			map2 = false;
		}
	}
	else if (gameover)
	{
		iShowBMP(0, 0, "images/gameover.bmp");
	}
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		dead = true;
		float bulletSpeed = 5;
		float bulletDirection = 0;
		bullets.push_back(Bullet(character_x, character_y, bulletSpeed, mx, my));
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (!vEnemies.empty())
		{
			vEnemies.pop_back();
			cout << mx << " " << my << endl;
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (home_menu)
		{

			// Handle menu clicks
			if ((mx >= 113 && mx <= 268) && (my >= 524 && my <= 573))
			{
				home_menu = false;
				play_Button = true;
				// map1 = true;
				// map_page = true;
				// health = 10000;
			}

			else if ((mx >= 114 && mx <= 431) && (my >= 425 && my <= 519))
			{
				cout << 3;
				home_menu = false;
				instruction = true;
			}
			else if ((mx >= 105 && mx <= 355) && (my >= 326 && my <= 365))
			{
				home_menu = false;
				about_us = true;
			}
			else if ((mx >= 111 && mx <= 411) && (my >= 227 && my <= 269))
			{
				cout << "high_score";
				home_menu = false;
				high_score = true;
			}
			else if ((mx >= 110 && mx <= 232) && (my >= 132 && my <= 172))
			{
				cout << "exit";
				exit(1);
			}
		}

		// After home menu == 0
		else if (play_Button)
		{
			//char character_name[20];
			map_page = true;
			play_Button = false;
		}
		else if (map_page && (mx >= 0 && mx <= 640) && (my >= 258 && my <= 720))
		{
			map_page = false;
			map1 = true;
			if (level == 1){
				health = 3000;
				maxHealth = health;
			}

			else if (level == 2)
			{
				health = 2000;
				maxHealth = health;
			}
			else if (level == 3)
			{
				health = 1000;
				maxHealth = health;
			}
		}
		else if (map_page && (mx >= 640 && mx <= 1280) && (my >= 258 && my <= 720))
		{
			map_page = false;
			map2 = true;
			if (level == 1){
				health = 3000;
				maxHealth = health;
			}
				
			else if (level == 2)
			{
				health = 2000;
				maxHealth = health;
			}
			else if (level == 3)
			{
				health = 1000;
				maxHealth = health;
			}
		}
		else if (about_us && mx >= 1201 && mx <= 1260 && my >= 653 && my <= 707)
		{
			about_us = false;
			home_menu = true;
		}
		else if (instruction && mx >= 1201 && mx <= 1260 && my >= 653 && my <= 707)
		{
			instruction = false;
			home_menu = true;
		}
		else if (high_score && mx >= 1190 && mx <= 1260 && my >= 630 && my <= 707)
		{
			high_score = false;
			home_menu = true;
		}


		/*if (play_Button && mx >= 0 && mx <= 1280 && my >= 715 && my <= 720 && map1) {
			map1 = false;
			map2 = true;
		} */
	}
}

void iUpdate()
{
	// Move bullets shot by the player
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i].bulletShoot();

		// Check for bullet-enemy collision
		for (size_t j = 0; j < vEnemies.size(); j++)
		{
			if (bullets[i].x >= vEnemies[j].x &&
				bullets[i].x <= vEnemies[j].x + 50 &&
				bullets[i].y >= vEnemies[j].y &&
				bullets[i].y <= vEnemies[j].y + 50)
			{
				kill_count++;
				cout << "Kill " << kill_count << endl;
				// Bullet hit the enemy
				bullets.erase(bullets.begin() + i);
				vEnemies.erase(vEnemies.begin() + j);

				// Increase the counter when an enemy is killed
				enemy_killed++;

				// Check if all enemies are killed
				if (vEnemies.empty())
				{
					// Respawn 10 enemies
					for (int k = 0; k < 10; k++)
					{
						vEnemies.push_back(Enemy());
					}
				}

				// Check if the total number of enemies killed reaches 50
				if (enemy_killed == 30)
				{
					//kono ekdin lekhbo 
				}
				break;
			}
		}
	}

	// Move enemies and handle shooting logic
	for (size_t i = 0; i < vEnemies.size(); i++)
	{
		vEnemies[i].MoveTowards(character_x, character_y, vEnemies);

		// If the enemy is shooting, update the bullet position
		if (vEnemies[i].isShooting)
		{
			vEnemies[i].enemyBullet.bulletShoot();

			// Check for bullet-main character collision
			if (character_x >= vEnemies[i].enemyBullet.x &&
				character_x <= vEnemies[i].enemyBullet.x + 2 &&
				character_y >= vEnemies[i].enemyBullet.y &&
				character_y <= vEnemies[i].enemyBullet.y + 2)
			{
				// Main character hit by enemy bullet, handle accordingly
				// For example, set player health, game over, etc.
				// In this case, decrease the hitCounter when the bullet hits the main character
				enemy_killed--; // Decrease the counter when hit by enemy bullet
			}
			else
			{
				// Bullet missed the main character, move it beyond the screen
				if (vEnemies[i].enemyBullet.x < 0 || vEnemies[i].enemyBullet.x > screen_width ||
					vEnemies[i].enemyBullet.y < 0 || vEnemies[i].enemyBullet.y > screen_height)
				{
					vEnemies[i].isShooting = false; // Stop shooting if the bullet goes off-screen
				}
			}
		}
	}
}

void iKeyboard(unsigned char key)
{

	if (gameover == true && key == '\r')
	{
		gameover = false;
		start_screen = 0;
		home_menu = 1;
	}

	if (start_screen == true and key != '\b')
	{
		if (key == ' ')
		{
			input_name[index_name] = '_';
			index_name++;
			input_name[index_name] = '\0';
			character_name[index_name] = '_';
		}
		else if (key == '\r')
		{
			// fopen_s(&playerInfo, FileDirectory, "a");
			// fprintf(playerInfo, "%s\n", name);
			// printf("File is writen\n");
			// fclose(playerInfo);
			// readScoreFile();
			// names.push_back()
			start_screen = 0;
			home_menu = 1;
			// key = '\\';
		}

		else
		{
			input_name[index_name] = key;
			character_name[index_name] = key;
			index_name++;
			input_name[index_name] = '\0';
		}
	}

	else
	{
		if (index_name <= 0)
		{
			index_name = 0;
		}
		else
		{
			index_name--;
		}

		input_name[index_name] = '\0';
	}

	//if (key == 'w' and play_Button == 1 and map2 == 1 and home_menu == 0)
	//{
		//character_x += 5;
	//}
	if (key == '1')
	{
		level = 1;
	}
	else if (key == '2')
	{
		level = 2;
	}
	else if (key == '3')
	{
		level = 3;
	}

	if (key == 'w')
	{
		character_y += 15;
		index = 0;
	}

	if (key == 's')
	{
		character_y -= 15;
		index = 1;
	}

	if (key == 'a')
	{
		character_x -= 15;
		index = 2;
	}

	if (key == 'd')
	{
		character_x += 15;
		index = 3;
	}

	if (key == 'm')
	{
		music_on = music_on ^ 1;
		// cout << music_on;
		if (music_on == 0)
		{
			PlaySound(0, 0, 0);
		}
		else
		{
			PlaySound("sounds/bgm.wav", NULL, SND_LOOP | SND_ASYNC);
		}
	}

	if (key == 'p')
	{
		pause = true;
	}
	if (key == 'r')
	{
		pause = false;
	}
}

void iSpecialKeyboard(unsigned char key)
{
	// Implement your special keyboard logic here
}

void iMouseMove(int mx, int my)
{
	// Implement your mouse move logic here
}

void iPassiveMouseMove(int mx, int my)
{
	// Implement your passive mouse move logic here
}

void bulletGayeb()
{
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets.erase(bullets.begin() + i);
	}
}

int main()
{
	iInitialize(screen_width, screen_height, "Valar Morghulis");

	// Set up a timer to call iUpdate function every 16 milliseconds (adjust as needed)
	iSetTimer(16, iUpdate);
	iSetTimer(1500, bulletGayeb);
	PlaySound("sounds/bgm.wav", NULL, SND_LOOP | SND_ASYNC);
	writeDefaultScores();
	iStart();
	return 0;
}

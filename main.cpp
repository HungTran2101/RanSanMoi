//Code by TranNhatHung
//Last edited 6/7/2020

#include"kmin_console.h"
#include<ctime>
#include<string.h>

#define LEN 1
#define XUONG 2
#define TRAI 3
#define PHAI 4
#define TUONG_TREN 4 //tường trên - tường dưới = 21
#define TUONG_DUOI TUONG_TREN+21
#define TUONG_TRAI 15 //tường trái - tường phải = 90
#define TUONG_PHAI TUONG_TRAI+90
#define BEGINNER 250
#define EASY 200
#define MEDIUM 150
#define HARD 100
#define EXPERT 50
#define CLASSIC FALSE
#define MODERN TRUE

struct ToaDo
{
	int x, y;
};

struct listScore
{
	char name[30] = "";
	int score;
};

ToaDo ConRan[100]; //Con rắn dài tối đa 100 đốt
int soDot = 3; //Số đốt rắn
int Diem = 0, speed = HARD;
bool quitOption = FALSE;
listScore Player[6];
bool gamemode = CLASSIC;
int CountFood = 0;

void RunningGame();
void khoiTaoRan(); //Tạo điểm xuất phát con rắn
void hienThiRan(int huong, ToaDo DotCuoi); //Xuất con rắn ra màn hình
ToaDo diChuyen(int huong); //Con rắn di chuyển theo hướng đã điều khiển
int dieuKhien(int& huong,ToaDo food); //Điều khiển hướng con rắn
void veTuong(); //Vẽ map
bool checkEndGame(); //Kiểm tra điều kiện thua
void XuLyThua(); //Xử lí sau khi thua game
bool checkEaten(ToaDo food); //kiểm tra rắn đã ăn food chưa
ToaDo SpawnFood(int CountFood); 
void GiaoDienGame();
void GiaoDienLevel();
int SelectLevel(int x, int y);
void GiaoDienScore();
bool kiemTraTrungFood(int foodx, int foody);
void SortScore();
bool GiaodienGamemode();
int GiaodienPause(int key, ToaDo food);

int main()
{
	srand((int)time(NULL));
	noCursorType();
	veTuong();
	for (int i = 0; i < 100; i=i+(1+rand()%10))
	{
		setTextColor(1 + rand() % 15);
		gotoXY(TUONG_TRAI + 33, TUONG_TREN + 10);
		printf("GAME IS LOADING... %d %%", i);
		Sleep(150+rand()%51);
	}
	gotoXY(TUONG_TRAI + 33, TUONG_TREN + 10);
	printf("                        ");
	GiaoDienGame();
	return 0;
}
void GiaoDienGame()
{
	// Cập nhật điểm người chơi
	FILE* inputScore;
	fopen_s(&inputScore, "HighScore.txt", "r");
	if (!(inputScore == NULL))
	{
		for (int i = 0; i < 5; i++)
		{
			fscanf(inputScore, "%s", Player[i].name);
			fscanf(inputScore, "%d", &Player[i].score);
		}
	}
	fclose(inputScore);

	int Option1x = TUONG_TRAI + 40; // Tọa độ option đầu tiên
	int Option1y = TUONG_TREN + 9; //
	int pointery = Option1y; // Tọa độ y của con trỏ

	//vòng lặp chính của game
	while (quitOption == FALSE)
	{
		soDot = 3;
		Diem = 0;
		veTuong();
		gotoXY(Option1x, Option1y);
		printf("PLAY GAME!");
		gotoXY(Option1x, Option1y + 3);
		printf("SPEED ");
		switch (speed)
		{
		case BEGINNER:printf("(BEGINNER)"); break;
		case EASY:printf("(EASY)"); break;
		case MEDIUM:printf("(MEDIUM)"); break;
		case HARD:printf("(HARD)"); break;
		case EXPERT:printf("(EXPERT)"); break;
		}
		gotoXY(Option1x, Option1y + 6);
		printf("HIGHSCORE");
		gotoXY(Option1x, Option1y + 9);
		printf("QUIT GAME");
		gotoXY(Option1x - 6, Option1y - 5);
		setTextColor(6);
		printf("Code by TranNhatHung");

		gotoXY(Option1x - 3, pointery);
		printf(">");
		while (1)
		{
			gotoXY(TUONG_TRAI + 41, TUONG_TREN + 2);
			setTextColor(1 + rand() % 15);
			printf("SNAKE");
			Sleep(50);
			int key = inputKey();
			if (key == 13 && pointery == Option1y) // 13 = ENTER
			{
				setTextColor(15);
				bool flagEsc = GiaodienGamemode();
				if (flagEsc == TRUE)
				{
					clrscr();
					break;
				}
				setTextColor(15);
				RunningGame();
				break;
			}
			else if (key == 13 && pointery == Option1y + 3)
			{
				GiaoDienLevel();
				clrscr();
				break;
			}
			else if (key == 13 && pointery == Option1y + 6)
			{
				GiaoDienScore();
				clrscr();
				break;
			}
			else if(key == 13 && pointery == Option1y + 9)
			{
				quitOption = TRUE;
				break;
			}
			else if ((key == KEY_DOWN || key == 's' || key == 'S') && pointery <= Option1y + 9) //Điều khiển lựa chọn đi xuống
			{
				gotoXY(Option1x - 3, pointery);
				printf(" ");
				if(pointery == Option1y + 9)
					pointery = Option1y;
				else pointery = pointery + 3;
				gotoXY(Option1x - 3, pointery);
				printf(">");
			}
			else if ((key == KEY_UP || key == 'w' || key == 'W') && pointery >= Option1y) //Điều khiển lựa chọn đi lên
			{
				gotoXY(Option1x - 3, pointery);
				printf(" ");
				if(pointery == Option1y)
					pointery = pointery + 9;
				else pointery = pointery - 3;
				gotoXY(Option1x - 3, pointery);
				printf(">");
			}
		}
	}
	clrscr();
	gotoXY(TUONG_TRAI + 30, TUONG_TREN + 9);
	setTextColor(10);
	printf("Thanks for playing the game\n");
	gotoXY(TUONG_TRAI + 32, TUONG_TREN + 11);
	setTextColor(4);
	printf(" Code by TranNhatHung");
	setTextColor(15);
	veTuong();
}
void RunningGame()
{
	//noCursorType();
	clrscr();
	khoiTaoRan();
	int huongDiChuyen = PHAI;
	int flag = 0;
	veTuong();
	ToaDo food = SpawnFood(CountFood);
	gotoXY(TUONG_TRAI, TUONG_TREN - 2);
	printf("Point: %d", Diem);
	gotoXY(TUONG_PHAI - 19, TUONG_TREN - 2);
	printf("Highest point: %d", Player[0].score);
	gotoXY(TUONG_TRAI + 30, TUONG_DUOI + 1);
	printf("Press Space to pause the game");
	gotoXY(TUONG_TRAI + 33, TUONG_DUOI + 2);
	printf("Press ESC exit to MENU");

	while (1)
	{
		ToaDo DotCuoi = diChuyen(huongDiChuyen); //Lưu vị trí đốt cuối để xóa khi di chuyển
		hienThiRan(huongDiChuyen, DotCuoi);
		if (flag == 1) // Đã pause game
		{
			setTextColor(12);
			gotoXY(TUONG_TRAI + 35, TUONG_TREN - 1);
			printf("CONTINUE IN: ");
			for (int i = 3; i > 0; i--)
			{
				gotoXY(TUONG_TRAI + 48, TUONG_TREN - 1);
				printf("%d", i);
				Sleep(1000);
			}
			gotoXY(TUONG_TRAI + 35, TUONG_TREN - 1);
			printf("              ");
			setTextColor(15);
		}
		else if (flag == -1) //đã ấn ESC
		{
			break;
		}
		if (checkEaten(food) == TRUE)
		{
			CountFood++;
			if (CountFood == 6)
			{
				Diem += 200;
				gotoXY(TUONG_TRAI, TUONG_TREN - 2);
				printf("Point: %d", Diem);
				soDot++;
				food = SpawnFood(CountFood);
				CountFood = 0;
			}
			else
			{
				Diem += 100;
				gotoXY(TUONG_TRAI, TUONG_TREN - 2);
				printf("Point: %d", Diem);
				soDot++;
				food = SpawnFood(CountFood);
			}
		}
		if (checkEndGame() == TRUE)
			break;
		Sleep(speed);
		flag = dieuKhien(huongDiChuyen,food);
	}
	CountFood = 0;
	if(flag != -1)
		XuLyThua();
	clrscr();
}
void khoiTaoRan()
{
	int x = 20;
	ConRan[0].x = TUONG_TRAI + x;
	ConRan[1].x = TUONG_TRAI + (x - 1);
	ConRan[2].x = TUONG_TRAI + (x - 2);
	ConRan[0].y = ConRan[1].y = ConRan[2].y = TUONG_TREN + 5;
}
void hienThiRan(int huong, ToaDo DotCuoi)
{
	for (int i = soDot-1; i >= 0; i--)
	{
		gotoXY(ConRan[i].x, ConRan[i].y);
		if (i == 0)
		{
			if (huong == LEN)
			{
				printf("^");
				break;
			}
			else if (huong == XUONG)
			{
				printf("V");
				break;
			}
			else if (huong == TRAI)
			{
				printf("<");
				break;
			}
			else if (huong == PHAI)
			{
				printf(">");
				break;
			}
		}
		printf("%c", 254);
	}
	gotoXY(DotCuoi.x, DotCuoi.y);
	printf(" ");
}
ToaDo diChuyen(int huong)
{
	ToaDo DotCuoi = ConRan[soDot - 1];

	for (int i = soDot-1; i > 0; i--)
	{
		ConRan[i] = ConRan[i - 1];
	}

	switch (huong)
	{
	case LEN:ConRan[0].y--; break;
	case XUONG:ConRan[0].y++; break;
	case TRAI:ConRan[0].x--; break;
	case PHAI:ConRan[0].x++; break;
	}

	if (gamemode == MODERN)
	{
		if (ConRan[0].x == TUONG_TRAI)
			ConRan[0].x = TUONG_PHAI - 1;
		else if (ConRan[0].x == TUONG_PHAI)
			ConRan[0].x = TUONG_TRAI + 1;
		else if (ConRan[0].y == TUONG_TREN)
			ConRan[0].y = TUONG_DUOI - 1;
		else if (ConRan[0].y == TUONG_DUOI)
			ConRan[0].y = TUONG_TREN + 1;
	}
	return DotCuoi;
}
int dieuKhien(int& huong, ToaDo food)
{
	int key = inputKey();
	if ((key == KEY_UP || key =='w' || key == 'W') && huong != XUONG)
		huong = LEN;
	else if ((key == KEY_DOWN || key == 's' || key == 'S') && huong != LEN)
		huong = XUONG;
	else if ((key == KEY_LEFT || key == 'a' || key == 'A') && huong != PHAI)
		huong = TRAI;
	else if ((key == KEY_RIGHT || key == 'd' || key == 'D') && huong != TRAI)
		huong = PHAI;
	else if (key == 32) //Vẽ màn hình game pause
	{
		return GiaodienPause(key,food);
	}
	else if (key == 27) //27 = ESC
	{
		return -1;
	}
	return 0;
}
void veTuong()
{
	for (int i = TUONG_TRAI; i < TUONG_PHAI; i++)
	{
		gotoXY(i, TUONG_TREN);
		printf("%c", 223);
		gotoXY(i, TUONG_DUOI);
		printf("%c", 220);
	}
	for (int i = TUONG_TREN; i < TUONG_DUOI+1; i++)
	{
		gotoXY(TUONG_TRAI, i);
		printf("%c", 222);
		gotoXY(TUONG_PHAI, i);
		printf("%c", 221);
	}
}
bool checkEndGame()
{
	if (gamemode == CLASSIC)
	{
		if (ConRan[0].x == TUONG_TRAI || ConRan[0].x == TUONG_PHAI)
			return TRUE;
		else if (ConRan[0].y == TUONG_TREN || ConRan[0].y == TUONG_DUOI)
			return TRUE;
		for (int i = 1; i < soDot; i++)
			if (ConRan[0].x == ConRan[i].x && ConRan[0].y == ConRan[i].y)
				return TRUE;
	}
	else
	{
		for (int i = 1; i < soDot; i++)
			if (ConRan[0].x == ConRan[i].x && ConRan[0].y == ConRan[i].y)
				return TRUE;
	}
	return FALSE;
}
void XuLyThua()
{
	Sleep(500);
	clrscr();
	gotoXY(TUONG_TRAI + 39,TUONG_TREN + 5);
	setTextColor(4);
	printf("GAME OVER!");
	setTextColor(15);
	veTuong();
	
	//Kiểm tra điểm người chơi có lớn hơn bảng highscore
	int flag = 0;
	for (int i = 0; i < 5; i++)
		if (Diem > Player[i].score)
		{
			flag = 1;
			break;
		}
	if (flag == 1)
	{
		char tempName[30];
		gotoXY(TUONG_TRAI + 20, TUONG_TREN + 9);
		printf("Please enter your name (No spacebar, max 9 characters)");
		gotoXY(TUONG_TRAI + 33, TUONG_TREN + 13);
		printf("Press ENTER when done");
		gotoXY(TUONG_TRAI + 36, TUONG_TREN + 7);
		setTextColor(11);
		printf("Your score: %d", Diem);
		gotoXY(TUONG_TRAI + 35, TUONG_TREN + 11);
		setTextColor(15);
		printf("NAME: ");
		yesCursorType();
		scanf("%s", tempName);
		noCursorType();
		strcpy(Player[5].name, tempName);
		Player[5].score = Diem;

		// Sắp xếp điểm người chơi từ cao đến thấp
		SortScore();

		//Cập nhật điểm người chơi
		FILE* outputScore;
		fopen_s(&outputScore, "HighScore.txt", "w");
		for (int i = 0; i < 5; i++)
			fprintf(outputScore, "%s %d\n", Player[i].name, Player[i].score);
		fclose(outputScore);
	}
	else
	{
		gotoXY(TUONG_TRAI + 38, TUONG_TREN + 10);
		setTextColor(11);
		printf("Your score: %d", Diem);
		gotoXY(TUONG_TRAI + 33, TUONG_TREN + 12);
		setTextColor(15);
		printf("Press ENTER to continue");
		while (1)
		{
			int key = inputKey();
			if (key == 13) //13 = ENTER
				return;
		}
	}
}
ToaDo SpawnFood(int CountFood)
{
	int x, y;
	do
	{
		x = TUONG_TRAI + 1 + rand() % (TUONG_PHAI - TUONG_TRAI - 1);
		y = TUONG_TREN + 1 + rand() % (TUONG_DUOI - TUONG_TREN - 1);
	} while (kiemTraTrungFood(x, y) == TRUE);
	gotoXY(x, y);
	if (CountFood == 5)
		printf("Q");
	else
		printf("%c", 229);
	return ToaDo{ x,y };
}
bool checkEaten(ToaDo food)
{
	if (ConRan[0].x == food.x && ConRan[0].y == food.y)
		return TRUE;
	else
		return FALSE;
}
void GiaoDienLevel()
{
	clrscr();
	int leveloption1x = TUONG_TRAI + 40;
	int leveloption1y = TUONG_TREN + 7;
	gotoXY(leveloption1x, leveloption1y);
	printf("BEGINNER");
	gotoXY(leveloption1x, leveloption1y + 2);
	printf("EASY");
	gotoXY(leveloption1x, leveloption1y + 4);
	printf("MEDIUM");
	gotoXY(leveloption1x, leveloption1y + 6);
	printf("HARD");
	gotoXY(leveloption1x, leveloption1y + 8);
	printf("EXPERT");
	gotoXY(leveloption1x - 8, leveloption1y + 10);
	printf("Press ESC to back to menu");
	gotoXY(TUONG_TRAI + 36, TUONG_TREN + 5);
	setTextColor(1 + rand() % 15);
	printf("Choose the speed:");
	veTuong();

	int pointerlevely = leveloption1y;
	gotoXY(leveloption1x - 3, pointerlevely);
	printf(">");
	while (1)
	{
		int key = inputKey();
		if (key == 13)
		{
			int x = SelectLevel(pointerlevely,leveloption1y);
			switch (x)
			{
			case 1:speed = BEGINNER; break;
			case 2:speed = EASY; break;
			case 3:speed = MEDIUM; break;
			case 4:speed = HARD; break;
			case 5:speed = EXPERT; break;
			}
			break;
		}
		else if (key == 27)
		{
			clrscr();
			break;
		}
		else if ((key == KEY_DOWN || key == 's' || key == 'S') && pointerlevely <= leveloption1y + 8)
		{
			gotoXY(leveloption1x - 3, pointerlevely);
			printf(" ");
			if(pointerlevely == leveloption1y + 8)
				pointerlevely = leveloption1y;
			else pointerlevely = pointerlevely + 2;
			gotoXY(leveloption1x - 3, pointerlevely);
			printf(">");
		}
		else if ((key == KEY_UP || key == 'w' || key == 'W') && pointerlevely >= leveloption1y)
		{
			gotoXY(leveloption1x - 3, pointerlevely);
			printf(" ");
			if(pointerlevely == leveloption1y)
				pointerlevely = leveloption1y + 8;
			else pointerlevely = pointerlevely - 2;
			gotoXY(leveloption1x - 3, pointerlevely);
			printf(">");
		}
	}
}
int SelectLevel(int x, int y)
{
	if (x == y)
		return 1;
	else if (x == y + 2)
		return 2;
	else if (x == y + 4)
		return 3;
	else if (x == y + 6)
		return 4;
	else
		return 5;
}
void GiaoDienScore()
{
	clrscr();
	veTuong();
	for (int i = 0; i < 5; i++)
	{
		gotoXY(TUONG_TRAI + 37, TUONG_TREN + 7 + 2 * i);
		printf("%-5d %s", Player[i].score, Player[i].name);
	}
	gotoXY(TUONG_TRAI + 28, TUONG_TREN + 18);
	printf("Press ESC or ENTER go back to MENU");
	while (1)
	{
		setTextColor(1 + rand() % 15);
		gotoXY(TUONG_TRAI + 38, TUONG_TREN + 5);
		printf("HIGHSCORE:");
		Sleep(100);
		int key = inputKey();
		if (key == 27 || key == 13) // 27 = ESC | 13 ENTER
			break;
	}
}
bool kiemTraTrungFood(int foodx, int foody)
{
	for (int i = 0; i < soDot; i++)
	{
		if (foodx == ConRan[i].x && foody == ConRan[i].y)
			return TRUE;
	}
	return FALSE;
}
void SortScore()
{
	for(int i=0;i<=5;i++)
		for(int j=i+1;j<=5;j++)
		if (Player[i].score < Player[j].score)
		{
			listScore temp = Player[i];
			Player[i] = Player[j];
			Player[j] = temp;
		}
}
bool GiaodienGamemode()
{
	clrscr();
	veTuong();
	int optiongm1x = TUONG_TRAI + 26;
	int pointerx = optiongm1x - 3;
	gotoXY(optiongm1x, TUONG_TREN + 11);
	printf("CLASSIC");
	gotoXY(optiongm1x + 30, TUONG_TREN + 11);
	printf("MODERN");
	gotoXY(pointerx, TUONG_TREN + 11);
	printf(">");
	gotoXY(TUONG_TRAI + 31, TUONG_TREN + 13);
	printf("Press ESC to back to menu");
	setTextColor(1 + rand() % 15);
	gotoXY(TUONG_TRAI + 36, TUONG_TREN + 8);
	printf("SELECT GAME MODE:");

	while (1)
	{
		int key = inputKey();
		if (key == KEY_RIGHT && pointerx < optiongm1x + 27)
		{
			gotoXY(pointerx, TUONG_TREN + 11);
			printf(" ");
			pointerx += 30;
			gotoXY(pointerx, TUONG_TREN + 11);
			printf(">");
		}
		else if (key == KEY_LEFT && pointerx > optiongm1x)
		{
			gotoXY(pointerx, TUONG_TREN + 11);
			printf(" ");
			pointerx -= 30;
			gotoXY(pointerx, TUONG_TREN + 11);
			printf(">");
		}
		else if (key == 13)
		{
			if (pointerx == optiongm1x - 3)
				gamemode = CLASSIC;
			else
				gamemode = MODERN;
			break;
		}
		else if (key == 27)
			return TRUE;
	}
	return FALSE;
}
int GiaodienPause(int key, ToaDo food)
{
	clrscr();
	veTuong();
	gotoXY(TUONG_TRAI + 38, TUONG_TREN + 10);
	printf("Game Paused!");
	gotoXY(TUONG_TRAI + 32, TUONG_TREN + 11);
	printf("Press Enter to countinue...");
	gotoXY(TUONG_TRAI, TUONG_TREN - 2);
	printf("Point: %d", Diem);
	gotoXY(TUONG_PHAI - 19, TUONG_TREN - 2);
	printf("Highest point: %d", Player[0].score);
	do
	{
		key = inputKey();
	} while (key != 13);
	clrscr();
	veTuong();
	gotoXY(TUONG_TRAI, TUONG_TREN - 2);
	printf("Point: %d", Diem);
	gotoXY(TUONG_PHAI - 19, TUONG_TREN - 2);
	printf("Highest point: %d", Player[0].score);
	gotoXY(TUONG_TRAI + 30, TUONG_DUOI + 1);
	printf("Press Space to pause the game");
	gotoXY(TUONG_TRAI + 33, TUONG_DUOI + 2);
	printf("Press ESC exit to MENU");
	gotoXY(food.x, food.y);
	if (CountFood == 5)
		printf("Q");
	else printf("%c", 229);
	return 1;
}
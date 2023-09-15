#include<iostream>
#include<conio.h>
#include<direct.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std; 
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

class game{
    public:
    static void gotoxy(int x, int y);
    static void setcursor(bool visible, DWORD size);
    static void drawBorder();
    static void genEnemy(int ind);
    static void drawEnemy(int ind);
    static void eraseEnemy(int ind);
    static void resetEnemy(int ind);
    static void genBullet();
    static void moveBullet();
    static void drawBullets();
    static void eraseBullets();
    static void eraseBullet(int i);
    static void drawship();
    static void eraseship();
    static int collision();
    static int bulletHit();
    static void gameover();
    static void updateScore();
    static void instructions();
    static void play();
};


int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char ship[3][5] = { '^',' ','^',' ','^',
					'^','^','*','^','^',
					'^','*','*','*','^' }; 
int shipPos = WIN_WIDTH/2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;

void game::gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void game::setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
void game::drawBorder(){ 
	
	for(int i=0; i<SCREEN_WIDTH; i++){ 
		game::gotoxy(i,SCREEN_HEIGHT); cout<<"±";
	}
	
	for(int i=0; i<SCREEN_HEIGHT; i++){
		game::gotoxy(0,i); cout<<"±";
		game::gotoxy(SCREEN_WIDTH,i); cout<<"±";
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		game::gotoxy(WIN_WIDTH,i); cout<<"±";
	}
}
void game::genEnemy(int ind){
	enemyX[ind] = 3 + rand()%(WIN_WIDTH-10);  
}
void game::drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		game::gotoxy(enemyX[ind], enemyY[ind]);   cout<<" ^^ ";  
		game::gotoxy(enemyX[ind], enemyY[ind]+1); cout<<" oo "; 
		game::gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"v--v"; 
		game::gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"^^^^"; 
		 
	} 
}
void game::eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		game::gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";  
		game::gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    "; 
		game::gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    "; 
		game::gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    "; 
	} 
}
void game::resetEnemy(int ind){
	game::eraseEnemy(ind);
	enemyY[ind] = 4;
	game::genEnemy(ind);
}
void game::genBullet(){
	bullets[bIndex][0] = 22;
	bullets[bIndex][1] = shipPos;
	bullets[bIndex][2] = 22; 
	bullets[bIndex][3] = shipPos+4; 
	bIndex++;
	if( bIndex == 20)
		bIndex = 0;
}
void game::moveBullet(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 2 )
			bullets[i][0]--;
		else
			bullets[i][0] = 0;
		
		if( bullets[i][2] > 2 )
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	} 
}
void game::drawBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 1){
			game::gotoxy(bullets[i][1],bullets[i][0]); cout<<"."; 
			game::gotoxy(bullets[i][3],bullets[i][2]); cout<<".";
		}
	}
}
void game::eraseBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] >= 1 ){
			game::gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
			game::gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
		}
	}
}
void game::eraseBullet(int i){ 
	game::gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
	game::gotoxy(bullets[i][3],bullets[i][2]); cout<<" "; 
}
void game::drawship(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			game::gotoxy(j+shipPos, i+22); cout<<ship[i][j];
		}
	}
}
void game::eraseship(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			game::gotoxy(j+shipPos, i+22); cout<<" ";
		}
	}
}
 
int game::collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - shipPos >= 0 && enemyX[0] + 4 - shipPos < 8  ){
			return 1;
		}
	}
	return 0;
}
int game::bulletHit(){
	for(int i=0; i<20; i++){
		for(int j=0; j<4; j+=2){
			if( bullets[i][j] != 0 ){
				if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
					if( bullets[i][j+1] >= enemyX[0] && bullets[i][j+1] <= enemyX[0]+4 ){
						game::eraseBullet(i);
						bullets[i][j] = 0;
						game::resetEnemy(0);
						return 1;
					}
				}
				if( bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1]+4 ){
					if( bullets[i][j+1] >= enemyX[1] && bullets[i][j+1] <= enemyX[1]+4 ){
						game::eraseBullet(i);
						game::resetEnemy(1); 
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
void game::gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t--------------------------"<<endl;
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\t--------------------------"<<endl<<endl;
    cout<<"\t\tYour Final Score: "<<score<<endl<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	getch();
}
void game::updateScore(){
	game::gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;
}

void game::instructions(){
	
	system("cls");
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Press spacebar to make ship shoot";
    cout<<"\n Press A to move left";
    cout<<"\n Press D to move right";
	cout<<"\n\nPress any key to go back to menu";
	getch();
}

void game::play(){
	
	shipPos = -1 + WIN_WIDTH/2;
	score = 0;
	enemyFlag[0] = 1;  
	enemyFlag[1] = 1;
	enemyY[0] = enemyY[1] = 4;
	
	for(int i=0; i<20; i++){
		bullets[i][0] = bullets[i][1] = 0; 
	}
	
	system("cls"); 
	game::drawBorder();
	game::genEnemy(0);
	game::genEnemy(1);
	game::updateScore();
	
	game::gotoxy(WIN_WIDTH + 2, 2);cout<<"Space Invaders game";
	game::gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	game::gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	game::gotoxy(WIN_WIDTH + 7, 12);cout<<"Control ";
	game::gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	game::gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Left";
	game::gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Right";
	game::gotoxy(WIN_WIDTH + 2, 16);cout<<" Spacebar = Shoot";
	
	game::gotoxy(10, 5);cout<<"Press any key to start";
	getch();
	game::gotoxy(10, 5);cout<<"                      ";
	
	while(1){
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( shipPos > 2 )
					shipPos -= 2;
			}
			if( ch=='d' || ch=='D' ){
				if( shipPos < WIN_WIDTH-7 )
					shipPos += 2;
			}
			if(ch==32){
				game::genBullet();
			} 
			if(ch==27){
				break;
			}
		}
		
		game::drawship();
		game::drawEnemy(0);
		game::drawEnemy(1);
		game::drawBullets();
		if( game::collision() == 1  ){
			game::gameover();
			return;
		}
		if(  game::bulletHit() == 1 ){
			score++;
			game::updateScore(); 
		} 
		Sleep(200);
		game::eraseship();
		game::eraseEnemy(0);
		game::eraseEnemy(1);
		game::eraseBullets();
		game::moveBullet();   
		
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;
		 
		if( enemyY[0] > SCREEN_HEIGHT-5 ){ 
			game::resetEnemy(0); 
		}
		if( enemyY[1] > SCREEN_HEIGHT-5 ){ 
			game::resetEnemy(1); 
		}
	}
}

int main()
{
	game::setcursor(0,0); 
	srand( (unsigned)time(NULL)); 
	 
	do{
		system("cls");
		game::gotoxy(10,5); cout<<" -------------------------- "; 
		game::gotoxy(10,6); cout<<" |   Space Invaders Game  | "; 
		game::gotoxy(10,7); cout<<" --------------------------";
		game::gotoxy(10,9); cout<<"1. Start Game";
		game::gotoxy(10,10); cout<<"2. Instructions";	 
		game::gotoxy(10,11); cout<<"3. Quit";
		game::gotoxy(10,13); cout<<"Select option: ";
		char op = getche();
		
		if( op=='1') game::play();
		else if( op=='2') game::instructions();
		else if( op=='3') exit(0);
		
	}while(1);
	
	return 0;
}
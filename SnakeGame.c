#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>

#define SNAKE_ARRAY_SIZE 300
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define PAUSE 112
#define ESC 27
#define ENTER_KEY 13

const char BORDER='#';
const char SNAKE_HEAD='O';
const char SNAKE_BODY='+';
const char SNAKE_FOOD='F';

//Border Co-ordinate
int x=8,y=2,maxX=60,maxY=23;//Dont set x=1 and y=1

int SnakeLength=4;
long int Score=0;
int Life=1;

void gotoxy(int x,int y);
void ClearConsoleToColors(int TextColor,int ConsoleColor);
void TextColor(int TextColor);
void Border();
void Main();
void moveSnake(int snakeXY[2][SNAKE_ARRAY_SIZE],int direction);
int checkForKey(int direction);
void pause();
void generateSnakeFood(int snakeXY[2][SNAKE_ARRAY_SIZE],int foodXY[2]);
int GameOver(int snakeXY[2][SNAKE_ARRAY_SIZE]);
int collideWithItself(int snakeXY[2][SNAKE_ARRAY_SIZE]);
int checkFoodOnSnakeLength(int snakeXY[2][SNAKE_ARRAY_SIZE],int foodXY[2]);
void Info();
int Menu();
int selectMenu(int,int);
void Exit();
void CreateHighScore();
void displayHighScore();

int main()
{
	SetConsoleTitle("SNAKE GAME");
	ClearConsoleToColors(15,15);//white,white
	TextColor(12);
	
	do
	{
		system("cls");
	    switch(Menu())
	    {
		    case 5:
			    Main();
			    break;
			case 6:
				displayHighScore();
				break;
		    case 7:
			    Exit();

	    }
	}while(1);
	return 0;
}

void Main()
{
	int snakeXY[2][SNAKE_ARRAY_SIZE];
	int foodXY[2];
	int direction;
	register int i;
	int xy[2];
	int wait;
	
	system("cls");
	gotoxy(x+6,y-2);printf("SNAKE GAME In C Without Using Graphics");
	
	snakeXY[0][0]=maxX-18;
	snakeXY[1][0]=maxY-10;
	
	direction=LEFT_ARROW;
	
	xy[0]=snakeXY[0][0];
	xy[1]=snakeXY[1][0];
	for(i=1;i<=SnakeLength-1;i++)
	{
		snakeXY[0][i]=xy[0] + i;
		snakeXY[1][i]=xy[1];
	}
	
	Border();
	generateSnakeFood(snakeXY,foodXY);
	
	clock_t value;
	wait=CLK_TCK-(9)*(CLK_TCK/10);
	value=clock() + wait;
	Info();
	
	do
	{
		direction=checkForKey(direction);
		if(clock() >=value)
		{
		    moveSnake(snakeXY,direction);
		    if(snakeXY[0][0]==foodXY[0] && snakeXY[1][0]==foodXY[1])
		    {
		    	SnakeLength++;
		    	Score=Score+10;
		    	if(SnakeLength==SNAKE_ARRAY_SIZE-1)
		    	{
		    		gotoxy(maxX-32,maxY+2);
		    		printf("=== YOU WON ===");getch();
		    		exit(0);
		    	}
			    generateSnakeFood(snakeXY,foodXY);
			    Info();
		    }
		    value=clock() + wait;
	    }
	}while(!GameOver(snakeXY));
}


void moveSnake(int snakeXY[2][SNAKE_ARRAY_SIZE],int direction)
{
	int xy[2];
	register int i;
	xy[0]=snakeXY[0][SnakeLength-1];
	xy[1]=snakeXY[1][SnakeLength-1];
	gotoxy(xy[0],xy[1]);printf(" ");
	
	gotoxy(snakeXY[0][0],snakeXY[1][0]);printf("%c",SNAKE_BODY);
	
	for(i=SnakeLength-1;i>=1;i--)
	{
		snakeXY[0][i]=snakeXY[0][i-1];
		snakeXY[1][i]=snakeXY[1][i-1];
	}
	switch(direction)
	{
		case LEFT_ARROW:
			snakeXY[0][0]--;
			break;
		case RIGHT_ARROW:
			snakeXY[0][0]++;
			break;
		case UP_ARROW:
			snakeXY[1][0]--;
			break;
		case DOWN_ARROW:
			snakeXY[1][0]++;
			break;
	}
	gotoxy(snakeXY[0][0],snakeXY[1][0]);printf("%c",SNAKE_HEAD);
	gotoxy(0,0);
}

int checkForKey(int direction)
{
	int key;
	if(kbhit())
	{
		key=getch();
		if(key!=direction)
		{
			if(key==LEFT_ARROW && direction!=RIGHT_ARROW)
			    direction=key;
			else if(key==RIGHT_ARROW && direction!=LEFT_ARROW)
			    direction=key;
			else if(key==UP_ARROW && direction!=DOWN_ARROW)
			    direction=key;
			else if(key==DOWN_ARROW && direction!=UP_ARROW)
			    direction=key;
			else if(key==ESC)
			{
				SnakeLength=4;
                Score=0;
                Life=1;
                main();
			}
			else if(key==PAUSE)
			    pause();
		}
	}
	return(direction);
}

void generateSnakeFood(int snakeXY[2][SNAKE_ARRAY_SIZE],int foodXY[2])
{
	do
	{
		srand(time(NULL));
	    foodXY[0]=rand() % (maxX-x-1)+(x+1);
	    srand(time(NULL));
	    foodXY[1]=rand() % (maxY-y-1)+(y+1);
	  //gotoxy(foodXY[0],foodXY[1]);
	  //  printf("%c",SNAKE_FOOD);
	}while(checkFoodOnSnakeLength(snakeXY,foodXY));
	
	gotoxy(foodXY[0],foodXY[1]);
	printf("%c",SNAKE_FOOD);
}

int GameOver(int snakeXY[2][SNAKE_ARRAY_SIZE])
{
	int key;
	int a,b;
	if(snakeXY[0][0]==x || snakeXY[1][0]==y || snakeXY[0][0]==maxX || snakeXY[1][0]==maxY)
	{
		Life--;
		if(Life<0)
		{
			gotoxy(x+18,maxY+3);
		//	TextColor(16);
			printf("== Game Over ==");
			a:
			key=getch();
			if(key==13)
			{
				InputScore();
			    main();
			}
			else goto a ;
		}
		else Main();
	}
	else if(collideWithItself(snakeXY))
	{
		Life--;sleep(2);
		if(Life<0)
		{
			gotoxy(x+18,maxY+3);
			//TextColor(16);
			printf("== Game Over ==");
			b:
			key=getch();
			if(key==13)
			{
			    main();
			}
			else goto b ;
		}
		else Main();
	}
	return 0;
}

int collideWithItself(int snakeXY[2][SNAKE_ARRAY_SIZE])
{
	int i;
	for(i=1;i<=SnakeLength-1;i++)
	{
		if(snakeXY[0][0]==snakeXY[0][i] && snakeXY[1][0]==snakeXY[1][i])
		return 1;
	}
	return 0;
}

int checkFoodOnSnakeLength(int snakeXY[2][SNAKE_ARRAY_SIZE],int foodXY[2])
{
	int i;
	for(i=1;i<=SnakeLength-1;i++)
	{
		if(foodXY[0]==snakeXY[0][i] && foodXY[1]==snakeXY[1][i])
		return 1;
	}
	return 0;
}

void Info()
{
	gotoxy(x+1,maxY+2);printf("Score :%d",Score);
	gotoxy(x+1,maxY+3);printf("SnakeLength :%d",SnakeLength);
	gotoxy(x+1,maxY+4);printf("Life :%d",Life);
	//gotoxy(maxX-18,maxY+2);printf("Coder :Girish Rawat");
	gotoxy(maxX-25,maxY+4);printf("Press Esc to exit any time");
}

void pause()
{
	int pause;
	gotoxy(x+18,maxY+2);
	printf("== PAUSED ==");
	while(!kbhit());
	gotoxy(maxX-32,maxY+2);printf("             ");
}

int Menu()
{
	int select;
	int x,y;
	int y1;
	x=10  ,  y=5;
	y1=y;
	
	gotoxy(x,y++);printf("New Game");
	gotoxy(x,y++);printf("High Scores");
	gotoxy(x,y++);printf("Exit");
	
	select=selectMenu(x,y1);
	
	return(select);
}

int selectMenu(int x,int y1)
{
	int key;
	int i=0;
	x=x-2;
	
	gotoxy(x,y1);printf(">");
	gotoxy(1,1);
	
	do
	{
		key=getch();
		if(key==DOWN_ARROW)
		{
			
			if(y1>=7)
			{
				gotoxy(x,y1);printf(" ");
			    y1=4;
			    gotoxy(x,y1++);printf(" ");
			    gotoxy(x,y1);printf(">");
			}
			else 
			{
			    gotoxy(x,y1++);printf(" ");
			    gotoxy(x,y1);printf(">");
		    }
		}
		
		if(key==UP_ARROW)
		{
			if(y1==5)
			{
				gotoxy(x,y1);printf(" ");
				y1=7;
				gotoxy(x,y1);printf(">");
			}
			else
			{
				gotoxy(x,y1--);printf(" ");
				gotoxy(x,y1);printf(">");
			}
		}
	}while(key!=ENTER_KEY);
	
	return(y1);
}

void Exit()
{
	int key;
	printf("Are You Sure You Want To Exit(Y/N)");
	do
	{
	    key=getch();
	    if(key==121 || key==89)
	    	exit(0);
	    else if(key==110 || key==78)
	        return;
	    
	}while(1);
}

void CreateHighScore()
{
	int i;
	FILE *fp;
	fp=fopen("Score.txt","w+");
	if(fp==NULL)
	{
		system("cls");
		printf("Error in Creating File");
		exit(1);
	}
	for(i=1;i<=5;i++)
	{
	    fprintf(fp,"%d",i);
	    fprintf(fp,"%s","\tEMPTY\t\t0\n");
	}
	fclose(fp);
	return;
}

void displayHighScore()
{
	int y=5;
	char str[200];
	FILE *fp;
	system("cls");
	
	fp=fopen("Score.txt","r");
	if(fp==NULL)
	{
		CreateHighScore();
		if((fp=fopen("Score.txt","r"))==NULL)
		{
			system("cls");
			printf("Error in Opening File");
			exit(1);
		}
	}
	gotoxy(10,y++);printf("High Scores");
	gotoxy(10,y++);printf("RANK\tNAME\t\tSCORES");
	while(!feof(fp))
	{
		gotoxy(10,y++);
		if(fgets(str,50,fp))
		printf("%s",str);
	}
	fclose(fp);
	getch();
}

void InputScore()
{
	char name[15];
	FILE *fp;
	system("cls");
	fp=fopen("Score.txt","r");
	if(fp==NULL)
	{
		CreateHighScore();
		if((fp=fopen("Score.txt","a"))==NULL)
		{
			system("cls");
			printf("Error in Creating File");exit(1);
		}
	}
	getch();
}

void Border()
{
	int i;
	for(i=x;i<=maxX;i++)
	{
		gotoxy(i,y);printf("%c",BORDER);
	}
	for(i=y;i<=maxY;i++)
	{
		gotoxy(maxX,i);printf("%c",BORDER);
	}
	for(i=maxX;i>=x;i--)
	{
		gotoxy(i,maxY);printf("%c",BORDER);
	}
	for(i=maxY;i>=y;i--)
	{
		gotoxy(x,i);printf("%c",BORDER);
	}
}

void gotoxy(int x,int y)
{
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

//This Fuction Changes the Console background color
void ClearConsoleToColors(int TextColor,int ConsoleColor)
{
	COORD coord={0,0};
	WORD wColor=((ConsoleColor & 0x0F)<<4) + (TextColor & 0x0F);
	HANDLE hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut,wColor);
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(hStdOut,&csbi))
	{
		FillConsoleOutputCharacter(hStdOut,(TCHAR)32,csbi.dwSize.X * csbi.dwSize.Y,coord,&count);
		FillConsoleOutputAttribute(hStdOut,csbi.wAttributes,csbi.dwSize.X * csbi.dwSize.Y,coord,&count);
		SetConsoleCursorPosition(hStdOut,coord);
	}
}

//This Function Changes The TextColor
void TextColor(int TextColor)
{
	WORD wColor;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi))
	{
		wColor=(csbi.wAttributes & 0xF0) + (TextColor & 0x0F);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),wColor);
	}
}

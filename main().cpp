#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<string>
#include<ctype.h>
#include<windows.h>

#define UP 72
#define DOWN 80
#define enter 13
#define LEFT 75
#define RIGHT 77
#define DEL 83
#define F1 59
#define F2 60
#define PAGEUP 73
#define PAGEDOWN 81
#define ESC 27

char wordType[20][20] = {"NOUN", "VERB","ADJECTIVE", "ADVERB","PRONOUN","PREPOSITION"};
char wordTypeShort[20][20] = {"N", "V","ADJ","ADV","PRE"};
int xcol1left = 1, xcol1right = 46, ycol1begin = 3, ycol1max= 39, ycol1=3, ycol2 = 3, ycol2begin = 3, ycol2max = 39;
int colCur = 1;
int xcol2left = 60, xcol2right = 142;
int maxPage = 34;
int vitriMean[20] = {0},vitriEx[6]= {0}; //phan tu [0] chua so luong cua mean, example, sau do la cac vi tri cua mean, example
int saveFlag = 0;
int vitriAddEx = 0;
int vitriAddMean = 0;
int numPage = 0;
int editFlag = 0;

// khai bao cau truc cua word
struct meanNode {
	char wordMean[200];
	meanNode *next;
};
typedef struct meanNode* ptrmeanNode;

typedef struct {
	char word[20];
	char wordType[20];
	ptrmeanNode meanFirst;
	char *example[5];
	int numEx;
} word;

struct node_word {
	word newWord;
	node_word *left;
	node_word *right;
};
typedef struct node_word *node;

typedef struct {
	node first;
	node last;
} wordList;

//xu li do hoa ====================================================================================================
void textColor (int color) {
	//set mau cho chu + nen chu
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}

void resizeConsole(int width, int height) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(
	    GetStdHandle(STD_OUTPUT_HANDLE),coord
	);
}//yes

int wherex() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.X;
}

int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.Y;
}

void SetColor(WORD color) {
	//set mau chu
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}//yes

void SetBGColor(WORD color) {
	//set rieng mau nen cho chu
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	color <<= 4;
	wAttributes &= 0xff0f;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}//yess

void ShowCur(bool CursorVisibility) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

void clearOneLine(int x, int y, int width) {
	gotoxy(x,y);
	for(int i = 1; i <= width; i++) {
		printf(" ");
	}
}

void veKhung(int x, int y, int width, int height) {
	//ve 4 goc
	//ve goc tren ben trai
	gotoxy(x, y);
	printf("%c",218);
	//ve goc tren ben phai
	gotoxy(x + width, y);
	printf("%c",191);
	//ve goc duoi ben trai
	gotoxy(x, y + height);
	printf("%c",192);
	//ve goc duoi ben phai
	gotoxy(x + width, y + height);
	printf("%c",217);

	//ve thanh ngang tren
	for (int i = x + 1; i < (x + width); i++) {
		gotoxy(i, y);
		printf("%c",196);
	}
	//ve thanh ngang ben duoi
	for (int i = x + 1; i < (x + width); i++) {
		gotoxy(i, y+height);
		printf("%c",196);
	}
	//ve thanh doc ben trai
	for (int i = y + 1; i < (y + height); i++) {
		gotoxy(x, i);
		printf("%c",179);
	}
	//ve thanh doc ben phai
	for (int i = y + 1; i < (y + height); i++) {
		gotoxy(x+width, i);
		printf("%c",179);
	}
}//yes

void drawNetDoc(int x, int y, int length,int color) {
	for(int i = 0; i<=length; i++) {
		gotoxy(x,y+i);
		//SetColor(1);
		printf("%c%c%c",219,219,219);
	}
}

void drawNetNgang(int x, int y, int length) {
	gotoxy(x,y);
	for(int i = 0; i<=length; i++) {
		printf("%c",219);
	}
}

void drawIntroduce() {
	system("color 71");
	// thanh ngang tren
	drawNetNgang(0,0,144);
	// thanh ngang duoi
	drawNetNgang(0,20,144);
	// thanh doc trai
	drawNetDoc(0,0,19,11);
	// thanh doc phai
	drawNetDoc(142,0,19,11);
	// in ra chu T
	drawNetNgang(10,5,16);
	drawNetDoc(17,5,8,11);
	// in ra chu U
	drawNetDoc(27,5,8,11);
	drawNetNgang(27,13,12);
	drawNetDoc(40,5,8,11);
	// in ra chu D
	drawNetDoc(60,5,8,11);
	drawNetNgang(60,5,10);
	drawNetNgang(60,13,10);
	drawNetNgang(69,6,3);
	drawNetNgang(71,7,3);
	drawNetNgang(72,8,3);
	drawNetNgang(72,9,3);
	drawNetNgang(72,10,3);
	drawNetNgang(71,11,3);
	drawNetNgang(69,12,3);
	// in ra chu I
	drawNetDoc(79,5,8,11);
	// in ra chu E
	drawNetDoc(86,5,8,11);
	drawNetNgang(86,5,14);
	drawNetNgang(86,9,14);
	drawNetNgang(86,13,14);
	// in ra chu N
	drawNetDoc(105,5,8,11);
	drawNetNgang(105,5,7);
	drawNetNgang(110,6,3);
	drawNetNgang(111,7,3);
	drawNetNgang(112,8,3);
	drawNetNgang(113,9,3);
	drawNetNgang(114,10,3);
	drawNetNgang(115,11,3);
	drawNetNgang(116,12,3);
	drawNetNgang(117,13,3);
	drawNetDoc(121,5,8,11);
	//============TAC GIA============
	gotoxy(60,30);
	printf("MADE BY:");
	gotoxy(60,31);
	printf("LE THANH TUAN _ N17DCAT061");
}

void introduceAnimation() {
	ShowCur(0);
	drawIntroduce();
	int i = 0;
	while(!kbhit()) {
		gotoxy(55,40);
		SetColor(i);
		printf("<<< BAM PHIM BAT KI DE TIEP TUC >>>");
		Sleep(300);
		i++;
	}
	getch();
	SetColor(1);
}

void drawMenu(char chuoi1[], char chuoi2[], char chuoi3[]) {
	veKhung(0,0,144,45);
	veKhung(12,10,120,25);
	veKhung(23,12,100,21);
	gotoxy(56,20);
	printf("%s",chuoi1);
	gotoxy(56,21);
	printf("%s",chuoi2);
	gotoxy(53,22);
	printf("%s",chuoi3);
	gotoxy(50,24);
	SetBGColor(14); //highlight USE
	printf("USE:  KEY UP , KEY DOWN : to move. ENTER to choose");
}

int menuChoosen() {
	ShowCur(0);
	SetColor(1);
	char chuoi1[50] = "----->>>>   TRA TU DIEN   <<<<-----";
	char chuoi2[50] = "----->>>>   THEM TU MOI   <<<<-----";
	char chuoi3[50] = "----->>>>   THOAT CHUONG TRINH   <<<<-----";
	drawMenu(chuoi1,chuoi2, chuoi3);
	int vitriConTro = 20;
	int kb_hit = 0;
	while(1) {
		if(kbhit()) {
			int kb_hit = getch();
			if(kb_hit == 224) {
				kb_hit = getch();
				//reset mau cho menu
				gotoxy(56,vitriConTro);
				SetBGColor(7);
				if(vitriConTro == 20) printf("%s",chuoi1);
				if(vitriConTro == 21) printf("%s",chuoi2);
				if(vitriConTro == 22) {
					gotoxy(53,vitriConTro);
					printf("%s",chuoi3);
				}

				if(kb_hit == UP || kb_hit ==DOWN) { //up
					if(kb_hit == UP) {
						vitriConTro--;
						if(vitriConTro == 19) vitriConTro = 22;
					}
					if(kb_hit == DOWN) {
						vitriConTro++;
						if(vitriConTro == 23) vitriConTro = 20;
					}
					gotoxy(56,vitriConTro);
					SetBGColor(139);
					if(vitriConTro == 20) printf("%s",chuoi1);
					if(vitriConTro == 21) printf("%s",chuoi2);
					if(vitriConTro == 22) {
						gotoxy(53,vitriConTro);
						printf("%s",chuoi3);
					}
				}
			}
			if(kb_hit == enter)  return vitriConTro - 20; // enter
		} //endif
	} //endwhile
	SetBGColor(7);
}

void drawPanelBackground(int x, int y, int color) {
	textColor(color);
	for(int h = 0; h<5; h++) {
		drawNetNgang(x,y+h,50);
	}
}

int AskPanel(int xpanel, int ypanel, char ques[]) { //tham so la toa do in ra
	drawPanelBackground(xpanel,ypanel,4);
	gotoxy(xpanel+10,ypanel+1);
	textColor(4);
	printf("%s",ques);
	char yes[4] = "YES";
	char no[3] = "NO";
	gotoxy(xpanel+15,ypanel+3);
	printf("%s",yes);
	textColor(117); // mau chu tim bG trang
	gotoxy(xpanel+35,ypanel+3);
	printf("%s",no);
	int vitriHighlight = 2; // bat dau o NO
	while(1) {
		int kb_hit = 0;
		if(kbhit()) {
			kb_hit = getch();
			if(kb_hit == 224) kb_hit = getch();
			if(kb_hit == LEFT || kb_hit == RIGHT) {
				if(vitriHighlight == 2) {
					gotoxy(xpanel+35,ypanel+3);
					textColor(4);
					printf("%s",no);
					gotoxy(xpanel+15,ypanel+3);
					textColor(117);
					printf("%s",yes);
					vitriHighlight = 1;
					continue;
				}
				if(vitriHighlight == 1) {
					gotoxy(xpanel+15,ypanel+3);
					textColor(4);
					printf("%s",yes);
					gotoxy(xpanel+35,ypanel+3);
					textColor(117);
					printf("%s",no);
					vitriHighlight = 2;
					continue;
				}
			}
			if(kb_hit == enter) { //enter
				if(vitriHighlight == 1) return 1; //yes
				if(vitriHighlight == 2) return 0; //no
			}
		}
	}
}

int saveConfirmPanel(int xpanel, int ypanel) { //tham so la toa do in ra
	int result;
	drawPanelBackground(xpanel,ypanel,0);
	gotoxy(xpanel+10,ypanel+1);
	textColor(4);
	if(editFlag == 0) {
		printf("BAN CHUA THUC HIEN THAY DOI NAO !");
		gotoxy(xpanel+12,ypanel+2);
		printf(" AN BAT KY PHIM DE TIEP TUC !");
		while(!kbhit()) {
		}
		getch();
		for(int h = ypanel; h<=ypanel+5; h++) {
			clearOneLine(0,h,144);
		}
		return 0;
	} else {
		char question[50] = "BAN CO MUON LUU LAI THAY DOI ?";
		result = AskPanel(xpanel,ypanel,question);
		//editFlag = 0;
		return result;
	}
}

int inputAskPanel(int x, int y) {
	int result;
	char question[50] = "BAN CO MUON NHAP TIEP KHONG ? ";
	result = AskPanel(x,y,question);
	return result;
}

int repairAskPanel(int x, int y) {
	int result;
	char question[50] = "BAN CO MUON CHINH SUA THEM KHONG ? ";
	result = AskPanel(x,y,question);
	return result;
}

int deleteConfirmPanel(int x, int y) {
	int result;
	char question[50] = "BAN CO CHAC CHAN MUON XOA KHONG ? ";
	result = AskPanel(x,y,question);
	return result;
}

void drawHelpPanelBackground() {
	for(int h = 41; h<=46; h++) {
		clearOneLine(0,h,144);
	}
	textColor(116);
	veKhung(0,41,144,6);
}

void drawHelpFind() {
	drawHelpPanelBackground();
	gotoxy(1,42);
	printf("HELP COMMAND");
	gotoxy(10,44);
	printf("ENTER:CHON/TIM TU MOI        F1:CHINH SUA         F2:SAVE        KEY UP/DOWN:MOVE UP/DOWN         ESC:THOAT        DELETE:XOA TU");
	gotoxy(35,46);
	printf("PAGE UP: TRANG SAU                           PAGE DOWN: TRANG TRUOC");
}

void drawHelpChinhSua() {
	drawHelpPanelBackground();
	gotoxy(1,42);
	printf("EDIT HELP COMMAND");
	gotoxy(20,44);
	printf("ENTER:CHON DE SUA        KEY UP/DOWN:MOVE UP/DOWN        KEY LEFT/RIGHT: EXIT EDIT        DELETE:XOA");
}

void drawInputNew() {
	drawHelpPanelBackground();
	gotoxy(1,42);
	printf("HELP");
	gotoxy(20,44);
	printf("1. NHAP '.' DE KET THUC NHAP MEAN/EXAMPLE.");
	gotoxy(20,45);
	printf("2. NHAP TOI DA 5 VI DU CHO MOT TU. ");
}

void drawKhungFind() {
	system("cls");
	system("color 74");
	veKhung(0,0,144,46);
	for(int i = 1; i<41; i++) {
		gotoxy(48,i);
		printf("%c",179);
	}
	// in cot ngang tren
	gotoxy(1,4);
	for(int i = 1; i<144; i++) {
		printf("%c",196);
	}
	// in cot ngang giua col2
	gotoxy(49,20);
	for(int i = 1 ; i<96 ; i++) {
		printf("%c",196);
	}
	gotoxy(1,1);
	printf("Find: ");
	gotoxy(49,1);
	printf("Word: ");
	gotoxy(49,2);
	printf("Word type: ");
	gotoxy(49,5);
	printf("Word Mean: ");
	gotoxy(49,21);
	printf("Example:");
	gotoxy(0,46);
	drawHelpFind();
}//yes

void clearCol1() {
	for(int i = 6; i<=40; i++) {
		clearOneLine(5,i,39);
	}
}

void clearCol2() {
	clearOneLine(63,1,76); //xoa word
	clearOneLine(63,3,76); //xoa wordtype
	//xoa phan con lai
	for(int col = 6; col<=39; col++) {
		if(col==20) col++;
		clearOneLine(63,col,140-63);
	}
}//yes

// xu li word danh sach lien ket kep ===============================================================================================

void initListWord(wordList &list) {
	list.first = list.last = NULL;
}

void initBam(wordList k[]) {
	for(int i = 0; i<26; i++) {
		initListWord(k[i]);
	}
}

node mallocNewNodeWord() {
	node p;
	p = (node)malloc(sizeof(node_word));
	p->left = NULL;
	p->right = NULL;
	return p;
}

int isEmptyWord(node &first) {
	return (first == NULL ? 1 : 0);
}

void clearListMean(ptrmeanNode &first) {
	ptrmeanNode p;
	while(first!= NULL) {
		p = first;
		first = first->next;
		free(p);
	}
}

void clearListExample(node &p) {
	for(int i = 0; i<5 ; i++) {
		free(p->newWord.example[i]);
	}
}

void deleteFirstWord(node &first) {
	node copy;
	if (isEmptyWord(first) == 1) printf("Khong con tu nao de xoa!");
	else {
		copy = first;
		if (first->right == NULL) {
			first = NULL;
		} else {
			first = copy->right;
			first->left = NULL;
		}
		clearListMean(copy->newWord.meanFirst);
		clearListExample(copy);
		free(copy);
	}
}

int deleteLastWord(node &first, node &p) {
	if (isEmptyWord(first) == 1) printf("Khong con tu nao de xoa!");
	else {
		node front, after;
		front = p->left;
		front->right = NULL;
		clearListMean(p->newWord.meanFirst);
		clearListExample(p);
		free(p);
	}
}

int deleteWord(node &first, node p) {
	node front,after;
	if(p == NULL) return 0;
	else {
		if(first ==NULL) return 0;
		else {
			if(p == first) deleteFirstWord(first);
			else if( p->right == NULL) deleteLastWord(first,p);
			else {
				front = p->left;
				after = p->right;
				front->right = after;
				after->left = front;
				clearListMean(p->newWord.meanFirst);
				clearListExample(p);
				free(p);
				return 1;
			}
		}
	}
}

void insertLastWord(wordList &l, node &canthem) {
	if(isEmptyWord(l.first)==1) {
		l.first = canthem;
		l.last = canthem;
	} else {
		l.last->right = canthem;
		canthem->left = l.last;
		l.last = canthem;
		canthem->right = NULL;
	}
}

void insertFirstWord(wordList &l, node &canthem) {
	if(l.first == NULL) {
		l.last  = canthem;
		l.first = canthem;
	} else {
		canthem->right = l.first;
		l.first->left = canthem;
	}
	l.first = canthem;
	canthem->left = NULL;
}//yes

void insertRightWord(node &truoc,node &canthem) {
	node sau;
	if(truoc==NULL) {
		printf("Khong the them !");
	} else {
		sau = truoc->right;
		// lien ket giua truoc va canthem
		truoc->right = canthem;
		canthem->left = truoc;
		// lien ket giua sau va canthem
		sau->left = canthem;
		canthem->right = sau;
	}
}

void insertOrder(wordList &l, node &canthem) {
	node w = l.first;
	node t = w;
	for( ; w != NULL && strcmp(w->newWord.word,canthem->newWord.word)<0 ; t = w , w = w->right) {
	};
	if( t == l.first) {
		insertFirstWord(l,canthem);
	} else if(w == NULL) {
		insertLastWord(l,canthem);
	} else insertRightWord(t,canthem);
}

//xu li meanNode danh sach lk don====================================================================================================

void initmeanNode(ptrmeanNode &first) {
	first = NULL;
}//yes

ptrmeanNode new_node_mean() {
	ptrmeanNode mean = (ptrmeanNode)malloc(sizeof(meanNode));
	return mean;
}//yes

int isEmtyMean(ptrmeanNode first) {
	return (first == NULL ? 1 : 0);
}

void insertFirstMean(ptrmeanNode &first, ptrmeanNode &canthem) {
	canthem->next = first;
	first = canthem;
}//yes

//xoa phan tu sau phan tu p

void deleteFirstMean(ptrmeanNode &first) {
	ptrmeanNode temp = first;
	if(first == NULL) printf("Khong the xoa !");
	else {
		first = first->next;
		free(temp);
	}
}

void deleteAfterMean(ptrmeanNode &truoc) {
	ptrmeanNode canxoa = truoc->next;
	if(truoc->next==NULL) printf("Khong the xoa !");
	else {
		truoc->next = canxoa->next;
		free(canxoa);
	}
}
// Bat dau chuc nang tim kiem ===================================================================================================

void saveListNew(wordList*);
void startProgram(wordList*);

//lay vi tri con tro dung truoc con tro can gan cua dslk don mean
node getNode(wordList l, char word[]) {
	node p = l.first;
	while(p!=NULL) {
		if(strcmp(p->newWord.word,word)==0) {
			return p;
		}
		p = p->right;
	}
	return p = NULL;
}//yes

node getNodeChon(wordList l, int posWord,char word[]) {
	node p = l.first;
	int dem = 0;
	while(p!=NULL) {
		if(strncmp(p->newWord.word,word,strlen(word)) == 0) {
			dem++;
			if(posWord == dem) {
				return p;
			}
		}
		p= p->right;
	}
	return (p = NULL) ;
}
void magicInput(int mode , int kb_hit, int &count, char input[], int &sodong) {
	int posLetter = (wherex() - 64)%70;              // lay vi tri con tro nhap
	if(posLetter == (count%70)) {           // neu con tro o ngoai cung
		if(count >= (sodong*70) && count % 70 == 0) {
			gotoxy(64,wherey()+1);
			sodong++;
		}
		printf("%c",kb_hit);
		if(mode == 1) kb_hit = toupper((char)kb_hit);
		input[count] = (char)kb_hit;
		count++;
	} else {                                // neu con tro nam trong chuoi dang nhap
		int x = wherex(), y = wherey();
		int index = posLetter + (sodong -1)*70 ;
		for(int i = ++count ; i >= index + 1; i--) {
			input[i] = input[i - 1];        // dich phai cac tu trong chuoi lui ve sau 1 ki tu
		}
		printf("%c",kb_hit);                // in ki tu moi
		for(int i = index + 1; i<count; i++) {
			printf("%c",input[i]);          // in ki tu cu
			if(wherex()>(64+70)) {
				gotoxy(64,wherey()+1);
			}
		}
		if(mode == 1) kb_hit = toupper((char)kb_hit);
		input[posLetter] = (char)kb_hit;    // gan ki tu moi vao chuoi
		gotoxy(x+1,y); // dua con tro ve vi tri sau tu moi nhap
	}
}
//test input-- mode =1 : nhap word, wordType khong duoc nhap khoang trang, khong co so, mode = 0: nhap example, mean co khoang trang
// return 0 neu da nhap hoan thanh, return -1 neu nhap chua hoan thanh ma di chuyen len xuong, return 2 neu dang nhap ma bam esc
int checkInput(char p[], int mode, int length) {
	char input[200]="";
	int count = 0;
	int sodong = 1;
	int posLetter;
	ShowCur(1);
	while(1) {
		while (kbhit()) {
			int kb_hit = getch();
			if(kb_hit == 224) {
				kb_hit = getch();                               //di chuyen con tro sang trai , phai
				if(kb_hit == 75) {
					if(wherex()-1 < (64));
					else gotoxy(wherex()-1,wherey());
				}
				if(kb_hit == 77) {
					if(wherex()+1 > (64 + count));
					else gotoxy(wherex()+1,wherey());
				}
				continue;
			}
			if(kb_hit == enter) {								//enter
				strncpy(p,input,count);
				return 0; 										//thoat ra sau khi nhap hoan thanh
			}
			if(kb_hit == ESC) {  								//dang nhap ma muon thoat esc
				return 2;
			}
			if(kb_hit == 8 && count > 0) {
				int posLetter = wherex() - 64;					//backspace
				if(count > 0 && count % 70 == 0) {
					gotoxy(64+70,wherey()-1);
					sodong--;
				}
				char temp_input[200]="";
				if(posLetter == (count % 70) ) {
					printf("\b");
					printf(" ");
					printf("\b");
					strncpy(temp_input,input,count-1);
					strncpy(input,temp_input,length-1);
				} else {
					printf("\b");
					printf(" ");
					printf("\b");
					for(int i = posLetter-1; i<count-1; i++) {
						input[i] = input[i+1];
						printf("%c",input[i]);
					}
					gotoxy(64+count,wherey());
					//count--;
					printf("\b");
					printf(" ");
					printf("\b");
					gotoxy(64+posLetter - 1,wherey());
				}
				count--;
			}
			if(count > length) continue;
			if(mode == 0) {                                     // nhap vi du hoac mean co dau
				switch(kb_hit) {
					case 32: //space
					case 34: //dau "
					case 39: //dau '
					case 40: //dau (
					case 41: //dau )
					case 44: //dau ,
					case 46: //dau .
					case 63: { //dau ?
						if(count > (sodong*70) && count % 70 == 0) {  // tran 1 dong, xuong dong
							gotoxy(64,wherey()+1);
							sodong++;
						}
						magicInput(0, kb_hit,count,input,sodong);
						break;
					}
				}
			}
			if (((kb_hit >= 65 && kb_hit <= 90) || (kb_hit >= 97 && kb_hit <= 122) || (kb_hit >= 48 && kb_hit <=57))) {
				if(mode == 1 && (kb_hit >= 48 && kb_hit <=57 )) continue;
				magicInput(mode,kb_hit,count,input,sodong);
			}
		}
	}
	ShowCur(0);
}//yes

// mORe = mean or example ? mean = 1. ex =2
int checkEdit(char p[], node &editnode, int vitri, int mORe) {
	char input[200]="";
	int count = strlen(p);
	int sodong = 1;
	int posLetter = 0;
	ShowCur(1);
	strcpy(input,p);
	while(1) {
		while (kbhit()) {
			int kb_hit = getch();
			if(kb_hit == 224) {
				kb_hit = getch();
				if(kb_hit == 75) {
					if(wherex()-1 < (64));
					else gotoxy(wherex()-1,wherey());
				}
				if(kb_hit == 77) {
					if(wherex()+1 > (64 + count%70));
					else gotoxy(wherex()+1,wherey());
				}
				continue;
			}
			if(kb_hit == enter) { //enter
				strncpy(p,input,count);
				return 0; //thoat ra sau khi nhap hoan thanh
			}
			// khac nhau voi ham checkinput la xu ly dich chuyen cac dong con lai khi xuong dong
			if(count >= (sodong*70) && count % 70 == 0) {
				if(mORe == 1) {
					for(int h = wherey()+1; h<= vitriAddMean; h++) {
						clearOneLine(63,h,76);
					}
					for(int i = vitri+1; i<= vitriMean[0]; i++) {
						vitriMean[i] ++ ;
					}
					vitriAddMean++;
					int k = 1;
					for (ptrmeanNode p = editnode->newWord.meanFirst; p != NULL; p = p->next) {
						if(k > vitri) {
							gotoxy(63,vitriMean[k]);
							printf("-");
							for(int m = 0; m<= strlen(p->wordMean); m++) {
								if(m>0 && m%70 == 0) gotoxy(64,wherey()+1);
								printf("%c",p->wordMean[m]);
							}
						}
						k++; // tang chi so cua mang chua vi tri tung do mean tiep theo
					}
					gotoxy(64,vitriMean[vitri]);
				}
				if(mORe == 2) {
					for(int h = wherey()+1; h<= vitriAddEx; h++) {
						clearOneLine(63,h,76);
					}
					for(int v = vitri+2; v<= vitriEx[0] ; v++) {
						vitriEx[v]++ ;
					}
					vitriAddEx++;
					for(int i =vitri+1 ; i < editnode->newWord.numEx ; i++) {
						gotoxy(63,vitriEx[i+1]);
						printf("-");
						for(int k = 0; k < strlen(editnode->newWord.example[i]); k++) {
							if( k > 0 && (k % 70 == 0)) gotoxy(64,wherey()+1);
							printf("%c",editnode->newWord.example[i][k]);
						}
					}
					gotoxy(64,vitriEx[vitri+1]);
				}
			}
			if(kb_hit == 8 && count > 0) { //backspace
				posLetter = wherex() - 64;                      // lay vi tri con tro
				if(count > 0 && count % 70 == 0) {              // dich chuyen con tro len dong tren neu do la tu cuoi cung cua dong duoi
					gotoxy(64+70,wherey()-1);
					sodong--;
				}
				if(posLetter == (count % 70)) {                 // neu con tro o ngoai cung ben phai
					printf("\b");
					printf(" ");
					printf("\b");
					char temp_input[200]="";
					strncpy(temp_input,input,count-1);
					strncpy(input,temp_input,199);
				} else {                                        // neu con tro nam trong
					printf("\b");                               // xoa tu truoc con tro
					printf(" ");
					printf("\b");
					for(int i = posLetter-1; i<count-1; i++) {  // dich chuyen cac tu cu sang trai va in lai ra man hinh
						input[i] = input[i+1];
						printf("%c",input[i]);
					}
					gotoxy(64+count,wherey());                  // den vi tri ngoai cung xoa tu ngoai cung
					printf("\b");
					printf(" ");
					printf("\b");
					gotoxy(64+posLetter - 1,wherey());              // dua con tro ve vi tri nhap ban dau
				}
				count--;                                            // giam so luong tu nhap vao
			}
			if(count > 199) continue;
			switch(kb_hit) {
				case 32: //space
				case 34: //dau "
				case 39: //dau '
				case 40: //dau (
				case 41: //dau )
				case 44: //dau ,
				case 46: //dau .
				case 63: { //dau ?
					if(count >= (sodong*70) && count % 70 == 0) {
						gotoxy(64,wherey()+1);
						sodong++;
					}
					magicInput(0,kb_hit,count,input,sodong);
					break;
				}
			}
			if (((kb_hit >= 65 && kb_hit <= 90) || (kb_hit >= 97 && kb_hit <= 122)) || (kb_hit >=49 &&kb_hit <=57)) {
				if(count >= (sodong*70) && count % 70 == 0) {
					gotoxy(64,wherey()+1);
					sodong++;
				}
				magicInput(0,kb_hit,count,input,sodong);
			}
		}
	}
	ShowCur(0);
}//yes

int checkType(char p[]) {
	for(int i = 0; i<6; i++) {
		if(strcmp(p,wordType[i])==0) return 1;
		if(strcmp(p,wordTypeShort[i])==0) return 1;
	}
	return 0;
}

void outputMean(ptrmeanNode &first,int &x, int &y) {
	vitriMean[0] = 0;
	int i = 1;
	ptrmeanNode p;
	for (p = first; p != NULL; p = p->next) {
		gotoxy(x-1,y);
		vitriMean[0]++;
		vitriMean[i] = y;
		printf("-");
		for(int m = 0; m<= strlen(p->wordMean); m++) {
			if(m>0 && m%70 == 0) gotoxy(x,wherey()+1);
			printf("%c",p->wordMean[m]);
		}
		y = wherey()+1;  // tang tung do cua vi tri in ra mean
		i++; // tang chi so cua mang chua vi tri tung do mean tiep theo
	}
	vitriAddMean = wherey()+1;
}//yes

void outputExample(node p, int &x, int &y) {
	vitriEx[0] = 0;
	for(int i =0; i<p->newWord.numEx; i++) {
		gotoxy(x-1,y);
		vitriEx[0]++;
		vitriEx[i+1] = y;
		printf("-");
		for(int k = 0; k<strlen(p->newWord.example[i]); k++) {
			if( k > 0 && k%70 == 0) gotoxy(x,wherey()+1);
			printf("%c",p->newWord.example[i][k]);
		}
		y = wherey()+1; // tang tung do cua vi tri in ra example
	}
	vitriAddEx= wherey()+1;
}//yes

//in ra danh sach col2
void printCol2(node p) { //in ra node du lieu cua word o cot 2
	ShowCur(0);
	int xfirstWord = 64;
	int yfirstWord = 1;
	int xfirstWordType = 64;
	int yfirstWordType = 3;
	int xfirstMean = 64;
	int yfirstMean = 6;
	int xfirstEx = 64;
	int yfirstEx = 22;
	SetColor(0); //set mau chu mau den
	gotoxy(xfirstWord,yfirstWord);
	printf("%s",p->newWord.word);
	gotoxy(xfirstWordType,yfirstWordType);
	printf("%s",p->newWord.wordType);
	outputMean(p->newWord.meanFirst,xfirstMean,yfirstMean);
	SetColor(0);
	outputExample(p,xfirstEx,yfirstEx);
	SetColor(0);
}//yes

void moveUp() {
	if(colCur == 1) {
		gotoxy(xcol1left,ycol1);
		printf("   ");
		gotoxy(xcol1right,ycol1);
		printf("  ");
		ycol1--;
		if(ycol1 == 5 ) ycol1-=2;
		if(ycol1<ycol1begin) ycol1 = ycol1max;
		gotoxy(xcol1left,ycol1);
		SetColor(1);
		printf("<<");
		gotoxy(xcol1right,ycol1);
		printf(">>");
		SetColor(0);
	}
	if(colCur == 2) {
		gotoxy(xcol2left,ycol2);
		printf("   ");
		gotoxy(xcol2right,ycol2);
		printf("  ");
		ycol2--;
		if( ycol2 == 5 || ycol2 ==21) ycol2-=2;
		if(ycol2<ycol2begin) ycol2 = ycol2max;
		gotoxy(xcol2left,ycol2);
		SetColor(1);
		printf("<<");
		gotoxy(xcol2right,ycol2);
		printf(">>");
		SetColor(0);
	}
}

void moveDown() {
	if(colCur == 1) {
		gotoxy(xcol1left,ycol1);
		printf("   ");
		gotoxy(xcol1right,ycol1);
		printf("  ");
		ycol1++;
		if(ycol1 == 4) ycol1+=2;
		if(ycol1 >ycol1max) ycol1 = ycol1begin;
		gotoxy(xcol1left,ycol1);
		SetColor(1);
		printf("<<");
		gotoxy(xcol1right,ycol1);
		printf(">>");
		SetColor(0);
	}
	if(colCur == 2) {
		gotoxy(xcol2left,ycol2);
		printf("   ");
		gotoxy(xcol2right,ycol2);
		printf("  ");
		ycol2++;
		if(ycol2 == 4 || ycol2 ==20) ycol2+=2;
		if(ycol2 >ycol2max) ycol2 = ycol2begin;
		gotoxy(xcol2left,ycol2);
		SetColor(1);
		printf("<<");
		gotoxy(xcol2right,ycol2);
		printf(">>");
		SetColor(0);
	}
}

void moveLeftRight() {
	if(colCur == 1) {
		gotoxy(xcol1left,ycol1);
		printf("   ");
		gotoxy(xcol1right,ycol1);
		printf("  ");
		ycol2 = 3;
		SetColor(1);
		gotoxy(xcol2left,ycol2);
		printf("<<");
		gotoxy(xcol2right,ycol2);
		printf(">>");
		SetColor(0);
		colCur++;
	} else if(colCur==2) {
		gotoxy(xcol2left,ycol2);
		printf("  ");
		gotoxy(xcol2right,ycol2);
		printf("  ");
		SetColor(1);
		gotoxy(xcol1left,ycol1);
		printf("<<");
		gotoxy(xcol1right,ycol1);
		printf(">>");
		SetColor(0);
		colCur--;
	}
}

// in ra danh sach col1
void printDsCol1(wordList l[],char wordFind[],int soLuongDaNhap, int page) {
	int xleft = 5, y = 6,dem=0;
	if(strcmp(wordFind,"")==0) {
		clearCol1();
		return;
	}
	node p = l[wordFind[0]-65].first;
	clearCol1();
	while(p!=NULL) {
		gotoxy(xleft,y);
		if(strncmp(wordFind,p->newWord.word,soLuongDaNhap)==0) {
			if(dem >= (page-1)*34 && dem < (page)*34) {
				ShowCur(0);
				printf("%s",p->newWord.word);
			}
			dem++;
			y++;
		}
		if(y>39) y = 6;
		p = p->right;
	}
	numPage = dem/34+1;
	gotoxy(5,40);
	printf("PAGE: %d OF %d",page,numPage);
}

int find(wordList l[],char word[]) {
	int count = 0; //dem so ki tu
	int page = 1;
	char input[200]= "";
	int x = 7;
	int posLetter = 0;
	ShowCur(1);
	while(1) {
		while (kbhit()) {
			posLetter = x- 7;
			ShowCur(1);
			gotoxy(x,3);
			int kb_hit = getch();
			if(kb_hit == 224) {
				kb_hit = getch();
				//thoat ra khi chon cac goi y khi dang nhap
				if(kb_hit == DOWN || kb_hit == UP) { //up down
					if(strlen(input)!=0) {
						strncpy(word,input,199);
						return -1;
					} else continue; //neu chua nhap gi thi khong up/down duoc ^^
				} //thoat ra trong khi nhap chua hoan thanh
				if(kb_hit == 75) {
					if(wherex()-1 < (7));
					else gotoxy(--x,wherey());
				}
				if(kb_hit == 77) {
					if(wherex()+1 > (7 + count));
					else gotoxy(++x,wherey());
				}
				continue;
			};
			if(kb_hit == 27) return 2;
			if(kb_hit == enter) { //enter
				if(strlen(input) == 0) break;
				strncpy(word,input,199);
				return 0; //thoat ra sau khi nhap hoan thanh
			}
			// chi nhan chu thuong hoac  chu in hoa.
			if (((kb_hit >= 65 && kb_hit <= 90) || (kb_hit >= 97 && kb_hit <= 122)) && count < 200) {
				if(posLetter == count) {
					printf("%c",kb_hit);
					ShowCur(1);
					kb_hit = toupper((char)kb_hit);
					input[count] = (char)kb_hit;
					count++;
				} else if( posLetter <count) {
					for(int i = ++count ; i >= posLetter + 1; i--) {
						input[i] = input[i - 1];
					}
					printf("%c",kb_hit);
					for(int i = posLetter + 1; i<count; i++) {
						printf("%c",tolower(input[i]));
					}
					kb_hit = toupper((char)kb_hit);
					input[posLetter] = (char)kb_hit;
					gotoxy(7+ posLetter + 1,wherey());
				}
				x++;
			}
			if(kb_hit == 8 && count > 0) { //backspace
				if(posLetter == count) {
					gotoxy(x,3);
					printf("\b");
					printf(" ");
					printf("\b");
					char temp_input[200]="";
					strncpy(temp_input,input,count-1);
					strncpy(input,temp_input,199);
				} else if( posLetter < count) {
					printf("\b");
					printf(" ");
					printf("\b");
					for(int i = posLetter-1; i<count-1; i++) {
						input[i] = input[i+1];
						printf("%c",input[i]);
					}
					gotoxy(7+count,wherey());
					printf("\b");
					printf(" ");
					printf("\b");
					gotoxy(7+posLetter - 1,wherey());
				}
				count--;
				x--;
			}
			printDsCol1(l,input, count,page);
		}
	}
	ShowCur(0);
}

// chuc nang xoa mean, example===================================================================================================
void addMean(node repairword) {
	clearOneLine(64,vitriAddMean,76);
	gotoxy(64,vitriAddMean);
	char nextMean[200]="";
	SetColor(0);
	checkInput(nextMean,0,200);
	if(strlen(nextMean)==0) {
		SetColor(8);
		gotoxy(64,vitriAddMean);
		printf("+ Add mean");
		SetColor(4);
	} else {
		editFlag = 1;
		if(saveConfirmPanel(48,41)==1) {
			ptrmeanNode newMean;
			newMean = new_node_mean();
			strncpy(newMean->wordMean,nextMean,199);
			insertFirstMean(repairword->newWord.meanFirst,newMean);
			vitriMean[0]++;
			vitriMean[vitriMean[0]] = vitriAddMean;
			vitriAddMean++;
			saveFlag = 1;
		}
		editFlag = 0;
	}
}

void deleteMeanOfWord(node repairWord) {
	for(int i = 1; i<=vitriMean[0]; i++) {
		if(ycol2 == vitriMean[i]) { // lay vi tri cua con tro => vi tri cua mean trong list mean
			//gotoxy(64,vitriMean[i]); // da xac dinh vi tri mean o vitri i
			ptrmeanNode p;
			int dem = 1; // dai dien cho vi tri cua mean trong list
			if(i == 1) deleteFirstMean(repairWord->newWord.meanFirst);
			else {
				for(p = repairWord->newWord.meanFirst ; p!=NULL ; p = p->next) {
					if((dem+1) == i) {
						//if(i==vitriMean[0]) clearOneLine(64,vitriMean[vitriMean[0]],76);
						for(int k = vitriMean[i]; k<=(vitriMean[i]+strlen(p->next->wordMean)/70) ; k++) {
							clearOneLine(63,k,76);	// xoa mean cu
						}
						deleteAfterMean(p);
						vitriAddMean--; // giam toan do cua vitri in ra dong addMean
						vitriMean[0]--; // giam so luong mean
						return;
					}
					dem++;
				}
			}
		}
	}
}

void addEx(node repairWord) {
	clearOneLine(64,vitriAddEx,76);
	char save[200]="";
	gotoxy(64,vitriAddEx);
	checkInput(save,0,200);
	if(strlen(save)==0) {
		SetColor(8);
		gotoxy(64,vitriAddEx);
		printf("+ Add example");
		SetColor(4);
	} else {
		editFlag =1 ;
		if(saveConfirmPanel(48,41) == 1) {
			repairWord->newWord.example[repairWord->newWord.numEx] = (char *)malloc(200*sizeof(char));
			strncpy(repairWord->newWord.example[repairWord->newWord.numEx],save,199);
			repairWord->newWord.numEx++;
			if(repairWord->newWord.numEx<5) vitriAddEx++;
		}
		editFlag = 0;
	}
}

void deleteExOfWord(node repairWord) {
	for(int i = 1; i<= vitriEx[0]; i++) {
		if(ycol2 == vitriEx[i]) {	//lay vi tri cua mean tu ycol2 => vi tri cua Ex trong mang ex[]
			for(int k = vitriEx[i]; k <= (vitriEx[i] + strlen(repairWord->newWord.example[i-1])/70); k++) clearOneLine(63,k,76);
			vitriEx[0]--;
			vitriAddEx = vitriEx[vitriEx[0]] + 1;
			repairWord->newWord.numEx--;
			for(int v=i - 1; v < repairWord->newWord.numEx; v++) {
				strcpy(repairWord->newWord.example[v],repairWord->newWord.example[v+1]);
			}
			return;
		}
	}
}

// chuc nang chinh sua===========================================================================================================
void chinhSuaMean(node repairWord) {
	SetColor(0);
	char newMean[200]="";
	for(int i = 1; i<=vitriMean[0]; i++) {
		if(ycol2 == vitriMean[i]) { // lay vi tri cua con tro => vi tri cua mean trong list mean
			ptrmeanNode p;
			int dem = 1;
			for(p = repairWord->newWord.meanFirst ; p!=NULL ; p = p->next) {
				if(dem== i) {
					strncpy(newMean,p->wordMean,199);
					gotoxy(64,vitriMean[i]);
					checkEdit(newMean,repairWord,i,1); // i la thu tu cua mean trong node mean
					if(strcmp(newMean, p->wordMean)!=0) {
						editFlag =1 ;
						if(saveConfirmPanel(48,41) == 1) {
							saveFlag = 1;
							strncpy(p->wordMean,newMean,199);
						}
						editFlag = 0;
					}
					return;
				}
				dem++;
			}
		}
	}
}

void chinhSuaWordType(node repairWord) {
	clearOneLine(64,ycol2,70);
	while(1) {
		char temp[20]="";
		gotoxy(64,3);
		checkInput(temp,1,20);
		if(checkType(temp)==1) {
			editFlag = 1;
			if(saveConfirmPanel(48,41) == 1) {
				strncpy(repairWord->newWord.wordType,temp,19);
				saveFlag = 1;
			}
			editFlag = 0;
			return;
		}
		gotoxy(64,2);
		printf("Wrong Word type ! (Ex: noun,adjective,proposition,adverb,...)");
		gotoxy(64,3);
		printf("Press enter to re-input !");
		while(!kbhit()) {
		}
		getch();
		clearOneLine(64,2,76);
		clearOneLine(64,3,76);
	}
}

void chinhSuaEx( node repairWord) {
	for(int i = 1; i<= vitriEx[0]; i++) {
		if(ycol2 == vitriEx[i]) {	//lay vi tri cua mean tu ycol2 => vi tri cua Ex trong mang ex[]
			gotoxy(64,vitriEx[i]);
			char save[200]="";
			strcpy(save,repairWord->newWord.example[i-1]);
			checkEdit(save,repairWord,i-1,2); // i-1 mang vitriEx bat dau tu 1 con mang ex bat dau tu 0
			if(strcmp(save,repairWord->newWord.example[i-1])!=0) {
				editFlag =1 ;
				if(saveConfirmPanel(48,41) == 1) {
					saveFlag = 1;
					strncpy(repairWord->newWord.example[i-1],save,199);
				}
				editFlag = 0;
			}
			return;
		}
	}
}

void chinhSua(wordList l[] , node repairNode) {
	drawHelpChinhSua();
	int kb_hit = 0;
	int thoatFlag = 0;
	while(thoatFlag==0) {
		if(kbhit()) {
			kb_hit = getch();
			if(kb_hit==224) kb_hit = getch();
			switch (kb_hit) {
					// up
				case DOWN:
				case UP: {
					if(kb_hit == UP) moveUp();
					if(kb_hit == DOWN) moveDown();
					if(ycol2>=6 && ycol2<=vitriMean[vitriMean[0]]+1) {
						clearOneLine(64,vitriAddEx,76);
						gotoxy(64,vitriAddMean);
						SetColor(8);
						printf("+ Add mean");
						SetColor(4);
					}
					if(ycol2>=22 && repairNode->newWord.numEx <5) {
						clearOneLine(64,vitriAddMean,76);
						gotoxy(64,vitriAddEx);
						SetColor(8);
						printf("+ Add example");
						SetColor(4);
					}
					break;
				}
				case LEFT:
				case RIGHT: {
					if(repairAskPanel(48,41)==0) thoatFlag = 1;
					drawHelpChinhSua();
					break;
				}
				case enter: {
					// chinh sua, them Mean, Ex
					if(ycol2 >= 6 && ycol2 <= vitriMean[vitriMean[0]]) chinhSuaMean(repairNode);
					if(ycol2 >= 22 && ycol2 <= vitriEx[vitriEx[0]]) chinhSuaEx(repairNode);
					if(ycol2 == vitriAddMean) addMean(repairNode);
					if(ycol2 == vitriAddEx && repairNode->newWord.numEx < 5) addEx(repairNode);
					if(ycol2 == 3) chinhSuaWordType(repairNode);
					drawHelpChinhSua();
					clearCol2();
					printCol2(repairNode);
					break;
				}
				case DEL: {
					// nut delete, xoa mean, xoa Ex
					if(ycol2 >= 6 && ycol2 <= vitriMean[vitriMean[0]]) {
						if(deleteConfirmPanel(48,41)==1) {
							saveFlag = 1;
							deleteMeanOfWord(repairNode);
						}
					}
					if(ycol2 >= 22 && ycol2 <= vitriEx[vitriEx[0]]) {
						if(deleteConfirmPanel(48,41)==1) {
							saveFlag = 1;
							deleteExOfWord(repairNode);
						}
					}
					drawHelpChinhSua();
					clearCol2();
					printCol2(repairNode);
					break;
				}
			}
		}
	}
}

void keyboardInFindFunc(wordList l[]) {
	int kb_hit;
	int curPage = 1;
	int test = 0; //test find tu
	gotoxy(xcol1left,ycol1);
	SetColor(1);
	printf("<<");
	gotoxy(xcol1right,ycol1);
	SetColor(1);
	printf(">>");
	SetColor(0);
	char word[200] = "";
	gotoxy(7,3);
	printf("Enter to find !!!");
	while(true) {
		if(kbhit()) {
			kb_hit = getch();
			if(kb_hit == 224 ) kb_hit = getch();
			switch (kb_hit) {
					// up
				case UP: {
					moveUp();
					break;
				}
				//down
				case DOWN: {
					moveDown();
					break;
				}
				//enter
				case enter: {
					// truong hop nhap full tu
					if(ycol1 == 3 && xcol1right == 46) {
						clearCol2();
						clearOneLine(6,3,39);
						clearOneLine(6,5,39);
						clearCol1();
						gotoxy(7,3);
						test = find(l,word);
						if(test==0) { //test == 0 tuc la da nhap full chu
							node p = getNode(l[int(word[0])-65],word);
							if(p!=NULL) {
								printCol2(p);
							} else {
								gotoxy(7,5);
								printf("Khong co tu nay ! Enter de tim tiep ! ");
							}
						}
						if(test == 2) { //dang nhap ma bam esc
							system("cls");
							startProgram(l);
							return;
						}
					}
					//truong hop xong chon
					if(colCur == 1 && ycol1>=6 && strlen(word)!=0) {
						clearOneLine(64,1,76);
						clearOneLine(64,3,76);
						clearCol2();
						int posWord = (ycol1-6+1)%maxPage+ 34*(curPage-1);
						node p = getNodeChon(l[int(word[0])- 65],posWord,word);
						if(p!=NULL) printCol2(p);
					}
					break;
				}
				case F1: { // f1 chinh sua
					//colCur = 2
					node cansua;
					moveLeftRight();
					if(ycol1==3) cansua = getNode(l[int(word[0])- 65],word);
					else {
						int posWord = (ycol1-6+1)%maxPage+ 34*(curPage-1);
						cansua = getNodeChon(l[int(word[0])- 65],posWord,word);
					}
					if(cansua != NULL) chinhSua(l,cansua);
					moveLeftRight();
					if(saveFlag == 1) saveListNew(l); //save sau khi edit
					drawHelpFind();
					break;
				}
				case DEL: {
					// delete
					node canxoa;
					if(ycol1==3) canxoa = getNodeChon(l[int(word[0])- 65],1,word);
					else {
						int posWord = (ycol1-6+1)%maxPage+ 34*(curPage-1);
						canxoa = getNodeChon(l[int(word[0])- 65],posWord,word);
					}
					if(canxoa != NULL && (deleteConfirmPanel(48,41)==1)) {
						editFlag = deleteWord(l[int(word[0])- 65].first,canxoa);
					}
					printDsCol1(l,word,strlen(word),curPage);
					clearCol2();
					if(editFlag == 1 ) saveListNew(l) ; //save sau khi delete
					drawHelpFind();
					break;
				}
				case PAGEUP: {
					// page Up : di toi
					if(curPage<numPage) curPage++;
					else curPage = 1;
					printDsCol1(l,word,strlen(word),curPage);
					break;
				}
				case PAGEDOWN: {
					//page down : di lui
					if(curPage > 1) curPage--;
					else {
						curPage = numPage;
					}
					printDsCol1(l,word,strlen(word),curPage);
					break;
				}
				case ESC: {
					//esc
					system("cls");
					startProgram(l);
					return;
				}
			}
		}
	}
}//yes

// chuc nang nhap tu moi===========================================================================================================
int inputNewWord(wordList l[],node &wordnew) {
	int test;
	while(1) {
		gotoxy(64,1);
		do{
			test = checkInput(wordnew->newWord.word,1,20);
		}
		while(strlen(wordnew->newWord.word) == 0);
		if(test == 2) return 2;  //2 la esc, 0 la nhap thanh cong
		int k = int(wordnew->newWord.word[0])-65;
		if(strlen(wordnew->newWord.word)==0) {
			gotoxy(64,1);
			printf("Khong nhap khoang trang !");
			while(!kbhit()) {
			}
			getch();
			clearOneLine(64,1,76);
		} else {
			if( getNode(l[k],wordnew->newWord.word) != NULL) {
				gotoxy(64,1);
				printf("Trung voi tu da co ! Vui long nhap tu khac !");
				while(!kbhit()) {
				}
				getch();
				clearOneLine(64,1,76);
			} else break;
		}
	}
	return 0;
}

int inputNewWordType(node &wordnew2) {
	fflush(stdin);
	int test ;
	while(1) {
		char temp[20]="";
		gotoxy(64,3);
		test = checkInput(temp,1,20);
		if(test == 2) return 2; //ktra esc
		if(checkType(temp)==1) {
			strncpy(wordnew2->newWord.wordType,temp,19);
			break;
		}
		gotoxy(64,2);
		printf("Wrong Word type ! (Ex: noun,adjective,proposition,adverb,...)");
		gotoxy(64,3);
		printf("Press enter to re-input !");
		while(!kbhit()) {
		}
		getch();
		clearOneLine(64,2,76);
		clearOneLine(64,3,76);
	}
	return 0;
}

int inputOneMean(ptrmeanNode &p) {
	int test;
	while(1) {
		test = checkInput(p->wordMean,0,200);
		if(test == 2) return 2;
		if(strlen(p->wordMean)==0) {
			printf("Khong duoc de khoang trang !!! Vui long nhap lai !!!");
		} else if(strncmp(p->wordMean," ",1)==0) printf("Khong duoc khoang trang dau tien!!! Vui long nhap lai");
		else break;
		while(!kbhit()) {
		}
		getch();
		clearOneLine(64,wherey(),70);
		gotoxy(64,wherey());
	}
	if(strcmp(p->wordMean,".")==0) return 1;
	return 0;
}

int inputNewMean(wordList l[], node &wordnew) {
	fflush(stdin);
	int test;
	vitriMean[0] = 1;
	initmeanNode(wordnew->newWord.meanFirst);
	gotoxy(64,5);
	while(1) {
		ptrmeanNode p = new_node_mean();
		gotoxy(56,wherey()+1);
		printf("Mean %2d:", vitriMean[0]);
		gotoxy(64,wherey());
		vitriMean[vitriMean[0]] = wherey();
		test = inputOneMean(p);
		if(test == 2) return 2; //kiem tra esc
		if(test == 0) insertFirstMean(wordnew->newWord.meanFirst, p);
		else {
			clearOneLine(56,wherey(),76);
			vitriAddMean = wherey();
			break;
		}
		vitriMean[0]++;
	}
}

int inputOneEx(char ex[],int x, int y) {
	int testesc;
	while(1) {
		gotoxy(x,y);
		testesc = checkInput(ex,0,200);
		if(testesc == 2) return 2; //chan esc
		if(strlen(ex) == 0) {
			gotoxy(x,y);
			printf("Khong duoc de khoang trang !!! Vui long nhap lai !!!");
		} else if(strncmp(ex," ",1)==0) {
			gotoxy(x,y);
			printf("Khong duoc khoang trang dau tien!!! Vui long nhap lai");
		} else break;
		while(!kbhit()) {
		}
		getch();
		clearOneLine(x,y,70);
	}
	return 0; //nhap thanh cong
}

int inputExample(char *p[], int x, int y, int posEx[]) {
	gotoxy(x,y-1);
	int testesc;
	for(int i = 0; i<5; i++) {
		testesc = 0;
		char temp[200];
		gotoxy(x-10,wherey()+1);
		printf("Example %d: ",i+1);
		testesc = inputOneEx(temp,x,wherey());
		if(testesc == 2) return 2; //chan esc
		if(strcmp(temp,".")==0) {
			clearOneLine(64,wherey(),76);
			vitriAddEx = wherey();
			break;
		} else {
			posEx[0]++ ;
			p[i] = (char *)malloc(200*sizeof(char));
			strncpy(p[i],temp,199);
			posEx[posEx[0]] = wherey();
		}
	}
	return 0; //nhap thanh cong
}

int inputNewEx(wordList l[], node &wordnew) {
	fflush(stdin);
	int test;
	vitriEx[0] = 0;
	test = inputExample(wordnew->newWord.example,64,22,vitriEx);
	if(test == 2) return 2; //ktra esc
	wordnew->newWord.numEx = vitriEx[0];
}

int addOneNewWord(wordList l[],node &wordnew) {
	int esc;
	wordnew = mallocNewNodeWord();
	//====================Nhap word cua tu moi========================
	esc = inputNewWord(l,wordnew);
	if(esc == 2) return 2;
	//=====================Nhap wordType cua tu moi=====================
	esc = inputNewWordType(wordnew);
	if(esc == 2) return 2;
	//======================Nhap nghia cua tu moi====================
	esc = inputNewMean(l,wordnew);
	if(esc == 2) return 2;
	//======================Nhap vi du cua tu moi=======================
	esc = inputNewEx(l,wordnew);
	if(esc == 2) return 2;
	//====================Ket thuc nhap -> chinh sua=======================
	int edit = repairAskPanel(50,10);
	if(edit == 1) {
		drawKhungFind();
		drawHelpChinhSua();
		printCol2(wordnew);
		moveLeftRight();
		chinhSua(l,wordnew);
	}
	return 0;
}

void addManyNewWord(wordList l[]) {
	int stop = 1;
	while(stop == 1) {
		drawKhungFind();
		drawInputNew();
		node wordnew;
		int success ,save = 0;
		success = addOneNewWord(l,wordnew);
		if(success == 0) {
			editFlag = 1;
			save = saveConfirmPanel(50,10);
			if(save == 1) {
				insertOrder(l[int(wordnew->newWord.word[0])-65],wordnew);
				saveListNew(l);
			}
			stop = inputAskPanel(50,10);
		} else break;
	}
}

// xu li file: save , load  =======================================================================================================
void saveOneList(wordList p[], int filePosition) {
	char filename[6] = "a.txt";
	filename[0] = filename[0] + filePosition;
	FILE *k = fopen(filename,"w");
	if(k == NULL) {
		printf("Khong mo duoc file Save !");
		exit(1);
	}
	for(node q= p[filePosition].first; q!=NULL; q=q->right) {
		fprintf(k,"%s\n",q->newWord.word);
		fprintf(k,"%s\n",q->newWord.wordType);
		for(ptrmeanNode h = q->newWord.meanFirst; h!=NULL; h= h->next) {
			fprintf(k,"%s\n",h->wordMean);
		}
		fprintf(k,"%s\n",".");
		for(int m= 0; m < q->newWord.numEx ; m++) {
			fprintf(k,"%s\n",q->newWord.example[m]);
		}
		if(q->newWord.numEx < 5) fprintf(k,"%s\n",".");
	}
	fclose(k);
	saveFlag = 0;
	editFlag = 0;
}

void saveListNew(wordList p[]) {
	for(int fileIndex = 0; fileIndex<=25; fileIndex++) {
		saveOneList(p,fileIndex);
	}
	//sau khi save thif trar 2 flag = 0
	saveFlag = 0;
	editFlag = 0;
}

void loadFile(wordList p[]) {
	for(int i = 0; i<=25; i++) {
		char filename[6] = "a.txt";
		filename[0] = filename[0] + i;
		FILE *k = fopen(filename,"r");
		if(k==NULL) {
			printf("\nKhong mo duoc file Load !");
			exit(1);
		}
		initListWord(p[i]);
		char word2[255]="";
		while(fgets(word2,255,k)) {
			node q;
			q = mallocNewNodeWord();
			word2[strlen(word2) -1 ] = '\0';
			strcpy(q->newWord.word,word2);//lay word
			fgets(q->newWord.wordType,255,k);//lay wordType
			q->newWord.wordType[strlen(q->newWord.wordType)-1] = '\0';
			//lay mean
			initmeanNode(q->newWord.meanFirst);
			while(1) {
				ptrmeanNode j ;
				j = new_node_mean();
				fgets(j->wordMean,255,k);
				if(strcmp(j->wordMean,".\n")==0) break;
				j->wordMean[strlen(j->wordMean) -1] = '\0';
				insertFirstMean(q->newWord.meanFirst,j );
			}
			//lay example
			q->newWord.numEx=0;
			for(int h = 0; h<5; h++) {
				char ex_temp[255] = "";
				fgets(ex_temp,255,k);
				if(strcmp(ex_temp,".\n") == 0) break;
				q->newWord.example[h] = (char *)malloc(200*sizeof(char));
				ex_temp[strlen(ex_temp)-1] = '\0';
				strcpy(q->newWord.example[h],ex_temp);
				q->newWord.numEx++;
			}
			insertLastWord(p[i],q);
		}
		fclose(k);
	}
}//yes

void clearList(wordList &l) {
	node f;
	f = l.first;
	while (l.first != NULL) {
		f = l.first;
		l.first = l.first->right;
		free(f);
	}
}

void startProgram(wordList L[]) {
	system("cls");
	int luaChon = menuChoosen();
	system("cls");
	if(luaChon == 0) {
		drawKhungFind();
		keyboardInFindFunc(L);
	}
	if(luaChon == 1) {
		gotoxy(0,0);
		addManyNewWord(L);
		startProgram(L);
	}
	if(luaChon == 2) {
		if(saveFlag ==1 ) saveListNew(L); //truong hop sot save
		SetColor(1);
		gotoxy(50,10);
		printf("XIN CAM ON VI DA SU DUNG CHUONG TRINH !\n");
	}
	for(int i = 0; i<= 25; i++) {
		clearList(L[i]);
	}
}

int main() {
	wordList L[26];
	initBam(L);
	loadFile(L);
	resizeConsole(1200,808);
	introduceAnimation();
	startProgram(L);
	system("pause");
	return 0;
}
//note: add input number :done
//note 19/5: updated introduceAnimation()
//note 21/5: dang optimize drawSave() , done !
//note 22/5: optimzined full drawpanel.
//note 23-24/5: fixed chinhsua function
//note 27/5: dang fixed xong ham deleteExOfWord, deleteMeanOfWord
//note 31/5: update clearList()
//note 2/6: chinh sua nho cac ham, sap xep lai thu tu
//note 3/6: them showcur()
//note 4/6: upgrade chuc nang checkInput, checkEdit va chinhsua
//note 6/6: fixed bug ham checkInput, checkEdit, fixed chinhsuaEx, chinhsuaMean
// last check : 6/6/2018

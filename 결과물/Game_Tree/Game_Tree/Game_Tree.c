#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define boardsize 3
#define true  1
#define false 0
#define me 1
#define brother     2
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

int checkcount_for_draw = 0; //이미 둔 자리의 수 9가 되면 비긴 것 
int winner = 0; //me = 1, brother = 2
int maxDepth =9 ; //몇 수 앞까지 두어볼지를 결정 한수 앞만 고려
int bestPosition[3] = { 2, 2}; // Mimimax 함수에서 사용 

int gui_arg[4] = { 0,2,6,8 };//귀
int byun_arg[4] = { 1,3,5,7 };//변
int center_arg[1] = { 4 };//중앙

int me_win = 0; //나의 승리 횟수
int brother_win = 0; // 동생의 승리 횟수
int draw = 0; // 무승부

int me_win_for_making_gametree = 0; //트리를 구성하기 위한 시뮬레이션에서 나의 승리
int brother_win_for_making_gametree = 0;//트리를 구성하기 위한 시뮬레이션에서 동생의 승리
int draw_for_making_gametree = 0;//트리를 구성하기 위한 시뮬레이션에서 무승부
int nomeaning_for_making_gametree = 0;//트리를 구성하기 위한 시뮬레이션에서 아직 승부가 결정되지 않은 경우

int minimax_tree_node_num = 0;//최적화 된 트리의 노드 개수 파악을 위해

//int random_counter = 0;

//1차원 숫자(1-9)로 입력된 값을 좌표 값으로 변환하기 위한 구조체 
typedef struct tagPoint {
	int row;
	int col;
}Point;

// 1~9까지의 숫자를 3X3 2차원 배열에서 좌표로 변환 
Point get_point_from_arr(int position)
{
	Point p;
	p.row = position / boardsize;
	p.col = position % boardsize;
	return p;
}

// 입력이 1차원이고, 보드는 2차원으로 되어 있어 입력된 위치에 체크되어 있는지 검사 
int is_fill(int(*board)[boardsize], int position)
{
	Point p = get_point_from_arr(position);
	if (board[p.row][p.col])
	{
		return true;
	}
	return false;
}

// 보드를 0으로 초기화 
void initboard(int(*board)[boardsize])
{
	int i, j;
	for (i = 0; i < boardsize; i++)
	{
		for (j = 0; j < boardsize; j++)
		{
			board[i][j] = 0;
		}
	}
}

/*
// 보드의 상태를 화면에 표시 
// 테스트를 위한 함수 
void print_board(int(*board)[boardsize])
{
	
	lodingjung++;
	printf("게임트리를 만드는 중입니다! 에러 아님 진행중임을 확인하기 위하여 로딩중....(%d)\n", lodingjung);
	int i, j;
	char symbol[3] = { ' ', 'X', 'O' };
	for (i = 0; i < boardsize; i++)
	{
		for (j = 0; j < boardsize; j++)
		{
			if (board[i][j])
			{
				printf("%c ", symbol[board[i][j]]);
			}
			else
			{
				printf("%d ", i * boardsize + j + 1);
			}
		}
		printf("\n");
	}
	printf("\n");
	

}
*/

// brother 또는 me로부터 입력된 값을 보드에 저장 
void set_number_about_board(int(*board)[boardsize], int position, int player)
{
	Point p = get_point_from_arr(position);
	board[p.row][p.col] = player;
	checkcount_for_draw++;
}

// 주어진 위치를 0으로 복원  
void set_zero(int(*board)[boardsize], int position)
{
	Point p = get_point_from_arr(position);
	board[p.row][p.col] = 0;
	checkcount_for_draw--;
}

// 승리체크 
int is_win(int(*board)[boardsize], int player)
{
	if ((board[0][0] == player && board[0][0] == board[0][1] && board[0][1] == board[0][2]) || // 가로0 
		(board[1][0] == player && board[1][0] == board[1][1] && board[1][1] == board[1][2]) || // 가로1 
		(board[2][0] == player && board[2][0] == board[2][1] && board[2][1] == board[2][2]) || // 가로2 
		(board[0][0] == player && board[0][0] == board[1][0] && board[1][0] == board[2][0]) || // 세로1 
		(board[0][1] == player && board[0][1] == board[1][1] && board[1][1] == board[2][1]) || // 세로2 
		(board[0][2] == player && board[0][2] == board[1][2] && board[1][2] == board[2][2]) || // 세로3 
		(board[0][0] == player && board[0][0] == board[1][1] && board[1][1] == board[2][2]) || // 대각선1 
		(board[2][0] == player && board[2][0] == board[1][1] && board[1][1] == board[0][2]))  // 대각선2 
	{
		winner = player;
		return true;
	}

	return false;
}


// 게임이 끝이 났을 때의 점수  
int evaluation()
{
	int score = 0;
	
	if (winner == me)
	{
		score = 1;
		me_win_for_making_gametree++;
	}
	else if (winner == brother)
	{
		score = -1;
		brother_win_for_making_gametree++;
	}
	winner = 0;
	return score;
}

// 누군가 승리를 했거나 비겼나 확인하고 결과를 출력 
int is_game_over(int(*board)[boardsize], int player)
{
	char* Player[3] = { " ", "Compuer", "brother" };

	if (is_win(board, player))
	{
		//printf("%s won!\n", Player[player]);
		if (player == me)
		{
			me_win++;
		}
		else if (player == brother)
		{
			brother_win++;
		}

	}
	else if (checkcount_for_draw == boardsize * boardsize)
	{
		draw++;
		//printf("Draw!\n");
	}
	else
	{
		return false;
	}
	winner = 0;
	return true;
}

// 보드의 빈곳을 위치값으로 변환하여 반환 배열의 끝에는 개수를 넣어준다. 
void get_empty_position(int(*board)[boardsize], int* emptyPosition)
{
	int i, j, index = 0;
	for (i = 0; i < boardsize; i++)
	{
		for (j = 0; j < boardsize; j++)
		{
			if (board[i][j] == 0)
			{
				emptyPosition[index++] = i * boardsize + j;
			}
		}
	}
	emptyPosition[9] = index; //빈곳의 개수가 필요하므로 배열의 마지막에 표시해줌 
}

// me가 둘 위치를 찾기위한 함수 
void set_base_position(int pos, int score)
{

	if (bestPosition[1] > score)
	{
		//printf("내가 체택한것은 %d %d\n", pos+1,score);
		bestPosition[0] = pos;
		bestPosition[1] = score;
	}
	
	/*
	else if (bestPosition[1] == score&& score!=2)
	{
		random_counter++;
	}
	*/
}

// 베스트 위치를 찾기위한 서치함수 
int minimax_for_min_value(int depth, int(*board)[boardsize], int player, int alpha, int beta)
{
	int win_check = is_win(board, 3 - player);

	if (depth == 0|| (win_check) || checkcount_for_draw == 9)
	{
		minimax_tree_node_num++;

		if(checkcount_for_draw == 9 && !(win_check))
		{
			draw_for_making_gametree++;

		}
		if (depth == 0 && !(win_check) && checkcount_for_draw != 9)
		{
			nomeaning_for_making_gametree++;
		}
		return evaluation();
	}
	int emptyposition[10];
	get_empty_position(board, emptyposition);
	int k = 0;
	if (player == me)
	{
		int i, score, minscore = 100;
		for (i = 0; i < emptyposition[9]; i++)
		{
			set_number_about_board(board, emptyposition[i], player);
			score = minimax_for_min_value(depth - 1, board, brother, alpha, beta);
			set_zero(board, emptyposition[i]);
			minscore = min(score, minscore);
			beta = min(minscore, beta);

			if (depth == maxDepth)
			{
				set_base_position(emptyposition[i], minscore);
			}
			if (beta <= alpha)
			{
				break;
			}
		}
		return minscore;
	}
	else
	{
		int i, score, maxscore = -100;
		for (i = 0; i < emptyposition[9]; i++)
		{
			set_number_about_board(board, emptyposition[i], player);
			score = minimax_for_min_value(depth - 1, board, me, alpha, beta);
			set_zero(board, emptyposition[i]);
			maxscore = max(score, maxscore);
			alpha = max(maxscore, alpha);
			if (beta <= alpha)
			{
				break;
			}
		}
		return maxscore;
	}
}

// 게임이 진행되는 함수 
int play_game(int(*board)[boardsize], int player)
{
	if (player == me)
	{
		
		minimax_for_min_value(maxDepth, board, player, -100, 100);
		// 한 수를 두고나면 베스트 값을 초기화해줘야 다음에 옳바른 값을 찾을 수 있슴 
		bestPosition[1] =2;
		
		/* //트리를 구성할 때 존재하지 않는 스테이트에 경우에는 모든칸에 동일한 확률로 수를 둔다.
		//미니맥스 탐색 트리에서는 해당하는 수의 모든 모든 경우를 트리화 시킴으로 존재하지 않는 스테이트는 따로 존재하지 않는다.
		if ((emptyposition[9] - 1) == random_counter)
		{
			random_counter = 0;
			return getnumber_in_random(board);
		}
		else
		{
			random_counter = 0;
			return bestPosition[0];

		}
		*/
		return bestPosition[0];
	}
	else
	{
		return get_number(board);
	}
}

// 게임이 진행되는 함수 랜덤하게 시뮬레이션 할 때 사용
int playgame_in_random(int(*board)[boardsize], int player)
{
	if (player == me)
	{
		return getnumber_in_random(board);
	}
	else
	{
		return get_number(board);
	}
}


// 게임이 시작될 때 초기화가 필요한 변수들을 모아서 초기화 해줌 
int initGame(int(*board)[boardsize])
{
	int player;
	initboard(board);
	//print_board(board);
	checkcount_for_draw = 0;
	bestPosition[1] =2;
	player = 1; // 선을 선택하기 위하여 
	return player;
}


// 보드에 체크할 위치를 숫자로 입력받기위한 함수 
int get_number(int(*board)[boardsize])
{
	//char number;
	int number=0;

	int emptyPosition[10];
	get_empty_position(board, emptyPosition);
	int gui = 0;
	int byun = 0;
	int center = 0;
	for (int i = 0; i < emptyPosition[9]; i++)
	{
		//printf("position = %d \n", emptyposition[i]);

		if ((emptyPosition[i] == 0) || (emptyPosition[i] == 2) || (emptyPosition[i] == 6) || (emptyPosition[i] == 8))
		{
			gui++;
		}
		if ((emptyPosition[i] == 1) || (emptyPosition[i] == 3) || (emptyPosition[i] == 5) || (emptyPosition[i] == 7))
		{
			byun++;
		}
		if ((emptyPosition[i] == 4))
		{
			center++;
		}
	}
	//printf("gui = %d \n", gui);//1
	//printf("byun = %d \n", byun);//2
	//printf("center = %d \n", center);//3
	int pro = 0;
	int next_position_type=0;
	if (gui != 0 && byun != 0 && center != 0)
	{
		 pro = rand() % 7;

		if (pro >= 0 && pro <= 3)
		{
			next_position_type = 1;

		}
		else if (pro >= 4 && pro <= 5)
		{
			next_position_type = 2;
		}
		else if (pro == 6)
		{
			next_position_type = 3;
		}

	}
	else if (gui != 0 && byun != 0 && center == 0)
	{
		pro = rand() % 3;
		if (pro >= 0 && pro <= 1)
		{
			next_position_type = 1;

		}
		else if (pro == 2)
		{
			next_position_type = 2;
		}
	}
	else if (gui != 0 && byun == 0 && center != 0)
	{
		pro = rand() % 5;
		if (pro >= 0 && pro <= 3)
		{
			next_position_type = 1;

		}
		else if (pro == 4)
		{
			next_position_type = 3;
		}
	}
	else if (gui == 0 && byun != 0 && center != 0)
	{
		pro = rand() % 3;
		if (pro >= 0 && pro <=1)
		{
			next_position_type = 2;

		}
		else if (pro == 2)
		{
			next_position_type = 3;
		}
	}
	else if (gui != 0 && byun == 0 && center == 0)
	{
		next_position_type = 1;
	}
	else if (gui == 0 && byun != 0 && center == 0)
	{
		next_position_type = 2;
	}
	else if (gui == 0 && byun == 0 && center != 0)
	{
		next_position_type = 3;
	}


	while (true)
	{
		//printf("next_position_type = %d \n", next_position_type);//3
		if (next_position_type == 1)
		{

			number = rand() % 4;
			number = gui_arg[number];
			//printf(" 무한 루프 number = %d \n", number);//3
		}
		else if (next_position_type == 2)
		{
			number = rand() % 4;
			number = byun_arg[number];
		}
		else if (next_position_type == 3)
		{
			number = center_arg[0];
		}


		if (number < 0 || number > 8)
		{
			//printf("\nPlease enter the correct number.\n");
		}
		else if (is_fill(board, number))
		{
			//printf("Fill position, Please enter the other number\n");
		}
		else
		{
			break;
		}
	}
	return number;
}

//랜덤하게 값을 뽑아내는 함수
int getnumber_in_random(int(*board)[boardsize])
{
	int number;
	while (true)
	{
		//printf("Which position do you want? Input number(1 ~ 9) : \n");
		number = rand() % 9;
		if (number < 0 || number > 8)
		{
			//printf("\nPlease enter the correct number.\n");
		}
		else if (is_fill(board, number))
		{
			//printf("Fill position, Please enter the other number\n");
		}
		else
		{
			break;
		}
	}
	return number;
}

//메인 함수 게임 실행 등을 함
int main(void)
{
	int i;
	int player = 0;
	int position;
	int board[boardsize][boardsize];

	srand((unsigned)time(NULL));
	printf("\n");
	printf("----------알파 베타 가지치기와 minimax 알고리즘을 이용하여 최대한 optimal한 게임트리를 구성하는 경우----------\n");
	
	int k = 0;
	while (k < 1)
	{
		player = initGame(board);
		while (true)
		{
			position = play_game(board, player);
			set_number_about_board(board, position, player);
			//print_board(board);
			if (is_game_over(board, player))
			{
				break;
			}
			player = 3 - player;
		}
		k++;
	}
	int total_for_making_gametree = (me_win_for_making_gametree + brother_win_for_making_gametree + draw_for_making_gametree);
	printf("\n");
	printf("게임트리를 구성하기 위해 %d번 시뮬레이션한 경우.\n",total_for_making_gametree);
	printf("최대한 optimal한 트리를 구성하기 위한 시뮬레이션에서 \"나\"의 승리는 %d번이다.\n", me_win_for_making_gametree);
	printf("최대한 optimal한 트리를 구성하기 위한 시뮬레이션에서 \"동생\"의 승리는 %d번이다.\n", brother_win_for_making_gametree);
	printf("최대한 optimal한 트리를 구성하기 위한 시뮬레이션에서 \"나\"와 \"동생\"의 무승부는 %d번이다.\n", draw_for_making_gametree);

	float per = (float)brother_win_for_making_gametree/total_for_making_gametree;
	printf("최대한 optimal한 노드를 구성하기 위한 시뮬레이션에서 \"동생\"의 승률은 %f퍼센트이다.\n", per*100);

	me_win = 0;
	brother_win = 0;
	draw = 0;
	printf("\n");
	printf("랜덤하게 %d번 시뮬레이션 할 경우\n",total_for_making_gametree);

	k = 0;
	while (k < total_for_making_gametree)
	{
		player = initGame(board);
		while (true)
		{
			position = playgame_in_random(board, player);
			set_number_about_board(board, position, player);
			//print_board(board);
			if (is_game_over(board, player))
			{

				break;


			}

			player = 3 - player;
		}

		k++;
	}
	printf("랜덤하게 할 경우에서  \"나\"의 승리는 %d번이다 \n", me_win);
	printf("랜덤하게 할 경우에서  \"동생\"의 승리는 %d번이다 \n", brother_win);
	printf("랜덤하게 할 경우에서 \"나\"와 \"동생\"의 무승부는 %d번이다.\n", draw);
	int total_game = me_win + brother_win + draw;
	per = (float)brother_win / total_game;
	printf("랜덤하게 할 경우에서 \"동생\"의 승률은 %f퍼센트이다.\n", per * 100);
	me_win = 0;
	brother_win = 0;
	draw = 0;


	maxDepth = 1;
	printf("\n");
	printf("게임트리 구성 후 게임트리를 이용한 게임 시뮬레이션\n");
	me_win = 0;
	brother_win = 0;
	draw = 0;

	int num = 0;

	printf("시뮬레이션 횟수를 입력해 주세요(최소 100만번) 횟수 : ");
	scanf("%d", &num);
	printf("\n");
	k = 0;
	while (k < num)
	{
		player = initGame(board);
		while (true)
		{
			position = play_game(board, player);
			set_number_about_board(board, position, player);
			//print_board(board);
			if (is_game_over(board, player))
			{
				break;
			}
			player = 3 - player;
		}
		k++;
	}
	printf("결과 비교를 위해 %d번 시뮬레이션한 경우.\n",num);
	printf("최대한 optimal한 트리를 이용한 시뮬레이션에서 \"나\"의 승리는 %d번 이다.\n", me_win);
	printf("최대한 optimal한 트리를 이용힌 시뮬레이션에서 \"동생\"의 승리는 %d번 이다.\n", brother_win);
	printf("최대한 optimal한 트리를 이용한 시뮬레이션에서 \"나\"와 \"동생\"의 무승부는 %d번 이다.\n", draw);
	per = (float)brother_win / num;
	printf("최대한 optimal한 노드를 구성하기 위한 시뮬레이션에서 \"동생\"의 승률은 %f퍼센트이다.\n", per * 100);
	total_game = brother_win + me_win + draw;
	me_win = 0;
	brother_win = 0;
	draw = 0;

	printf("\n");
	printf("랜덤하게 %d번 시뮬레이션 할 경우\n", num);

	k = 0;
	while (k < num)
	{
		player = initGame(board);
		while (true)
		{
			position = playgame_in_random(board, player);
			set_number_about_board(board, position, player);
			//print_board(board);
			if (is_game_over(board, player))
			{

				break;


			}

			player = 3 - player;
		}

		k++;
	}
	printf("랜덤하게 할 경우에서  \"나\"의 승리는 %d번이다 \n", me_win);
	printf("랜덤하게 할 경우에서  \"동생\"의 승리는 %d번이다 \n", brother_win);
	printf("랜덤하게 할 경우에서 \"나\"와 \"동생\"의 무승부는 %d번이다.\n", draw);
	per = (float)brother_win / num;
	printf("랜덤하게 할 경우에서 \"동생\"의 승률은 %f퍼센트이다.\n", per * 100);
	me_win = 0;
	brother_win = 0;
	draw = 0;
	printf("\n");

	return 0;

}





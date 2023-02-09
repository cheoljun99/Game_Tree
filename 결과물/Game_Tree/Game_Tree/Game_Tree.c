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

int checkcount_for_draw = 0; //�̹� �� �ڸ��� �� 9�� �Ǹ� ��� �� 
int winner = 0; //me = 1, brother = 2
int maxDepth =9 ; //�� �� �ձ��� �ξ���� ���� �Ѽ� �ո� ���
int bestPosition[3] = { 2, 2}; // Mimimax �Լ����� ��� 

int gui_arg[4] = { 0,2,6,8 };//��
int byun_arg[4] = { 1,3,5,7 };//��
int center_arg[1] = { 4 };//�߾�

int me_win = 0; //���� �¸� Ƚ��
int brother_win = 0; // ������ �¸� Ƚ��
int draw = 0; // ���º�

int me_win_for_making_gametree = 0; //Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� ���� �¸�
int brother_win_for_making_gametree = 0;//Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� ������ �¸�
int draw_for_making_gametree = 0;//Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� ���º�
int nomeaning_for_making_gametree = 0;//Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� ���� �ºΰ� �������� ���� ���

int minimax_tree_node_num = 0;//����ȭ �� Ʈ���� ��� ���� �ľ��� ����

//int random_counter = 0;

//1���� ����(1-9)�� �Էµ� ���� ��ǥ ������ ��ȯ�ϱ� ���� ����ü 
typedef struct tagPoint {
	int row;
	int col;
}Point;

// 1~9������ ���ڸ� 3X3 2���� �迭���� ��ǥ�� ��ȯ 
Point get_point_from_arr(int position)
{
	Point p;
	p.row = position / boardsize;
	p.col = position % boardsize;
	return p;
}

// �Է��� 1�����̰�, ����� 2�������� �Ǿ� �־� �Էµ� ��ġ�� üũ�Ǿ� �ִ��� �˻� 
int is_fill(int(*board)[boardsize], int position)
{
	Point p = get_point_from_arr(position);
	if (board[p.row][p.col])
	{
		return true;
	}
	return false;
}

// ���带 0���� �ʱ�ȭ 
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
// ������ ���¸� ȭ�鿡 ǥ�� 
// �׽�Ʈ�� ���� �Լ� 
void print_board(int(*board)[boardsize])
{
	
	lodingjung++;
	printf("����Ʈ���� ����� ���Դϴ�! ���� �ƴ� ���������� Ȯ���ϱ� ���Ͽ� �ε���....(%d)\n", lodingjung);
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

// brother �Ǵ� me�κ��� �Էµ� ���� ���忡 ���� 
void set_number_about_board(int(*board)[boardsize], int position, int player)
{
	Point p = get_point_from_arr(position);
	board[p.row][p.col] = player;
	checkcount_for_draw++;
}

// �־��� ��ġ�� 0���� ����  
void set_zero(int(*board)[boardsize], int position)
{
	Point p = get_point_from_arr(position);
	board[p.row][p.col] = 0;
	checkcount_for_draw--;
}

// �¸�üũ 
int is_win(int(*board)[boardsize], int player)
{
	if ((board[0][0] == player && board[0][0] == board[0][1] && board[0][1] == board[0][2]) || // ����0 
		(board[1][0] == player && board[1][0] == board[1][1] && board[1][1] == board[1][2]) || // ����1 
		(board[2][0] == player && board[2][0] == board[2][1] && board[2][1] == board[2][2]) || // ����2 
		(board[0][0] == player && board[0][0] == board[1][0] && board[1][0] == board[2][0]) || // ����1 
		(board[0][1] == player && board[0][1] == board[1][1] && board[1][1] == board[2][1]) || // ����2 
		(board[0][2] == player && board[0][2] == board[1][2] && board[1][2] == board[2][2]) || // ����3 
		(board[0][0] == player && board[0][0] == board[1][1] && board[1][1] == board[2][2]) || // �밢��1 
		(board[2][0] == player && board[2][0] == board[1][1] && board[1][1] == board[0][2]))  // �밢��2 
	{
		winner = player;
		return true;
	}

	return false;
}


// ������ ���� ���� ���� ����  
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

// ������ �¸��� �߰ų� ��峪 Ȯ���ϰ� ����� ��� 
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

// ������ ����� ��ġ������ ��ȯ�Ͽ� ��ȯ �迭�� ������ ������ �־��ش�. 
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
	emptyPosition[9] = index; //����� ������ �ʿ��ϹǷ� �迭�� �������� ǥ������ 
}

// me�� �� ��ġ�� ã������ �Լ� 
void set_base_position(int pos, int score)
{

	if (bestPosition[1] > score)
	{
		//printf("���� ü���Ѱ��� %d %d\n", pos+1,score);
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

// ����Ʈ ��ġ�� ã������ ��ġ�Լ� 
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

// ������ ����Ǵ� �Լ� 
int play_game(int(*board)[boardsize], int player)
{
	if (player == me)
	{
		
		minimax_for_min_value(maxDepth, board, player, -100, 100);
		// �� ���� �ΰ��� ����Ʈ ���� �ʱ�ȭ����� ������ �ǹٸ� ���� ã�� �� �ֽ� 
		bestPosition[1] =2;
		
		/* //Ʈ���� ������ �� �������� �ʴ� ������Ʈ�� ��쿡�� ���ĭ�� ������ Ȯ���� ���� �д�.
		//�̴ϸƽ� Ž�� Ʈ�������� �ش��ϴ� ���� ��� ��� ��츦 Ʈ��ȭ ��Ŵ���� �������� �ʴ� ������Ʈ�� ���� �������� �ʴ´�.
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

// ������ ����Ǵ� �Լ� �����ϰ� �ùķ��̼� �� �� ���
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


// ������ ���۵� �� �ʱ�ȭ�� �ʿ��� �������� ��Ƽ� �ʱ�ȭ ���� 
int initGame(int(*board)[boardsize])
{
	int player;
	initboard(board);
	//print_board(board);
	checkcount_for_draw = 0;
	bestPosition[1] =2;
	player = 1; // ���� �����ϱ� ���Ͽ� 
	return player;
}


// ���忡 üũ�� ��ġ�� ���ڷ� �Է¹ޱ����� �Լ� 
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
			//printf(" ���� ���� number = %d \n", number);//3
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

//�����ϰ� ���� �̾Ƴ��� �Լ�
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

//���� �Լ� ���� ���� ���� ��
int main(void)
{
	int i;
	int player = 0;
	int position;
	int board[boardsize][boardsize];

	srand((unsigned)time(NULL));
	printf("\n");
	printf("----------���� ��Ÿ ����ġ��� minimax �˰����� �̿��Ͽ� �ִ��� optimal�� ����Ʈ���� �����ϴ� ���----------\n");
	
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
	printf("����Ʈ���� �����ϱ� ���� %d�� �ùķ��̼��� ���.\n",total_for_making_gametree);
	printf("�ִ��� optimal�� Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� \"��\"�� �¸��� %d���̴�.\n", me_win_for_making_gametree);
	printf("�ִ��� optimal�� Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� \"����\"�� �¸��� %d���̴�.\n", brother_win_for_making_gametree);
	printf("�ִ��� optimal�� Ʈ���� �����ϱ� ���� �ùķ��̼ǿ��� \"��\"�� \"����\"�� ���ºδ� %d���̴�.\n", draw_for_making_gametree);

	float per = (float)brother_win_for_making_gametree/total_for_making_gametree;
	printf("�ִ��� optimal�� ��带 �����ϱ� ���� �ùķ��̼ǿ��� \"����\"�� �·��� %f�ۼ�Ʈ�̴�.\n", per*100);

	me_win = 0;
	brother_win = 0;
	draw = 0;
	printf("\n");
	printf("�����ϰ� %d�� �ùķ��̼� �� ���\n",total_for_making_gametree);

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
	printf("�����ϰ� �� ��쿡��  \"��\"�� �¸��� %d���̴� \n", me_win);
	printf("�����ϰ� �� ��쿡��  \"����\"�� �¸��� %d���̴� \n", brother_win);
	printf("�����ϰ� �� ��쿡�� \"��\"�� \"����\"�� ���ºδ� %d���̴�.\n", draw);
	int total_game = me_win + brother_win + draw;
	per = (float)brother_win / total_game;
	printf("�����ϰ� �� ��쿡�� \"����\"�� �·��� %f�ۼ�Ʈ�̴�.\n", per * 100);
	me_win = 0;
	brother_win = 0;
	draw = 0;


	maxDepth = 1;
	printf("\n");
	printf("����Ʈ�� ���� �� ����Ʈ���� �̿��� ���� �ùķ��̼�\n");
	me_win = 0;
	brother_win = 0;
	draw = 0;

	int num = 0;

	printf("�ùķ��̼� Ƚ���� �Է��� �ּ���(�ּ� 100����) Ƚ�� : ");
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
	printf("��� �񱳸� ���� %d�� �ùķ��̼��� ���.\n",num);
	printf("�ִ��� optimal�� Ʈ���� �̿��� �ùķ��̼ǿ��� \"��\"�� �¸��� %d�� �̴�.\n", me_win);
	printf("�ִ��� optimal�� Ʈ���� �̿��� �ùķ��̼ǿ��� \"����\"�� �¸��� %d�� �̴�.\n", brother_win);
	printf("�ִ��� optimal�� Ʈ���� �̿��� �ùķ��̼ǿ��� \"��\"�� \"����\"�� ���ºδ� %d�� �̴�.\n", draw);
	per = (float)brother_win / num;
	printf("�ִ��� optimal�� ��带 �����ϱ� ���� �ùķ��̼ǿ��� \"����\"�� �·��� %f�ۼ�Ʈ�̴�.\n", per * 100);
	total_game = brother_win + me_win + draw;
	me_win = 0;
	brother_win = 0;
	draw = 0;

	printf("\n");
	printf("�����ϰ� %d�� �ùķ��̼� �� ���\n", num);

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
	printf("�����ϰ� �� ��쿡��  \"��\"�� �¸��� %d���̴� \n", me_win);
	printf("�����ϰ� �� ��쿡��  \"����\"�� �¸��� %d���̴� \n", brother_win);
	printf("�����ϰ� �� ��쿡�� \"��\"�� \"����\"�� ���ºδ� %d���̴�.\n", draw);
	per = (float)brother_win / num;
	printf("�����ϰ� �� ��쿡�� \"����\"�� �·��� %f�ۼ�Ʈ�̴�.\n", per * 100);
	me_win = 0;
	brother_win = 0;
	draw = 0;
	printf("\n");

	return 0;

}





#include<bits/stdc++.h>
using namespace std;

#define MISSIONARIES_MAX 3
#define BARBARIAN_MAX 3
#define SAILWAY_MAX 5
#define LEFT_BANK 1
#define RIGHT_BANK 0
#define STEP_MAX 100

extern int step[STEP_MAX];
extern int g_step_number;
extern int g_sail_way_max;
extern int g_crossing_history[3][STEP_MAX];

struct MyLeftBank
{
	int missionaries;	//����ʿ 
	int barbarians;		//Ұ����
	int boat_state;	//1��ʾ�����󰶣�0��ʾ�����Ұ�
};

int step[STEP_MAX];		//��¼ÿ��·�� 
int g_step_number = 0;
int g_sail_way_max = 0;
int g_crossing_history[3][STEP_MAX];
int g_sail_way[2][STEP_MAX];

int InitSailWay(int sailWay[2][100], int& sailWayMax, const int boatNumber)
{
	int miss_number = 0, barb_number = 0;			//��ʼ������ʿ��Ұ���˵�����
	sailWayMax = 0;									//��ʼ��·�߱��
	//����·�� 
	while(miss_number != boatNumber+1 || barb_number != 0)
	{
		barb_number++;
		if(miss_number + barb_number <= boatNumber)
		{
			sailWay[0][sailWayMax] = miss_number;
			sailWay[1][sailWayMax] = barb_number;
			sailWayMax++;
		}
		else
		{
			miss_number++;
			barb_number = 0;
			sailWay[0][sailWayMax] = miss_number;
			sailWay[1][sailWayMax] = barb_number;
			sailWayMax++;
		}
	}
	sailWayMax--;	//��ȥ�����״̬
	sailWay[0][sailWayMax] = 0;
	sailWay[1][sailWayMax] = 0;

	return 1;
}
//start g_step_number = 1;

int BoatGoOppositeBank(MyLeftBank& this_,int& sail_way,const int input_miss,const int input_barb)
{
	//���������
	if(LEFT_BANK == this_.boat_state)
	{
		//���������������·�ߺ���
		if(this_.missionaries >= g_sail_way[0][sail_way] && this_.barbarians >= g_sail_way[1][sail_way])
		{
			//�ı䴫��ʿ��Ұ���˺ʹ���״̬ 
			this_.missionaries -= g_sail_way[0][sail_way];
			this_.barbarians -= g_sail_way[1][sail_way];
			this_.boat_state = (g_step_number + 1) % 2;
			step[g_step_number] = sail_way;
			return 1;
		}
		sail_way++;
		return 0;
	}
	//��������Ұ� 
	else if(RIGHT_BANK == this_.boat_state)
	{
		int left_miss = this_.missionaries + g_sail_way[0][sail_way];
		int left_barb = this_.barbarians + g_sail_way[1][sail_way];
		if(left_miss <= input_miss && left_barb <= input_barb)
		{
			//�ı䴫��ʿ��Ұ���˺ʹ���״̬ 
			this_.missionaries = left_miss;
			this_.barbarians = left_barb;
			this_.boat_state = (g_step_number + 1) % 2;
			return 1;
		}
		sail_way++;
		return 0;
	}
	else  //����״̬���� 
	{
		cout << "\nBoat state goes wrong.\n";
		system("pause");
		return -1;
	}
}

int JudgeBothBankState(const MyLeftBank this_,int& sail_way ,const int input_miss, const int input_barb)
{
	for(int i=0;i<g_step_number;i++)//�������״̬��ǰһ��״̬��ͬ����ʹ����һ�����з�ʽ����
	{
		if(g_crossing_history[0][i] == this_.missionaries && g_crossing_history[1][i] == this_.barbarians && g_crossing_history[2][i] == this_.boat_state)
		{ 
			return 0;
		}
	}
	//�ų��������: Ұ������������0������ʿ����Ϊ0
	//barb_number>miss_number,��״̬�Ϸ�
	if((this_.missionaries == 0 && (this_.barbarians <= input_barb)) || (this_.missionaries == input_miss && (this_.barbarians <= input_barb)) || (this_.missionaries == 0 && this_.barbarians == 0))
	{
		g_crossing_history[0][g_step_number] = this_.missionaries;
		g_crossing_history[1][g_step_number] = this_.barbarians;
		g_crossing_history[2][g_step_number] = this_.boat_state;
		step[g_step_number] = sail_way;

		sail_way = 0;
		g_step_number++;

		return 1;
	}
	//���������״��
	int right_miss = input_miss - this_.missionaries;
	int right_barb = input_barb - this_.barbarians;
	if((this_.missionaries >= this_.barbarians) && (right_miss >= right_barb))
	{
		g_crossing_history[0][g_step_number] = this_.missionaries;
		g_crossing_history[1][g_step_number] = this_.barbarians;
		g_crossing_history[2][g_step_number] = this_.boat_state;
		step[g_step_number] = sail_way;

		sail_way = 0;
		g_step_number++;

		return 1;
	}
	else
	{
		return 0;
	}
}

int ReturnAvailableState(MyLeftBank& this_, int& sail_way,const int sail_way_max)
{
	//������Ŀ�ĵ�ʱ�����ؿ��õ�·�� 
	if((0 == this_.boat_state) && (0 == this_.missionaries) && (0 == this_.barbarians))
	{
		g_step_number -= 2;
		this_.missionaries = g_crossing_history[0][g_step_number - 1];
		this_.barbarians = g_crossing_history[1][g_step_number - 1];
		this_.boat_state = (g_step_number) % 2;

		sail_way = step[g_step_number];
		sail_way++;
		return 1;
	}
	//�ָ�����ʿ��Ұ���˺ʹ���״̬ 
	if(++sail_way<sail_way_max)
	{
		this_.missionaries = g_crossing_history[0][g_step_number-1];
		this_.barbarians = g_crossing_history[1][g_step_number-1];
		this_.boat_state = (g_step_number) % 2;
	}
	//���ص������õ�·��״̬ 
	while(sail_way>=sail_way_max)																										
	{
		if(g_step_number-- > 1)
		{
		this_.missionaries = g_crossing_history[0][g_step_number-1];
		this_.barbarians = g_crossing_history[1][g_step_number - 1];
		this_.boat_state = (g_step_number) % 2;
		sail_way = step[g_step_number];
		sail_way++;
		}
		else return 0;
	}
	return 1;
}

void PrintingSailWay(const int successWay)
{
	cout << "\n��" << successWay << "�ַ�����" << endl;
	for(int i=0;i<g_step_number;i++)
	{
		cout << '(' << g_crossing_history[0][i] << "," << g_crossing_history[1][i] << "," << g_crossing_history[2][i] << ')' << endl;
	}

}

void CrossRiver(MyLeftBank & leftbank, const int inputMissionaries, const int inputBarbarians, const int inputBoatnumber)
{
	int sailWay = 0;
	int success_way = 0;
	InitSailWay(g_sail_way, g_sail_way_max, inputBoatnumber);
	while(!(g_step_number == 0 && sailWay >= g_sail_way_max))
	{
		while(!((0 == leftbank.boat_state) && (0 == leftbank.missionaries) && (0 == leftbank.barbarians)))
		{
			if(1 == BoatGoOppositeBank(leftbank, sailWay, inputMissionaries, inputBarbarians))
			{
				if(0 == JudgeBothBankState(leftbank, sailWay, inputMissionaries, inputBarbarians))
				if(0 == ReturnAvailableState(leftbank, sailWay, g_sail_way_max))
					break;
			}
			if(sailWay >= g_sail_way_max)
			{
				ReturnAvailableState(leftbank, sailWay, g_sail_way_max);
			}
		}
		if((0 == leftbank.boat_state) && (0 == leftbank.missionaries) && (0 == leftbank.barbarians))
		{
			success_way++;
			PrintingSailWay(success_way);
			ReturnAvailableState(leftbank, sailWay, g_sail_way_max);
		}
	}
	if(success_way == 0)
	{
		cout << "\n���ź��������Ҳ�������\n\n";
	}
	else cout << "\n�����ܹ��ҵ���" << success_way << "�ַ���" << endl;
}

int main()
{
	int input_missionaries;  //����ʿ���� 
	int input_barbarians;  //Ұ�������� 
	int intput_boat_number;  //������������� 
	
	//���봫��ʿ������Ұ���������ʹ������������
	cout << "�����봫��ʿ����:";
	cin >> input_missionaries;
	
	cout << "������Ұ��������:";
	cin >> input_barbarians;

	cout << "�����봬�����������:";
	cin >> intput_boat_number;

	MyLeftBank left_bank;
	left_bank.missionaries = input_missionaries;
	left_bank.barbarians = input_barbarians;
	left_bank.boat_state = LEFT_BANK;

	memset(step, -1, STEP_MAX * sizeof(int));	//��ʼ�� step[STEP_MAX] Ϊ -1(��ʼ̬)

	//��¼������״̬
	g_crossing_history[0][g_step_number] = input_missionaries;
	g_crossing_history[1][g_step_number] = input_barbarians;
	g_crossing_history[2][g_step_number] = LEFT_BANK;
	step[g_step_number] = 0;

	g_step_number++; 
	CrossRiver(left_bank, input_missionaries, input_barbarians, intput_boat_number);
}

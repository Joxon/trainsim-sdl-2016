#ifndef VAR_H_
#define VAR_H_

/*===========================

		 �������

===========================*/

//���ڴ�С
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define USERVIEW_WIDTH (WINDOW_WIDTH/6)
#define USERVIEW_HEIGHT WINDOW_HEIGHT
#define TRAINVIEW_WIDTH (WINDOW_WIDTH*5/6)
#define TRAINVIEW_HEIGHT WINDOW_HEIGHT
//֡��
#define FPS 1
//��������
#define BLOCK_ROW 3
#define BLOCK_COLUMN 21
#define BLOCK_SIZE 60
//��ť����
#define BUTTON_ROW 7
#define BUTTON_COLUMN 3
#define BUTTON_WIDTH 50
#define BUTTON_HEIGHT 30

//���ʽ
enum CommandType
{
	FROM_FILE,
	FROM_KEYBOARD,
};
//����
enum Strategy
{
	ALTERNATIVE = 1,
	FAST_FIRST,
	MANUAL,
};

/*===========================

		 ������

===========================*/

//��������
#define MAX_TRAIN 3
#define MAX_RAIL 3
#define MAX_RAIL_LENGTH 99

//��״̬
enum TrainStatus
{
	STOP,
	RUN,
	PAUSE_COMMON,
	PAUSE_ANY,
	PAUSE_STATION,
	WAIT,
	IDLE,
	BUSY,
};

//�𳵷���
enum TrainDirection
{
	NORMAL = 1,
	REVERSE,
};
//������
enum TrainType
{
	FAST = 1,
	SLOW,
};

//�������
enum BlockPosition
{
	SOUTHWEST,
	WEST,
	NORTHWEST,
	NORTH,
	NORTHEAST,
	EAST,
	SOUTHEAST,
	SOUTH,
};

struct train
{
	//�ٶ�
	float speed;

	//����ʱ��
	int   startTime;

	//����λ��
	int   startPoint;

	/*�𳵵���ʼ�˶�����
	* NORMAL = ˳ʱ��
	* REVERSE = ��ʱ��
	*/
	int   direction;

	//�����з���
	char  directionStr[10];

	/*��״̬
	* RUN = ����
	* PAUSE_COMMON = �ȴ��������
	* PAUSE_ANY = �û���ͣ
	* PAUSE_STATION = ͣ��վ��
	*/
	int   status;

	/*�����ͣ�
	* FAST = �쳵
	* SLOW = ����
	*/
	int   type;

	//�𳵵�ǰ������λ��
	int   position;

	//�����й���ĵ�ĸ���
	int   railwayLength;

	//����վ�����ͣʱ��
	int   pausetime;
};

struct block
{
	//��ͼ����
	int x;
	int y;

	/*������
	* 0 = �ǹ���
	* >=0 = ����
	*/
	int common;

	/*������
	* WEST = ��
	* SOUTH = ��
	* EAST = ��
	* NORTH = ��
	*/
	int direction;

	//�ϴ�ͨ���Ļ�
	int last;

	/*վ����
	* 1Ϊվ��
	* 0Ϊ��վ��
	*/
	int station;
};
#endif //TRAIN_SIMULATION_STRUCT_H

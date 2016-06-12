#ifndef VAR_H_
#define VAR_H_

/*===========================

		 �������

===========================*/

//���ʽ
enum CommandType
{
	FROM_FILE,
	FROM_KEYBOARD,
};

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
#define MAX_TRAIN 9
#define MAX_RAIL 9
#define MAX_RAIL_LENGTH 999

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
	FAST,
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
	int pausetime;
};

struct block
{
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

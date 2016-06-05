#ifndef VAR_H_
#define VAR_H_

/*===========================

         �������

===========================*/

//���ʽ
#define FROM_FILE 1
#define FROM_KEYBOARD 2


/*===========================

         ������

===========================*/

//��������
#define MAX_TRAIN 9
#define MAX_RAIL 9
#define MAX_RAIL_LENGTH 999
//��״̬
#define STOP 1
#define RUN 2
#define PAUSE_COMMON 3
#define PAUSE_ANY 4
#define PAUSE 5
#define WAIT 6
//�𳵷���
#define NORMAL 1
#define REVERSE 2
//������
#define FAST 1
#define SLOW 2
//�������
#define IDLE 3
#define BUSY 4
#define EAST 1
#define WEST 2
#define SOUTH 3
#define NORTH 4

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
    char  dir[10];

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

#ifndef VAR_H_
#define VAR_H_

/*===========================

         程序相关

===========================*/

//命令方式
#define FROM_FILE 1
#define FROM_KEYBOARD 2


/*===========================

         火车与轨道

===========================*/

//对象数量
#define MAX_TRAIN 9
#define MAX_RAIL 9
#define MAX_RAIL_LENGTH 999
//火车状态
#define STOP 1
#define RUN 2
#define PAUSE_COMMON 3
#define PAUSE_ANY 4
#define PAUSE 5
#define WAIT 6
//火车方向
#define NORMAL 1
#define REVERSE 2
//火车类型
#define FAST 1
#define SLOW 2
//轨道参数
#define IDLE 3
#define BUSY 4
#define EAST 1
#define WEST 2
#define SOUTH 3
#define NORTH 4

struct train
{
    //速度
    float speed;

    //启动时间
    int   startTime;

    //启动位置
    int   startPoint;

    /*火车的起始运动方向
    * NORMAL = 顺时针
    * REVERSE = 逆时针
    */
    int   direction;

    //火车运行方向
    char  dir[10];

    /*火车状态
    * RUN = 运行
    * PAUSE_COMMON = 等待公共轨道
    * PAUSE_ANY = 用户暂停
    */
    int   status;

    /*火车类型，
    * FAST = 快车
    * SLOW = 慢车
    */
    int   type;

    //火车当前所处的位置
    int   position;

    //火车运行轨道的点的个数
    int   railwayLength;

    //火车在站点的暂停时间
    int pausetime;
};

struct block
{
    /*共轨标记
    * 0 = 非公共
    * >=0 = 公共
    */
    int common;

    /*方向标记
    * WEST = 左
    * SOUTH = 下
    * EAST = 右
    * NORTH = 上
    */
    int direction;

    //上次通过的火车
    int last;

    /*站点标记
    * 1为站点 
    * 0为非站点
    */
    int station;
};
#endif //TRAIN_SIMULATION_STRUCT_H

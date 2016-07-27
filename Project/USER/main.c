#include "main.h"

u16 NixieTubValue[3] = {0};   //��������ܵ�ֵ

u16 SilentTime=0;             //  ��Ĭʱ��
u8 State=0;                   //��ǰ״̬��0:��д��״̬��1������д��       
u8 UsartUsing = 1;            //0�������ڵȴ�  ��1���ڿ���
u8 UsartUsingTime = 0;        //���ڵȴ�ʱ��
u8 UsartAskCnt = 0;           //����ѯ�ʼƴ�
u8 faultCnt[3]={0};           //���ϼƴ�
u8 NetSta[3];
u8 Usart_flag = 1;
u8 SendCnt=0;  //����ע������Ĵ���
u8 nettemp;
OS_Q_DATA q_data; 
NetState netState;
NetParam netparam;


//�����ջ
OS_STK START_TASK_STK[START_TASK_SIZE];
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
OS_STK HANDSHAKE_TASK_STK[HANDSHAKE_STK_SIZE];
OS_STK NixieTube_TASK_STK[NixieTube_STK_SIZE];
OS_STK USARTSEND_TASK_STK[USARTSEND_STK_SIZE];
OS_STK PARAMSET_TASK_STK[PARAMSET_STK_SIZE];

void * MsgGrp[20];			//��Ϣ���д洢��ַ,���֧��256����Ϣ
void * MsgWrite[10];  
OS_EVENT * q_msg;			//��Ϣ����
OS_EVENT * msg_hand;	
OS_EVENT * uart;
OS_EVENT * msg_receive;
OS_EVENT * msg_write;

//�����ʱ��
OS_TMR   * tmr1;			//��Ĭʱ���ж�
OS_TMR   * tmr2;   //��������

int main(void)
{	
	delay_init();
	NVIC_Configuration();
	//����JTAG PA15��Ϊ��ͨIO	
	DisableJTAG();   
	TM1652_Init(19200); //USART2��3����
	Encode_Init();      //���������ܣ���ʱ��2��3,4
	Led_Init();
	USART1_Config(); //��T81ͨ��
	Key_Init();
	ReadFlash_param();
	OSInit();
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_TASK_SIZE-1],START_TASK_PRIO);//��ʼ����
	OSStart();
}

//��ʼ����
void start_task(void *pdata)
{
	u8 err;
	OS_CPU_SR cpu_sr=0;
	
	uart = OSSemCreate(1); //����ʹ���ź���
	msg_hand = OSMboxCreate((void*) 0); //���ִ��ڽ�������
	q_msg=OSQCreate(&MsgGrp[0],20);	//������Ϣ����
	msg_receive = OSMboxCreate((void*) 0); //��һ��ע������
	msg_write=OSQCreate(&MsgWrite[0],10);	//��һ��ע�����
	tmr1=OSTmrCreate(10,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);		//1sִ��һ��
	OSTmrStart(tmr1,&err);//���������ʱ��1	
	tmr2=OSTmrCreate(10,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_callback,0,"tmr2",&err);		//1sִ��һ��
	OSTmrStart(tmr2,&err);//���������ʱ��2
	
	OS_ENTER_CRITICAL();	//�����ٽ���(�޷����жϴ��)  
	OSTaskCreate(paramSet_task,(void *)0,(OS_STK*)&PARAMSET_TASK_STK[PARAMSET_STK_SIZE-1],PARAMSET_TASK_PRIO);//������λ��ע������
	OSTaskCreate(key_task,(void *)0,(OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],KEY_TASK_PRIO);	                       //��������
	OSTaskCreate(usartsend_task,(void *)0,(OS_STK*)&USARTSEND_TASK_STK[USARTSEND_STK_SIZE-1],USARTSEND_TASK_PRIO);//���ڷ�������
	OSTaskCreate(NixieTube_task,(void *)0,(OS_STK*)&NixieTube_TASK_STK[NixieTube_STK_SIZE-1],NixieTube_TASK_PRIO);//�������ʾ����						   					   
 OSTaskCreate(handshake_task,(void *)0,(OS_STK*)&HANDSHAKE_TASK_STK[HANDSHAKE_STK_SIZE-1],HANDSHAKE_TASK_PRIO);//��������	 				   
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}


#include <includes.h>


static  OS_STK        App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK        App_Task1Stk[APP_TASK_START_STK_SIZE];
static  OS_STK        App_Task2Stk[APP_TASK_START_STK_SIZE];
static  OS_STK        App_Task3Stk[APP_TASK_START_STK_SIZE];
unsigned char TaskData[3];
OS_EVENT*ACKMbox;
OS_EVENT*TxMbox;


static  void  App_TaskStart(void  *p_arg);
static  void  App_TaskCreate(void);
static  void  App_Task1(void  *p_arg);
static  void  App_Task2(void  *p_arg);
static  void  App_Task3(void  *p_arg);


int  main (void)
{
    INT8U    os_err;
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif

    BSP_Init();                                                 
    BSP_IntDisAll();                                            


    CPU_Init();                                                 

    OSInit();                                                   

    os_err = OSTaskCreate(App_TaskStart,  			
                          (void*)0,
                          (OS_STK*)&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                           0);


    OSStart();                                                  

    return (0);
}


static  void  App_TaskStart (void *p_arg)
{
    CPU_INT32U  cnts;
    (void)p_arg;                                                

                                                                
    cnts = BSP_CPU_ClkFreq() / (CPU_INT32U)OS_TICKS_PER_SEC;
    OS_CPU_SysTickInit(cnts);                                   
    Mem_Init();                                                 
    Math_Init();                                               
    RIT128x96x4Init(1000000);
    BSP_SerInit(BSP_SER_ID_UART0, 115200u);
                                                                
    App_TaskCreate();    

    ACKMbox = OSMboxCreate((void*)0);
    TxMbox = OSMboxCreate((void*)0);

    BSP_Print("\r\nMailboxes created\r\n");


    while (1) {                                          
        BSP_LED_Toggle(1);
        OSTimeDly(OS_TICKS_PER_SEC / 4);
       }
}


static  void  App_TaskCreate (void)
{
    INT8U  os_err;
    RIT128x96x4StringDraw("Creating tasks...", 0, 0,15);
    os_err = OSTaskCreate(App_Task1, (void*)0, &App_Task1Stk[APP_TASK_START_STK_SIZE-1], 1);
    os_err = OSTaskCreate(App_Task2, (void*)0, &App_Task2Stk[APP_TASK_START_STK_SIZE-1], 2);
    os_err = OSTaskCreate(App_Task3, (void*)0, &App_Task3Stk[APP_TASK_START_STK_SIZE-1], 3);
}

void App_Task1(void *data) {

   INT8U err;
   OS_CPU_SR cpu_sr;
   char txmsg;
   void* rx;

   data=data;
   txmsg='A';

    while(1){
      err = OSMboxPost(TxMbox, txmsg);
      BSP_Print("\r\n Task1 send broadcast: %c", txmsg);

      err=OSMboxPostOpt(TxMbox, txmsg, OS_POST_OPT_BROADCAST); 

      //rx = OSMboxPend(ACKMbox, 0, &err);

      txmsg++;
      if(txmsg == 'Z')
        txmsg = 'A';

      OSTimeDly(100);
    }
}


void App_Task2(void *data){
   INT8U err;
   OS_CPU_SR cpu_sr;
   char *rxmsg;

   data=data;

   while(1){
    rxmsg = OSMboxPend(TxMbox, 0, &err);
     BSP_Print("\r\n Task2 recieved and acknowledged: %c", rxmsg);
     OSTimeDly(100);
   }
}

void App_Task3(void *data){
   INT8U err;
   OS_CPU_SR cpu_sr;
   char *rxmsg;

   data=data;

   while(1){
    rxmsg = OSMboxPend(TxMbox, 0, &err);
     BSP_Print("\r\n Task3 recieved and acknowledged: %c", rxmsg);
     OSTimeDly(100);
   }
}

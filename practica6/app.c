#include <includes.h>


static  OS_STK        App_TaskStartStk[APP_TASK_START_STK_SIZE];
static  OS_STK        App_Task1Stk[APP_TASK_START_STK_SIZE];
static  OS_STK        App_Task2Stk[APP_TASK_START_STK_SIZE];


static  void  App_TaskStart(void  *p_arg);
static  void  App_TaskCreate(void);
static  void  App_Task1(void  *p_arg);
static  void  App_Task2(void  *p_arg);


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
}
void App_Task1(void *data) {

   INT8U err;
   OS_CPU_SR cpu_sr;

    while(1){
      BSP_Print("\r\n This is Task11111111111");
	RIT128x96x4StringDraw("Creating task1111", 0, 0,15);
      OSTimeDly(100);
    }
}


void App_Task2(void *data){
   INT8U err;
   OS_CPU_SR cpu_sr;

   while(1){
     BSP_Print("\r\nThis is Task2222222222222");
     OSTimeDly(1);
   }
}


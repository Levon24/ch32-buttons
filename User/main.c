#include "debug.h"

#define BUTTON_SETTINGS GPIO_Pin_3
#define BUTTON_NEXT     GPIO_Pin_4
#define BUTTON_UP       GPIO_Pin_5
#define BUTTON_DOWN     GPIO_Pin_6

/* Global define */

/* Global Variable */

/*******************************************************************/
void initPortC() {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef initButtons = {0};
  initButtons.GPIO_Pin = BUTTON_SETTINGS | BUTTON_NEXT | BUTTON_UP | BUTTON_DOWN;
  initButtons.GPIO_Mode = GPIO_Mode_IPU;
  initButtons.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &initButtons);
}

/*********************************************************************
 * @fn      main
 * @brief   Main program.
 * @return  none
 */
int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  SystemCoreClockUpdate();
  Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
  SDI_Printf_Enable();
#else
  USART_Printf_Init(115200);
#endif
  printf("SystemClk: %d\r\n", SystemCoreClock);
  printf("ChipID: %08x\r\n", DBGMCU_GetCHIPID());

  initPortC();

  uint8_t cycles = 0;

  while (1) {
    uint16_t buttons = GPIO_ReadInputData(GPIOC);
    uint8_t pressed = 0;

    if ((buttons & BUTTON_SETTINGS) == 0) {
      cycles++;

      if (cycles > 16) {
        cycles = 0;
        pressed = 1;
      }
    } else {
      if (cycles > 10) {
        pressed = 1;

        printf("Cycles: %d\r\n", cycles);
        cycles = 0;
      }
    }

    if (pressed > 0) {
      printf("Pressed\r\n");
    }

    Delay_Ms(20);
  }
}

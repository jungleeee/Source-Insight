/**
  ********************************************************************************
  * @File Name    : motor.h
  * @Author       : Jungle
  * @Mail         : Mail
  * @Created Time : 2019/1/11 17:14:54
  * @Version      : V1.0
  * @Last Changed : 2019/1/11 17:14:54
  * @Brief        : brief
  ********************************************************************************
  */

/* Define to prevent recursive inclusion ---------------------------------------*/
#ifndef __MOTOR_H_
#define __MOTOR_H_

/* Inlcude ---------------------------------------------------------------------*/

/* Exported typedef ------------------------------------------------------------*/
/* Exported constants define ---------------------------------------------------*/
/* Exported macro define -------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------*/
/* Exported functions ----------------------------------------------------------*/
void motor_head_driver_init(void (* pfStateMachineCBack)(void));
void motor_body_driver_init(void (* pfStateMachineCBack)(void));

void motor_head_left(int speed);
void motor_head_right(int speed);
void motor_head_stop(int speed);

void motor_body_left(int speed);
void motor_body_right(int speed);
void motor_body_stop(int speed);

#endif /* __MOTOR_H_ */

/************************** Coopyright (C) Robelf 2019 *******END OF FILE********/


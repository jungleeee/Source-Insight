/**
  ********************************************************************************
  * @File Name    : motor.c
  * @Author       : Jungle
  * @Mail         : Mail
  * @Created Time : 2019/1/11 17:14:18
  * @Version      : V1.0
  * @Last Changed : 2019/1/11 17:14:18
  * @Brief        : brief
  ********************************************************************************
  */

/* Inlcude ---------------------------------------------------------------------*/
#include "stdio.h"

#include "gpio.h"
#include "tim.h"

/** @addtogroup
  * @{
  */

/* Private function declaration ------------------------------------------------*/
/* Private typedef -------------------------------------------------------------*/
typedef void (* pfMOTOR_ONE_STEP_CALLBACK)(void);

typedef struct sMOTOR_BLOCK {
    pfMOTOR_ONE_STEP_CALLBACK pfOneStepCallBack;
    struct sMOTOR_BLOCK *psPrev;
    struct sMOTOR_BLOCK *psNext;
} Motor_Block_Definition_t;

/* Private constants define ----------------------------------------------------*/
#define     MOTOR_DRIVE_STEP_NUM        8

/* Private macro define --------------------------------------------------------*/
#define     MOTOR_HEAD_SET_SPEED(speed) tim_set_head_state_machine_freq(speed)
#define     MOTOR_BODY_SET_SPEED(speed) tim_set_body_state_machine_freq(speed)

/* Private variables -----------------------------------------------------------*/
static Motor_Block_Definition_t xRegisterHeadDrivingTiming[MOTOR_DRIVE_STEP_NUM] = {0};
static Motor_Block_Definition_t *psHeadMotorDriverList = NULL;
static Motor_Block_Definition_t xRegisterBodyDrivingTiming[MOTOR_DRIVE_STEP_NUM] = {0};
static Motor_Block_Definition_t *psBodyMotorDriverList = NULL;

/* Private functions -----------------------------------------------------------*/
/**
  * @brief  : +A +B
  * @param  : None
  * @retval : None
  */
static void motor_head_papb_step(void)
{
    HEAD_MOTOR_PIN_PA_HIGH;
    HEAD_MOTOR_PIN_PB_HIGH;
    HEAD_MOTOR_PIN_NA_LOW;
    HEAD_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : +B
  * @param  : None
  * @retval : None
  */
static void motor_head_pb_step(void)
{
    HEAD_MOTOR_PIN_PA_LOW;
    HEAD_MOTOR_PIN_PB_HIGH;
    HEAD_MOTOR_PIN_NA_LOW;
    HEAD_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : -A +B
  * @param  : None
  * @retval : None
  */
static void motor_head_napb_step(void)
{
    HEAD_MOTOR_PIN_PA_LOW;
    HEAD_MOTOR_PIN_PB_HIGH;
    HEAD_MOTOR_PIN_NA_HIGH;
    HEAD_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : -A
  * @param  : None
  * @retval : None
  */
static void motor_head_na_step(void)
{
    HEAD_MOTOR_PIN_PA_LOW;
    HEAD_MOTOR_PIN_PB_LOW;
    HEAD_MOTOR_PIN_NA_HIGH;
    HEAD_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : -A -B
  * @param  : None
  * @retval : None
  */
static void motor_head_nanb_step(void)
{
    HEAD_MOTOR_PIN_PA_LOW;
    HEAD_MOTOR_PIN_PB_LOW;
    HEAD_MOTOR_PIN_NA_HIGH;
    HEAD_MOTOR_PIN_NB_HIGH;
}

/**
  * @brief  : -B
  * @param  : None
  * @retval : None
  */
static void motor_head_nb_step(void)
{
    HEAD_MOTOR_PIN_PA_LOW;
    HEAD_MOTOR_PIN_PB_LOW;
    HEAD_MOTOR_PIN_NA_LOW;
    HEAD_MOTOR_PIN_NB_HIGH;
}

/**
  * @brief  : +A -B
  * @param  : None
  * @retval : None
  */
static void motor_head_panb_step(void)
{
    HEAD_MOTOR_PIN_PA_HIGH;
    HEAD_MOTOR_PIN_PB_LOW;
    HEAD_MOTOR_PIN_NA_LOW;
    HEAD_MOTOR_PIN_NB_HIGH;
}

/**
  * @brief  : +A
  * @param  : None
  * @retval : None
  */
static void motor_head_pa_step(void)
{
    HEAD_MOTOR_PIN_PA_HIGH;
    HEAD_MOTOR_PIN_PB_LOW;
    HEAD_MOTOR_PIN_NA_LOW;
    HEAD_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : +A +B
  * @param  : None
  * @retval : None
  */
static void motor_body_papb_step(void)
{
    BODY_MOTOR_PIN_PA_HIGH;
    BODY_MOTOR_PIN_PB_HIGH;
    BODY_MOTOR_PIN_NA_LOW;
    BODY_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : +B
  * @param  : None
  * @retval : None
  */
static void motor_body_pb_step(void)
{
    BODY_MOTOR_PIN_PA_LOW;
    BODY_MOTOR_PIN_PB_HIGH;
    BODY_MOTOR_PIN_NA_LOW;
    BODY_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : -A +B
  * @param  : None
  * @retval : None
  */
static void motor_body_napb_step(void)
{
    BODY_MOTOR_PIN_PA_LOW;
    BODY_MOTOR_PIN_PB_HIGH;
    BODY_MOTOR_PIN_NA_HIGH;
    BODY_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : -A
  * @param  : None
  * @retval : None
  */
static void motor_body_na_step(void)
{
    BODY_MOTOR_PIN_PA_LOW;
    BODY_MOTOR_PIN_PB_LOW;
    BODY_MOTOR_PIN_NA_HIGH;
    BODY_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  : -A -B
  * @param  : None
  * @retval : None
  */
static void motor_body_nanb_step(void)
{
    BODY_MOTOR_PIN_PA_LOW;
    BODY_MOTOR_PIN_PB_LOW;
    BODY_MOTOR_PIN_NA_HIGH;
    BODY_MOTOR_PIN_NB_HIGH;
}

/**
  * @brief  : -B
  * @param  : None
  * @retval : None
  */
static void motor_body_nb_step(void)
{
    BODY_MOTOR_PIN_PA_LOW;
    BODY_MOTOR_PIN_PB_LOW;
    BODY_MOTOR_PIN_NA_LOW;
    BODY_MOTOR_PIN_NB_HIGH;
}

/**
  * @brief  : +A -B
  * @param  : None
  * @retval : None
  */
static void motor_body_panb_step(void)
{
    BODY_MOTOR_PIN_PA_HIGH;
    BODY_MOTOR_PIN_PB_LOW;
    BODY_MOTOR_PIN_NA_LOW;
    BODY_MOTOR_PIN_NB_HIGH;
}

/**
  * @brief  : +A
  * @param  : None
  * @retval : None
  */
static void motor_body_pa_step(void)
{
    BODY_MOTOR_PIN_PA_HIGH;
    BODY_MOTOR_PIN_PB_LOW;
    BODY_MOTOR_PIN_NA_LOW;
    BODY_MOTOR_PIN_NB_LOW;
}

/**
  * @brief  :
  * @param  : None
  * @retval : None
  */
static void motor_driving_timing_init(Motor_Block_Definition_t **head, Motor_Block_Definition_t *add, pfMOTOR_ONE_STEP_CALLBACK pfCallback)
{
    Motor_Block_Definition_t *p = *head;

    if(p == NULL) {//head = &psHeadMotorDriverList, *head = psHeadMotorDriverList
        //first node
        add->pfOneStepCallBack = pfCallback;
        add->psPrev = add;
        add->psNext = add;

        *head = add;

        return;
    }

    p = p->psPrev;//end, p != *head
    p->psNext = add;// end.next = new

    add->psPrev = p;
    add->pfOneStepCallBack = pfCallback;
    add->psNext = *head;

    p = *head;
    p->psPrev = add;
}

/**
  * @brief  : motor driver init
  * @param  : None
  * @retval : None
  */
void motor_head_driver_init(void (* pfStateMachineCBack)(void))
{
    int i = 0;

    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_papb_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_pb_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_napb_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_na_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_nanb_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_nb_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_panb_step);
    motor_driving_timing_init(&psHeadMotorDriverList, &xRegisterHeadDrivingTiming[i++], motor_head_pa_step);

    gpio_head_motor_init();

    tim_head_state_machine_init(pfStateMachineCBack);
}

/**
  * @brief  : motor driver init
  * @param  : None
  * @retval : None
  */
void motor_body_driver_init(void (* pfStateMachineCBack)(void))
{
    int i = 0;

    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_papb_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_pb_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_napb_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_na_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_nanb_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_nb_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_panb_step);
    motor_driving_timing_init(&psBodyMotorDriverList, &xRegisterBodyDrivingTiming[i++], motor_body_pa_step);

    gpio_body_motor_init();

    tim_body_state_machine_init(pfStateMachineCBack);
}

/**
  * @brief  : head motor anticlockwise
  * @param  : None
  * @retval : None
  */
void motor_head_left(int speed)
{
    psHeadMotorDriverList->pfOneStepCallBack();
    psHeadMotorDriverList = psHeadMotorDriverList->psNext;

    MOTOR_HEAD_SET_SPEED(speed);
}

/**
  * @brief  : head motor clockwise
  * @param  : None
  * @retval : None
  */
void motor_head_right(int speed)
{
    psHeadMotorDriverList->pfOneStepCallBack();
    psHeadMotorDriverList = psHeadMotorDriverList->psPrev;

    MOTOR_HEAD_SET_SPEED(speed);
}

/**
  * @brief  : head motor stop
  * @param  : None
  * @retval : None
  */
void motor_head_stop(int speed)
{
    HEAD_MOTOR_PIN_PA_HIGH;//positive
    HEAD_MOTOR_PIN_PB_HIGH;
    HEAD_MOTOR_PIN_NA_HIGH;//negative
    HEAD_MOTOR_PIN_NB_HIGH;

    MOTOR_HEAD_SET_SPEED(speed);
}

/**
  * @brief  : body motor left
  * @param  : None
  * @retval : None
  */
void motor_body_left(int speed)
{
    psBodyMotorDriverList->pfOneStepCallBack();
    psBodyMotorDriverList = psBodyMotorDriverList->psNext;

    MOTOR_BODY_SET_SPEED(speed);
}

/**
  * @brief  : body motor right
  * @param  : None
  * @retval : None
  */
void motor_body_right(int speed)
{
    psBodyMotorDriverList->pfOneStepCallBack();
    psBodyMotorDriverList = psBodyMotorDriverList->psPrev;

    MOTOR_BODY_SET_SPEED(speed);
}

/**
  * @brief  : body motor stop
  * @param  : None
  * @retval : None
  */
void motor_body_stop(int speed)
{
    BODY_MOTOR_PIN_PA_HIGH;//positive
    BODY_MOTOR_PIN_PB_HIGH;
    BODY_MOTOR_PIN_NA_HIGH;//negative
    BODY_MOTOR_PIN_NB_HIGH;

    MOTOR_BODY_SET_SPEED(speed);
}

/**
  * @}
  */

/************************** Coopyright (C) Robelf 2019 *******END OF FILE********/


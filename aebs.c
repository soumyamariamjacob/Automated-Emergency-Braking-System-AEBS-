#include "stm32f4xx.h"
#include "stdbool.h"

// Define Pin Connections
#define RADAR_SENSOR_PIN    GPIO_PIN_0   // Example pin for Radar sensor
#define BRAKE_ACTUATOR_PIN  GPIO_PIN_1   // Example pin for Braking Actuator
#define OVERRIDE_SWITCH_PIN GPIO_PIN_2   // Example pin for manual override

// Threshold distance in cm for braking
#define COLLISION_THRESHOLD 50.0

// Function Prototypes
void SystemClock_Config(void);
void GPIO_Init(void);
void ADC_Init(void);
uint16_t Read_ADC(void);
void Activate_Brakes(void);
void Deactivate_Brakes(void);
bool Check_Manual_Override(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    ADC_Init();

    while (1) {
        uint16_t sensor_value = Read_ADC();
        float distance = (5.0 / 4095.0) * sensor_value * 100.0; // Convert ADC value to distance
        
        if (distance < COLLISION_THRESHOLD && !Check_Manual_Override()) {
            Activate_Brakes();
        } else {
            Deactivate_Brakes();
        }
        
        HAL_Delay(100); // Small delay for stability
    }
}

void SystemClock_Config(void) {
    // System clock configuration (HAL generated code usually goes here)
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Configure Brake Actuator Pin as Output
    GPIO_InitStruct.Pin = BRAKE_ACTUATOR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Configure Manual Override Pin as Input
    GPIO_InitStruct.Pin = OVERRIDE_SWITCH_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void ADC_Init(void) {
    __HAL_RCC_ADC1_CLK_ENABLE();
    ADC_HandleTypeDef hadc1;
    hadc1.Instance = ADC1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    HAL_ADC_Init(&hadc1);
    
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

uint16_t Read_ADC(void) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}

void Activate_Brakes(void) {
    HAL_GPIO_WritePin(GPIOA, BRAKE_ACTUATOR_PIN, GPIO_PIN_SET);
}

void Deactivate_Brakes(void) {
    HAL_GPIO_WritePin(GPIOA, BRAKE_ACTUATOR_PIN, GPIO_PIN_RESET);
}

bool Check_Manual_Override(void) {
    return HAL_GPIO_ReadPin(GPIOA, OVERRIDE_SWITCH_PIN) == GPIO_PIN_SET;
}

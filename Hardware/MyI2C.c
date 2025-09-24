#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define I2C_GPIO_INIT RCC_APB2Periph_GPIOB
#define I2C_GPIO GPIOB
#define I2C_SCL_PIN GPIO_Pin_4
#define I2C_SDA_PIN GPIO_Pin_3

/**
   * @brief 用于主机软件模拟控制SCL
   * @param 高电平还是低电平，即0/1
   * @retval 无
   */
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(I2C_GPIO,I2C_SCL_PIN,(BitAction)BitValue);
//	Delay_us(10);//每写一次就延时一段时间
}

/**
	 * @brief 用于主机把单个比特位数据放到SDA总线上
   * @param 高电平还是低电平，即0/1
   * @retval 无
   */
void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(I2C_GPIO,I2C_SDA_PIN,(BitAction)BitValue);
//	Delay_us(10);//每写一次就延时一段时间
}

/**
   * @brief 用于主机读取在SDA总线上的一个比特位
   * @param 无
   * @retval 高电平还是低电平，即0/1
   */
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue=GPIO_ReadInputDataBit(I2C_GPIO,I2C_SDA_PIN);
//	Delay_us(10);//每读一次就延时一段时间
	return BitValue;
}

/**
   * @brief I2C总线初始化
   * @param
   * @retval
   */
void MyI2C_Init(void)
{
	//禁用JTAG，启用SWD（保留SWCLK和SWDIO用于调试，释放PB3/PB4）
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	RCC_APB2PeriphClockCmd(I2C_GPIO_INIT,ENABLE);//开启GPIOB时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//开漏输出
	GPIO_InitStructure.GPIO_Pin=I2C_SCL_PIN | I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO,&GPIO_InitStructure);
	
	GPIO_SetBits(I2C_GPIO, I2C_SCL_PIN | I2C_SDA_PIN);//I2C总线空闲
}

/**
   * @brief 主机产生I2C通信开始标志
   * @param 无
   * @retval 无
   */
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);MyI2C_W_SCL(1);//先手动置高，以兼容Start Repeat
	MyI2C_W_SDA(0);MyI2C_W_SCL(0);
}

/**
   * @brief 主机产生I2C通信结束标志
   * @param 无
   * @retval 无
   */
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);//先拉低SDA
	MyI2C_W_SCL(1);MyI2C_W_SDA(1);
}

/**
   * @brief 主机向从机发送一个字节
   * @param 要发送的字节
   * @retval 无
   */
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MyI2C_W_SDA(Byte&(0x80>>i));//主机把各位数据放在SDA总线
		MyI2C_W_SCL(1);
		//从机此时接收
		MyI2C_W_SCL(0);
	}
}

/**
   * @brief 主机接收来自从机的一个字节
   * @param 无
   * @retval 接收到的字节
   */
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i,Byte=0x00;
	MyI2C_W_SDA(1);//主机释放SDA控制权
	//此时从机把数据放到SDA上
	for(i=0;i<8;i++)//主机开始读取
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA()==1){Byte|=(0x80>>i);}//依次从高位开始判断每一位是否为1，读出一个字节
		MyI2C_W_SCL(0);
	}
	return Byte;
}

/**
   * @brief 主机发送接收应答位
   * @param 0为接收到了，1为没有接收到
   * @retval 无
   */
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);//主机把应答位放在SDA总线
	MyI2C_W_SCL(1);
	//从机此时读取来自主机的应答位
	MyI2C_W_SCL(0);
}

/**
   * @brief 主机接收来自从机的接收应答
   * @param 无
   * @retval 0为接收到了，1为没有接收到
   */
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);//主机释放SDA控制权
	//此时从机把AckBit放到SDA上
	MyI2C_W_SCL(1);//主机开始读取
	AckBit=MyI2C_R_SDA();//主机读取SDA上来自从机的AckBit
	MyI2C_W_SCL(0);
	return AckBit;
}

void MPU6050_GetData()
{
	
}

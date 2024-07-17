/*
 * @Author       : yzy
 * @Date         : 2022-05-16 12:42:47
 * @LastEditors  : stark1898y 1658608470@qq.com
 * @LastEditTime : 2024-07-17 16:29:40
 * @Description  : CS1621
 * @FilePath     : \SCV_ControlBox_CH32V303RCT6_BC260\bsp\src\bsp_lcd_1621.c
 */
#include "bsp_lcd_1621.h"


static uint8_t lcd_buffer[LCD_BUFFER_LEN] = {0};

/**
 * @description: 向1621写一个byte的n位(从左到右)
 * @param {uint8_t} bit
 * @param {uint8_t} num
 * @return {*}
 */
static void LCD_WriteBit(uint8_t byte, uint8_t num)
{
  uint8_t i = 0;

  for (i = 0; i < num; i++)
  {
    LCD_WR_PIN_LOW; //拉低WR脚  Data线上的数据在WR脚位上升沿写入
    rt_thread_mdelay(1000*1);
    if ((byte << i) & 0x80) //如果当前bit为1，就拉高DATA引脚
    {
      LCD_DATA_PIN_HIGH;
    }
    else //如果当前bit为0，就拉低DATA引脚
    {
      LCD_DATA_PIN_LOW;
    }
    rt_thread_mdelay(1000*1);
    LCD_WR_PIN_HIGH; //当前这一bit写完，拉高WR脚，为下次WR为上升沿做准备
    rt_thread_mdelay(1000*5);
  }
}

/**
 * @description: 将要显示的数据写到1621
 * @param {uint8_t} addr  写入低6位地址 SEG(0-31)
 * @param {uint8_t} bit4  写入低四位数据 COM(bit0-bit3)
 * @return {*}
 */
static void LCD_WriteSeg(uint8_t seg_6bit, uint8_t com_4bit)
{
  LCD_CS_PIN_LOW;           //拉低CS脚  CS拉低时WR有效
  rt_thread_mdelay(1000*1);
  LCD_WriteBit(WR_DATA, 3);
  LCD_WriteBit(seg_6bit<<2, 6); //写入地址
  LCD_WriteBit(com_4bit<<4, 4); //写入数据  这里是对应单个SEG写的 故而为4
  LCD_CS_PIN_HIGN;
}

/**
 * @description: 写命令到1621
 * @param {uint8_t} cmd
 * @return {*}
 */
static void LCD_WriteCmd(uint8_t cmd)
{
  LCD_CS_PIN_LOW;           //拉低CS脚  CS拉低时WR有效
  rt_thread_mdelay(1000*1);
  LCD_WriteBit(WR_CMD, 3);  //写入命令标志100    0x80 3
  LCD_WriteBit(cmd, 8);  //写入命令数据
  LCD_WriteBit(0, 1);
  LCD_CS_PIN_HIGN;
}

/**
 * @description: 1621初始化
 * @param {*}
 * @return {*}
 */
void LCD_Init(void)
{
  // IO初始化
  rt_pin_mode(LCD_CS_PIN, PIN_MODE_OUTPUT);
  rt_pin_mode(LCD_WR_PIN, PIN_MODE_OUTPUT);
  rt_pin_mode(LCD_DATA_PIN, PIN_MODE_OUTPUT);

  LCD_CS_PIN_HIGN;
  LCD_WR_PIN_HIGH;
  LCD_DATA_PIN_HIGH;

  rt_thread_mdelay(1);
  // 配置1621
  LCD_WriteCmd(SYS_EN);
  LCD_WriteCmd(BIAS_3_4COM);  // 4COM端 LCD:1/3偏压
  LCD_WriteCmd(XTAL_32K);
  // LCD_WriteCmd(RC_256K);
  // LCD_WriteCmd(SYS_DIS);
  LCD_WriteCmd(WDT_DIS);
  LCD_WriteCmd(LCD_ON);
  // LCD_WriteCmd(TONE_4K);
  LCD_Clear();
  rt_thread_mdelay(1);
  LCD_ShowIcon(S5_PRESSURE_TIMING_VALVES, kShowOpen);
  LCD_Refresh();
}

/**
 * @description: 屏幕刷新
 * @param {*}
 * @return {*}
 */
void LCD_Refresh(void)
{
  uint8_t i = 0;
  for(i=0; i<LCD_BUFFER_LEN; i++)
  {
    LCD_WriteSeg(i, lcd_buffer[i]);
  }
}

/**
 * @description: 清屏
 * @param {*}
 * @return {*}
 */
void LCD_Clear(void)
{
  rt_memset(&lcd_buffer, 0, sizeof(lcd_buffer));
  LCD_Refresh();
}

/**
 * @description:
 * @param {LcdSeg} seg     0-13(LCD_BUFFER_LEN-1)
 * @param {LcdCom} com      1-4
 * @param {LcdSegmentShowControl} mode    kShowClose：熄灭；kShowOpen:点亮
 * @return {*}
 */
void LCD_ShowSegment(LcdSeg seg, LcdCom com, LcdSegmentShowControl mode)
{
  // 关闭
  // LCD_WriteBit(com_4bit<<4, 4); //写入数据  这里是对应单个SEG写的 故而为4
  if(mode == kShowClose)
  {
    if(com == LCD_COM1)
    {
      lcd_buffer[seg] &= B8(11110111);    //  1111 0111
    }
    else if(com == LCD_COM2)
    {
      lcd_buffer[seg] &= B8(11111011);    //  1111 1011
    }
    else if(com == LCD_COM3)
    {
      lcd_buffer[seg] &= B8(11111101);    //  1111 1101
    }
    else if(com == LCD_COM4)
    {
      lcd_buffer[seg] &= B8(11111110);    //  1111 1110
    }
  }
  // 开启
  else if(mode == kShowOpen)
  {
    if(com == LCD_COM1)
    {
      lcd_buffer[seg] |= B8(00001000);    //  0000 1000
    }
    else if(com == LCD_COM2)
    {
      lcd_buffer[seg] |= B8(00000100);    //  0000 0100
    }
    else if(com == LCD_COM3)
    {
      lcd_buffer[seg] |= B8(00000010);    //  0000 0010
    }
    else if(com == LCD_COM4)
    {
      lcd_buffer[seg] |= B8(00000001);    //  0000 0001
    }
  }
}

/**
 * @description: 显示图标
 * @param {LcdIconIndex} index
 * @param {LcdSegmentShowControl} mode  kShowClose：熄灭；kShowOpen:点亮
 * @return {*}
 */
void LCD_ShowIcon(LcdIconIndex index, LcdSegmentShowControl mode)
{
  switch (index)
  {
    case S1_OVER_PRESSURE:
      LCD_ShowSegment(LCD_SEG0, LCD_COM4, mode);
      break;
    case S2_UNDER_PRESSURE:
      LCD_ShowSegment(LCD_SEG0, LCD_COM3, mode);
      break;
    case S3_OVER_CURRENT:
      LCD_ShowSegment(LCD_SEG0, LCD_COM2, mode);
      break;
    case S4_LEAKAGE:
      LCD_ShowSegment(LCD_SEG0, LCD_COM1, mode);
      break;
    case S5_PRESSURE_TIMING_VALVES:
      LCD_ShowSegment(LCD_SEG2, LCD_COM1, mode);
      break;
    case S6_NETWORK:
      LCD_ShowSegment(LCD_SEG7, LCD_COM3, mode);
      break;
    case S7_LEAK_HUNTING:
      LCD_ShowSegment(LCD_SEG7, LCD_COM2, mode);
      break;
    case S8_FAULT:
      LCD_ShowSegment(LCD_SEG8, LCD_COM4, mode);
      break;
    case S9_ALARM:
      LCD_ShowSegment(LCD_SEG13, LCD_COM4, mode);
      break;

    case L1_LINE:
      LCD_ShowSegment(LCD_SEG6, LCD_COM1, mode);
      break;
    case L2_LINE:
      LCD_ShowSegment(LCD_SEG10, LCD_COM1, mode);
      break;
    case L3_LINE:
      LCD_ShowSegment(LCD_SEG8, LCD_COM1, mode);
      break;
    case L4_LINE:
      LCD_ShowSegment(LCD_SEG13, LCD_COM1, mode);
      break;

    case K1_SELF_CLOSING_VALVE_ON:
      LCD_ShowSegment(LCD_SEG8, LCD_COM2, mode);
      break;
    case K2_SELF_CLOSING_VALVE_OFF:
      LCD_ShowSegment(LCD_SEG8, LCD_COM3, mode);
      break;
    case K3_HAND_VALVE_ON:
      LCD_ShowSegment(LCD_SEG13, LCD_COM2, mode);
      break;
    case K4_HAND_VALVE_OFF:
      LCD_ShowSegment(LCD_SEG13, LCD_COM3, mode);
      break;

    case ICON_POINT:
      LCD_ShowSegment(LCD_SEG4, LCD_COM1, mode);
      break;
    case ICON_KPA:
      LCD_ShowSegment(LCD_SEG7, LCD_COM4, mode);
      break;
    case ICON_MIN:
      LCD_ShowSegment(LCD_SEG12, LCD_COM1, mode);
      break;

    default:
      break;
  }
}

/**
 * @description: LCD显示数字
 * @param {LcdNumIndex} index
 * @param {uint8_t} num (0-9)
 * @param {LcdSegmentShowControl} mode
 * @return {*}
 */
void LCD_ShowNum(LcdNumIndex index, uint8_t num, LcdSegmentShowControl mode)
{
  if(mode == kShowOpen)
  {
    if(num == 0)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowOpen);      // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowOpen);      // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowClose);     // G
    }
    else if(num == 1)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowClose);       // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowClose);       // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);       // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowClose);       // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowClose);     // G
    }
    else if(num == 2)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowClose);     // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowOpen);      // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowClose);       // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
    else if(num == 3)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);       // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowClose);       // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
    else if(num == 4)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowClose);       // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowClose);       // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);       // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowOpen);      // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
    else if(num == 5)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowClose);     // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);       // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowOpen);      // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
    else if(num == 6)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowClose);     // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowOpen);      // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowOpen);      // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
    else if(num == 7)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowClose);       // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);       // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowClose);       // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowClose);     // G
    }
    else if(num == 8)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowOpen);      // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowOpen);      // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
    else if(num == 9)
    {
      LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowOpen);      // A
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowOpen);    // B
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowOpen);    // C
      LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowOpen);      // D
      LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);       // E
      LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowOpen);      // F
      LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowOpen);    // G
    }
  }
  else if(mode == kShowClose)
  {
    LCD_ShowSegment((LcdSeg)index, LCD_COM4, kShowClose);         // A
    LCD_ShowSegment((LcdSeg)(index+1), LCD_COM4, kShowClose);     // B
    LCD_ShowSegment((LcdSeg)(index+1), LCD_COM2, kShowClose);     // C
    LCD_ShowSegment((LcdSeg)index, LCD_COM1, kShowClose);         // D
    LCD_ShowSegment((LcdSeg)index, LCD_COM2, kShowClose);         // E
    LCD_ShowSegment((LcdSeg)index, LCD_COM3, kShowClose);         // F
    LCD_ShowSegment((LcdSeg)(index+1), LCD_COM3, kShowClose);     // G
  }
}

/**
 * @description: LCD显示压力数值
 * @param {uint32_t} pressure  (0-99999)
 * @param {LcdSegmentShowControl} mode
 * @return {*}
 */
void LCD_ShowPressure(uint32_t pressure, LcdSegmentShowControl mode)
{
  uint8_t num_1, num_2, num_3;

  // 显示.
  LCD_ShowIcon(ICON_POINT, mode);
  // 显示kPa
  LCD_ShowIcon(ICON_KPA, mode);

  LCD_ShowNum(KPA_NUM_1, 0, kShowClose);
  // 10k+ pa
  if(pressure >= 10000)
  {
    num_1 = pressure / 10000;
    num_2 = (pressure / 1000) % 10;
    num_3 = (pressure / 100) % 10;
    // 显示十位
    LCD_ShowNum(KPA_NUM_1, num_1, mode);
  }
  // 1k+ pa
  else if(pressure >= 1000)
  {
    num_2 = pressure / 1000;
    num_3 = (pressure / 100) % 10;
  }
  // 100+ pa
  else if(pressure >= 100)
  {
    num_2 = 0;
    num_3 = pressure / 100;
  }
  else
  {
    num_1 = 0;
    num_2 = 0;
    num_3 = 0;
  }
  // 显示个位
  LCD_ShowNum(KPA_NUM_2, num_2, mode);
  // 显示小数点后一位
  LCD_ShowNum(KPA_NUM_3, num_3, mode);
}

/**
 * @description: LCD显示定时时间
 * @param {uint8_t} time  (0-99)
 * @param {LcdSegmentShowControl} mode
 * @return {*}
 */
void LCD_ShowTime(uint8_t time, LcdSegmentShowControl mode)
{
  uint8_t num_1, num_2;
  // 显示min
  LCD_ShowIcon(ICON_MIN, mode);
  if(time >= 10)
  {
    // 取十位
    num_1 = time / 10;
    // 取个位
    num_2 = time % 10;
    // 显示十位
    LCD_ShowNum(MIN_NUM_1, num_1, mode);
  }
  else
  {
    // 关闭十位
    LCD_ShowNum(MIN_NUM_1, 0, kShowClose);
    // 取个位
    num_2 = time % 10;
  }
  // 显示个位
  LCD_ShowNum(MIN_NUM_2, num_2, mode);
}


























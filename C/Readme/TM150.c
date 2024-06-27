// 通讯前关闭总中断，否则在IO引脚输入输出转换时会卡住。

s16 xtem = tem;
xtem     = (xtem < -99) ? -99 : xtem;
xtem     = (xtem > 999) ? 999 : xtem;
if (xtem < 0) {
    tm1650_write(0, 17, FALSE);
    xtem = ~xtem + 1;
    tm1650_write(1, xtem / 10 % 10, TRUE);
    tm1650_write(2, xtem % 10, FALSE);
} else {
    if (xtem < 100) {
        tm1650_write(0, 19, FALSE);
    } else {
        tm1650_write(0, xtem / 100 % 100, FALSE);
    }
    tm1650_write(1, xtem / 10 % 10, TRUE);
    tm1650_write(2, xtem % 10, FALSE);
}
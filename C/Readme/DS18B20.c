// 与 DS18B20 通讯时关闭总中断，否则容易通讯出错

while (!ds18b20_init()) {}
ds18b20_convert();
while (!ds18b20_init()) {}
tem = ds18b20_convert_get_tem(ds18b20_read());
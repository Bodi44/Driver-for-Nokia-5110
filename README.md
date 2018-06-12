# Driver-for-Nokia-5110
**Виконали Штогрінець Богдан, Семенюк Павло, Попов Ростислав.**
## Опис:
Драйвер дислея Nokia 5110 для Raspberry Pi
### Системні вимоги:
Raspberry Pi, Операційна система -- Raspbian, дисплей Nokia 5110

### Установка необхідного ПЗ:
**_1. Встановити kernel-space headers:_**
```
sudo apt-get install raspberrypi-kernel-headers
```
**_2. Виконати bash-script:_**
```
sudo bash script.sh
```
*Компілює, встановлює модуль в систему та виводить повідомлення про всі потоки виводу kernel*

**_Для того шоб вигрузити наш драйвер з системи:_**
```
sudo rmmod project.ko
```

###Використання:
В процесі....



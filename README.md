# Driver-for-Nokia-5110
**Виконали Штогрінець Богдан, Семенюк Павло, Попов Ростислав.**

## Системні вимоги:
Raspberry Pi, Операційна система Raspbian

### Установка необхідного ПЗ:
**_1. Встановити kernel-space headers:_**
```
sudo apt-get install raspberrypi-kernel-headers
```
**_2. Заранити bash-script:_**
```
sudo bash script.sh
```
*Компіляє, встановлює модуль в систему та виводить повідомлення про всі потоки виводу kernel*

**_Для того шоб видалити наш драйвер з системи:_**
```
rmmod project.ko
```

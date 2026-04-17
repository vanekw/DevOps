# Simple Docker

Введение в докер. Разработка простого докер-образа для собственного сервера.

## Part 1. Готовый докер

В этой главе разберемся с готовым докер образом ngnix.

- Возьмем официальный докер-образ с nginx и выкачаем его при помощи `docker pull ngnix`.  
![docker pull](resources/img/1.1.png)

- Проверим наличие докер-образа через `docker images`.  
![docker images](resources/img/1.2.png)

- Запустим докер-образ через `docker run -d 66f8bdd3810c`.  
![docker run](resources/img/1.3.png)

- Проверим, что образ запустился через `docker ps`.  
![docker ps](resources/img/1.4.png)

- Посмотрим информацию о контейнере через `docker inspect 69e9eca5c8b4`.  
![docker inspect](resources/img/1.5.png)

- Размер контейнера равен *67108864*.  
![size](resources/img/1.6.png)

- Нет замапленых портов.  
![Замапленые порты не отображаются](resources/img/1.7.png)

- Ip контейнера  
![ip контейнера](resources/img/1.8.png)

- Остановим докер контейнер через `docker stop 69e9eca5c8b4` и проверим что он остановился `docker ps`.  
![docker stop](resources/img/1.9.png)

- Запустим докер с портами 80 и 443 в контейнере, замапленными на такие же порты на локальной машине, через команду `docker run -d -p 80:80 -p 443:443 66f8bdd3810c`.  
![docker run](resources/img/1.10.png)

- Проверяем localhost:80.  
![localhost:80](resources/img/1.11.png)

- Перезапустим докер контейнер через `docker restart 02f2d1d7af47` и проверим что он перезапустился  `docker ps`.  
![docker restart](resources/img/1.12.png)

- Проверяем что контейнер перезапустился localhost:80.  
![localhost:80](resources/img/1.13.png)

## Part 2. Операции с контейнером

В этой главе поработаем с конфигурациями nginx и отобразим статус страницы.

- Прочитаем конфигурационный файл nginx.conf внутри докер контейнера через команду `docker exec 02f2d1d7af47 cat /etc/nginx/nginx.conf`.  
![docker exec](resources/img/2.1.png)

- Создим на локальной машине файл nginx.conf.  
![nginx.conf](resources/img/2.2.png)

- Настрим в нем по пути /status отдачу страницы статуса сервера nginx.  
![nginx.conf](resources/img/2.3.png)

- Скопируем созданный файл nginx.conf внутрь докер-образа через команду `docker cp nginx.conf 02f2d1d7af47:etc/nginx/`.  
![nginx.conf](resources/img/2.4.png)

- Проверим что файл сопировался с помощью командыы `docker exec 02f2d1d7af47 cat /etc/nginx/nginx.conf`.  
![nginx.conf](resources/img/2.5.png)

- Перезапустим nginx внутри докер-образа через команду `docker exec 02f2d1d7af47 nginx -s reload`.  
![docker reload](resources/img/2.6.png)

- Проверим, что по адресу localhost:80/status отдается страничка со статусом сервера nginx.  
![localhost:80/status](resources/img/2.7.png)

- Экспортируем контейнер в файл container.tar через команду `docker export 02f2d1d7af47 > container.tar`.  
![docker export](resources/img/2.8.png)

- Остановим контейнер `docker stop 02f2d1d7af47`.  
![docker stop](resources/img/2.9.png)

- Удали образ через `docker rmi -f nginx`, не удаляя перед этим контейнеры.  
![docker rmi](resources/img/2.10.png)

- Удалим остановленный контейнер `docker rm 02f2d1d7af47`.  
![docker rm](resources/img/2.11.png)

- Импортируем контейнер обратно через команду `docker import container.tar titandea`.  
![docker import](resources/img/2.12.png)

- Провери образы `docker images`.  
![docker images](resources/img/2.13.png)

- Запустим контейнер `docker run -d -p 80:80 -p 443:443 e5808a54319a nginx -g "daemon off;"`. Так как при импорте не была укзана команда по умалчанию при запуске контейнера, то укажем команду вручную.  
![docker run](resources/img/2.14.png)

- Проверим, что по адресу localhost:80/status отдается страничка со статусом сервера nginx.  
![localhost:80/status](resources/img/2.15.png)

## Part 3. Операции с контейнером

В этой главе напишем свой докер сервер.

- Обновим все пакеты, а так же установим необходимые. `sudo apt update` -> `sudo apt-get install libfcgi-dev` -> `sudo apt install spawn-fcgi`
![sudo apt update](resources/img/3.1.png)
![libfcgi-dev](resources/img/3.2.png)
![spawn-fcgi](resources/img/3.3.png)

- Напишем код сервера.  
![server.c](resources/img/3.4.png)

- Скачем образ ngnix с помщью `docker pull ngnix`.  
![docker pull ngnix](resources/img/3.5.png)

- Скомпилируем сервер `gcc -Wall -Werror -Wextra server.c -lfcgi -o server.cgi`
![sever компиляция](resources/img/3.6.png)

- Запустим докер с портами 80 и 443 в контейнере, замапленными на такие же порты на локальной машине, через команду `docker run -d -p 80:80 -p 443:443 66f8bdd3810c`.  
![docker run](resources/img/3.7.png) 

- Скопируем скомпилированный сервер в контейнер `docker cp server.cgi 49cb2c606c1b:/`.  
![sever компиляция](resources/img/3.8.png)

- Установим spawn-fcgi в контейнер `docker exec 49cb2c606c1b apt-get update` -> `docker exec 49cb2c606c1b apt-get install spawn-fcgi`.  
![apt-get update](resources/img/3.9.png)  
![spawn-fcgi](resources/img/3.10.png)

- Зайдем в директорию контейнера и установим libfcgi0ldbl `docker exec -it 49cb2c606c1b /bin/bash` -> `apt-get install libfcgi0ldbl`.  
![libfcgi0ldbl](resources/img/3.11.png)

- Запустим `docker exec 49cb2c606c1b spawn-fcgi -p 8080 -n server.cgi`.  
![spawn-fcgi](resources/img/3.12.png)

- Напиши свой nginx.conf, который нужен для запуска своего сервера.    
![nginx.conf](resources/img/3.13.png)

- Скопируем созданный файл nginx.conf внутрь докер-образа через команду `docker cp nginx.conf 49cb2c606c1b:etc/nginx/`.  
![nginx.conf](resources/img/3.14.png)

- Перезапустим nginx внутри докер-образа через команду `docker exec 02f2d1d7af47 nginx -s reload`.  
![docker reload](resources/img/3.15.png)

- Проверим, что по адресу localhost:80 отдается страничка "Hello World!".    
![localhost:80/status](resources/img/3.16.png)

- Напиши свой nginx.conf, который будет проксировать все запросы с 81 порта на 127.0.0.1:8080. Так как нынешний контейнер запускается с замаппленными портами 80 и 443, необходимо сохранить новый образ на основе этого контейнера и перезапустить с замапленым поротом 81.  
![commit](resources/img/3.17.png)
![docker run](resources/img/3.18.png)

- Скопируем созданный файл nginx.conf внутрь докер-образа через команду `docker cp nginx.conf 41a2634fd10c:etc/nginx/`.  
![nginx.conf](resources/img/3.19.png)
![nginx.conf](resources/img/3.20.png)


- Перезапустим nginx внутри докер-образа через команду `docker exec 41a2634fd10c nginx -s reload`.  
![docker reload](resources/img/3.21.png)

- Проверим, что по адресу localhost:80 отдается страничка "Hello World!".  
![localhost:80/status](resources/img/3.22.png)

## Part 4. Операции с контейнером

В этой главе напишем докер образ для созданного сервера.

- Напишем свой докер-образ, который:  

    1) собирает исходники мини сервера на FastCgi из Части 3;  

    2) запускает его на 8080 порту;  

    3) копирует внутрь образа написанный ./nginx/nginx.conf;  

    4) запускает nginx. (nginx можно установить внутрь докера самостоятельно, а можно воспользоваться готовым образом с nginx'ом, как базовым).  
![Dockerfile](resources/img/4.1.png)
![start.sh](resources/img/4.2.png)

- Создадим скрипт build.sh, который будет собирать образ и запускать контейнер с маппингом 81 порта на 80 на локальной машине и маппингом папки ./nginx внутрь контейнера по адресу, где лежат конфигурационные файлы nginx'а.  
![build.sh](resources/img/4.3.png)

- Проверим что build.sh все собрал и что все работает.  
![build.sh](resources/img/4.4.png)

- Проверим, что по localhost:80 доступна страничка написанного мини сервера.  
![localhost:80](resources/img/4.5.png)

- Проверим, что теперь по localhost:80/status отдается страничка со статусом nginx.  
![localhost:80/status](resources/img/4.6.png)

## Part 5. Dockle

В этой главе провери написанный докер образ на правильность с помощью Dockle.  

- Скачаем Dockle `curl -L -o dockle.deb https://github.com/goodwithtech/dockle/releases/download/v0.4.14/dockle_0.4.14_Linux-64bit.deb` -> `sudo dpkg -i dockle.deb && rm dockle.deb`.  
![Dockle](resources/img/5.1.png)

- Просканируем наш образ.  
    - Первая ошибка возникает в связи с использованием nginx. Так как по заданию нужно использовать nginx, то эту ошибку мы исправлять не будем.  
![Dockle](resources/img/5.2.png)
![Dockle](resources/img/5.3.png)
- `DKL-DI-0005: Clear apt-get caches` данная ошибка возникает, так как мы не чистим кэш от библиотек после их установки.  

- `CIS-DI-0001: Create a user for the container` нужно задать пользователя для контейнера.  

- `CIS-DI-0005: Enable Content trust for Docker` нужно включить доверие к контену докера.  

- `CIS-DI-0006: Add HEALTHCHECK instruction to the container image` нужно добавить отслеживание состояние приложения.  

- `CIS-DI-0008: Confirm safety of setuid/setgid files` нужно удалить или ограничить права setuid/setgid у перчисленных файлов.  
![Dockle](resources/img/5.4.png)

- Вызовем buil.sh  и команду `export DOCKER_CONTENT_TRUST=1`.  
![new image](resources/img/5.5.png)

- Просканируем наш образ. Как видим осталась только ошибка от nginx.  
![Dockle](resources/img/5.6.png)

- Проверим, что по localhost:80 доступна страничка написанного мини сервера.  
![localhost:80](resources/img/5.7.png)

- Проверим, что по localhost:80/status отдается страничка со статусом nginx.  
![localhost:80/status](resources/img/5.8.png)

## Part 6. Базовый Docker Compose

- Установим docker-compose `sudo curl -L "https://github.com/docker/compose/releases/download/1.29.2/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose`.  

- Установим право на выполнение `sudo chmod +x /usr/local/bin/docker-compose`.  

- Провемрим версию `docker-compose --version`.  
![docker-compose](resources/img/6.1.png)

- Напиши файл docker-compose.yml, с помощью которого:

    1) Подними докер-контейнер из Части 5 (он должен работать в локальной сети, т. е. не нужно использовать инструкцию EXPOSE и мапить порты на локальную машину).

    2) Подними докер-контейнер с nginx, который будет проксировать все запросы с 8080 порта на 81 порт первого контейнера.
![docker-compose.yml](resources/img/6.2.png)
- В это файле описан процесс сборки двух конитейнеров: первый контейнер собирается из конфигурайии 5 задания но без мапинга портов под именем container1. Второй контейнер называется nginx, собирается на основе образа nginx, vонтирует локальный файл nginx.conf в контейнер по пути /etc/nginx/nginx.conf, зависит от container1, запросы на порт 80 на вашей локальной машине будут перенаправлены на порт 8080 внутри контейнера nginx.
- Конфигурация второго контейнера.  
![nginx.conf](resources/img/6.3.png)

- Проверим, что по localhost:80 доступна страничка написанного мини сервера.  
![localhost:80](resources/img/6.4.png)

- Проверим, что по localhost:80/status отдается страничка со статусом nginx.  
![localhost:80/status](resources/img/6.5.png)
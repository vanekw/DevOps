# берем за основу nginx
FROM nginx 

# копируем конфигурацию, скрипт для докерфайла, сервер
COPY nginx/nginx.conf /etc/nginx/
COPY start.sh /
COPY server.c /

# качаем библиотеки и компилим сервер
RUN apt-get update &&  \
    apt-get install -y gcc spawn-fcgi libfcgi-dev && \ 
    gcc server.c -o webserver -l fcgi && \
    chmod +x /start.sh

# вызываем скрипт запуска nginx
ENTRYPOINT ["./start.sh"]
# берем за основу nginx
FROM nginx 

# копируем конфигурацию, скрипт для докерфайла, сервер
COPY nginx/nginx.conf /etc/nginx/
COPY start.sh /
COPY server.c /

# качаем библиотеки и компилим сервер
RUN apt-get update &&  \
    apt-get install -y gcc spawn-fcgi libfcgi-dev && \ 
    rm -rf /var/lib/apt/lists && \ 
    gcc server.c -o webserver -l fcgi && \
    chmod +x /start.sh
# добавляем очистку кеша после скачки пиблиотеки

RUN chown -R nginx:nginx /etc/nginx/nginx.conf; \
    chown -R nginx:nginx /var/cache/nginx; \
    chown -R nginx:nginx /home; \
    touch /var/run/nginx.pid; \
    chown -R nginx:nginx /var/run/nginx.pid; \
    chmod 255 /usr/bin/chage; \
    chmod u-s /usr/bin/chfn; \
    chmod u-s /usr/bin/chsh; \
    chmod u-s /bin/su; \
    chmod 255 /sbin/unix_chkpwd; \
    chmod u-s /bin/mount; \
    chmod u-s /bin/umount; \
    chmod u-s /usr/bin/newgrp; \
    chmod 255 /usr/bin/expiry; \
    chmod u-s /usr/bin/gpasswd; \
    chmod 755 /usr/bin/passwd 

# добавляем пользователя
USER nginx 
# добавляем отслеживание состояние приложения
HEALTHCHECK --interval=5m --timeout=3s CMD curl -f http://localhost/ || exit 1

# вызываем скрипт запуска nginx
ENTRYPOINT ["./start.sh"]



